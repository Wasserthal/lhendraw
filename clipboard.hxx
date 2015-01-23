//X11 clipboard.
//This clipboard is used in linux systems.
//This unit was taken from third-party GPLed (GPLv2) code from xclip from Kim Saunders and Peter Åstrand
#define clipboard_XCLIB_XCOUT_NONE 0
#define clipboard_XCLIB_XCIN_NONE 0
#define clipboard_XCLIB_XCIN_INCR  2
#define clipboard_XCLIB_XCOUT_BAD_TARGET 3
#define clipboard_XCLIB_XCOUT_INCR 2
#define clipboard_XCLIB_XCOUT_SENTCONVSEL 1
int xcout(Display * dpy,Window win,XEvent evt,Atom sel,Atom target,Atom * type,unsigned char **txt,unsigned long *len,unsigned int *context)
{
    /* a property for other windows to put their selection into */
    static Atom pty;
    static Atom inc;
    int pty_format;

    /* buffer for XGetWindowProperty to dump data into */
    unsigned char *buffer;
    unsigned long pty_size, pty_items, pty_machsize;

    /* local buffer of text to return */
    unsigned char *ltxt = *txt;

    if (!pty)
    {
	pty = XInternAtom(dpy, "XCLIP_OUT", False);
    }

    if (!inc)
    {
	inc = XInternAtom(dpy, "INCR", False);
    }

    switch (*context)
    {
    case clipboard_XCLIB_XCOUT_NONE:
	if ((*txt)!=NULL)
	{
	    free(*txt);
	}
	*len = 0;
	XConvertSelection(dpy, sel, target, pty, win, CurrentTime);
	*context = clipboard_XCLIB_XCOUT_SENTCONVSEL;
	return (0);

    case clipboard_XCLIB_XCOUT_SENTCONVSEL:
	if (evt.type != SelectionNotify)
	    return (0);
	/* return failure when the current target failed */
	if (evt.xselection.property == None)
	{
	    *context = clipboard_XCLIB_XCOUT_BAD_TARGET;
	    return (0);
	}

	/* find the size and format of the data in property */
	XGetWindowProperty(dpy,
			   win,
			   pty,
			   0,
			   0,
			   False,
			   AnyPropertyType, type, &pty_format, &pty_items, &pty_size, &buffer);
	XFree(buffer);

	if (*type == inc)
	{
	    /* start INCR mechanism by deleting property */
	    XDeleteProperty(dpy, win, pty);
	    XFlush(dpy);
	    *context = clipboard_XCLIB_XCOUT_INCR;
	    return (0);
	}

	/* not using INCR mechanism, just read the property */
	XGetWindowProperty(dpy,
			   win,
			   pty,
			   0,
			   (long) pty_size,
			   False,
			   AnyPropertyType, type, &pty_format, &pty_items, &pty_size, &buffer);

	/* finished with property, delete it */
	XDeleteProperty(dpy, win, pty);

	/* compute the size of the data buffer we received */
	pty_machsize = pty_items * (pty_format/8);

	/* copy the buffer to the pointer for returned data */
	ltxt = (unsigned char *) malloc(pty_machsize);
	memcpy(ltxt, buffer, pty_machsize);

	/* set the length of the returned data */
	*len = pty_machsize;
	*txt = ltxt;

	/* free the buffer */
	XFree(buffer);

	*context = clipboard_XCLIB_XCOUT_NONE;

	/* complete contents of selection fetched, return 1 */
	return (1);

    case clipboard_XCLIB_XCOUT_INCR:
	/* To use the INCR method, we basically delete the
	 * property with the selection in it, wait for an
	 * event indicating that the property has been created,
	 * then read it, delete it, etc.
	 */

	/* make sure that the event is relevant */
	if (evt.type != PropertyNotify)
	    return (0);

	/* skip unless the property has a new value */
	if (evt.xproperty.state != PropertyNewValue)
	    return (0);

	/* check size and format of the property */
	XGetWindowProperty(dpy,
			   win,
			   pty,
			   0,
			   0,
			   False,
			   AnyPropertyType,
			   type, &pty_format, &pty_items, &pty_size, (unsigned char **) &buffer);

	if (pty_size == 0)
	{
	    /* no more data, exit from loop */
	    XFree(buffer);
	    XDeleteProperty(dpy, win, pty);
	    *context = clipboard_XCLIB_XCOUT_NONE;

	    /* this means that an INCR transfer is now
	     * complete, return 1
	     */
	    return (1);
	}

	XFree(buffer);

	/* if we have come this far, the propery contains
	 * text, we know the size.
	 */
	XGetWindowProperty(dpy,
			   win,
			   pty,
			   0,
			   (long) pty_size,
			   False,
			   AnyPropertyType,
			   type, &pty_format, &pty_items, &pty_size, (unsigned char **) &buffer);

	/* compute the size of the data buffer we received */
	pty_machsize = pty_items * (pty_format/8);

	/* allocate memory to accommodate data in *txt */
	if (*len == 0)
	{
	    *len = pty_machsize;
	    ltxt = (unsigned char *) malloc(*len);
	}
	else
	{
	    *len += pty_machsize;
	    ltxt = (unsigned char *) realloc(ltxt, *len);
	}

	/* add data to ltxt */
	memcpy(ltxt+(*len - pty_machsize),buffer,pty_machsize);

	*txt = ltxt;
	XFree(buffer);

	/* delete property to get the next item */
	XDeleteProperty(dpy, win, pty);
	XFlush(dpy);
	return (0);
    }
    return (0);
}
int xcin(Display * dpy,Window * win,XEvent evt,Atom * pty,Atom target,unsigned char *txt,unsigned long len,unsigned long *pos,unsigned int *context)
{
    unsigned long chunk_len;	/* length of current chunk (for incr
				 * transfers only)
				 */
    XEvent res;			/* response to event */
    static Atom inc;
    static Atom targets;
    static long chunk_size;

    if (!targets)
    {
	targets = XInternAtom(dpy, "TARGETS", False);
    }

    if (!inc)
    {
	inc = XInternAtom(dpy, "INCR", False);
    }
    /* We consider selections larger than a quarter of the maximum
       request size to be "large". See ICCCM section 2.5 */
    if (!chunk_size)
    {
	chunk_size = XExtendedMaxRequestSize(dpy) / 4;
	if (!chunk_size)
	{
	    chunk_size = XMaxRequestSize(dpy) / 4;
	}
    }
    switch (*context)
    {
    case clipboard_XCLIB_XCIN_NONE:
	if (evt.type != SelectionRequest)
	    return (0);
	/* set the window and property that is being used */
	*win = evt.xselectionrequest.requestor;
	*pty = evt.xselectionrequest.property;
	/* reset position to 0 */
	*pos = 0;
	/* put the data into an property */
	if (evt.xselectionrequest.target==targets)
	{
		Atom types[2]={targets,target};
		/* send data all at once (not using INCR) */
		XChangeProperty(dpy,
			*win,
			*pty,
			XA_ATOM,
			32,PropModeReplace,(unsigned char *) types,
			(int) (sizeof(types) / sizeof(Atom))
		);
	}
	else if (len > chunk_size)
	{
	    /* send INCR response */
	    XChangeProperty(dpy, *win, *pty, inc, 32, PropModeReplace, 0, 0);
	    /* With the INCR mechanism, we need to know
	     * when the requestor window changes (deletes)
	     * its properties
	     */
	    XSelectInput(dpy, *win, PropertyChangeMask);
	    *context = clipboard_XCLIB_XCIN_INCR;
	}
	else
	{
	    /* send data all at once (not using INCR) */
	    XChangeProperty(dpy,
			    *win,
			    *pty, target, 8, PropModeReplace, (unsigned char *) txt, (int) len);
	}

	/* Perhaps FIXME: According to ICCCM section 2.5, we should
	   confirm that XChangeProperty succeeded without any Alloc
	   errors before replying with SelectionNotify. However, doing
	   so would require an error handler which modifies a global
	   variable, plus doing XSync after each XChangeProperty. */
	/* set values for the response event */
	res.xselection.property = *pty;
	res.xselection.type = SelectionNotify;
	res.xselection.display = evt.xselectionrequest.display;
	res.xselection.requestor = *win;
	res.xselection.selection = evt.xselectionrequest.selection;
	res.xselection.target = evt.xselectionrequest.target;
	res.xselection.time = evt.xselectionrequest.time;
	/* send the response event */
	XSendEvent(dpy,evt.xselectionrequest.requestor,0,0,&res);
	XFlush(dpy);
	return (len <= chunk_size);
	break;
    case clipboard_XCLIB_XCIN_INCR:
	/* length of current chunk */

	/* ignore non-property events */
	if (evt.type != PropertyNotify)
	    return (0);

	/* ignore the event unless it's to report that the
	 * property has been deleted
	 */
	if (evt.xproperty.state != PropertyDelete)
	    return (0);

	/* set the chunk length to the maximum size */
	chunk_len = chunk_size;

	/* if a chunk length of maximum size would extend
	 * beyond the end ot txt, set the length to be the
	 * remaining length of txt
	 */
	if ((*pos + chunk_len) > len)
	    chunk_len = len - *pos;

	/* if the start of the chunk is beyond the end of txt,
	 * then we've already sent all the data, so set the
	 * length to be zero
	 */
	if (*pos > len)
	    chunk_len = 0;

	if (chunk_len)
	{
	    /* put the chunk into the property */
	    XChangeProperty(dpy,*win,*pty,target,8,PropModeReplace,&txt[*pos],(int) chunk_len);
	}
	else
	{
	    /* make an empty property to show we've
	     * finished the transfer
	     */
	    XChangeProperty(dpy, *win, *pty, target, 8, PropModeReplace, 0, 0);
	}
	XFlush(dpy);
	/* all data has been sent, break out of the loop */
	if (!chunk_len) *context = clipboard_XCLIB_XCIN_NONE;
	*pos += chunk_size;
	/* if chunk_len == 0, we just finished the transfer,
	 * return 1
	 */
	return (chunk_len <= 0);
	break;
    }
    return (0);
}
void service_clipboard()
{
	XEvent evt;
	#ifndef NOCLIPBOARD
	if (LHENDRAW_clipboardmode)
	{
		if (!XPending(gfx_Display))
		{
			return;
		}
		while (1)
		{
			static _u32 clear=0;
			static _u32 context=clipboard_XCLIB_XCIN_NONE;
			static long _u32 sel_pos=0;
			static Window gfx_Window;//Does this need to be a different variable?
			static Atom pty;
			int finished;
			XNextEvent(gfx_Display,&evt);
			finished=xcin(gfx_Display,&gfx_Window,evt,&pty,clipboard_target,(unsigned char*)LHENDRAW_clipboardbuffer,LHENDRAW_clipboardbuffer_count,&sel_pos,&context);
			if (evt.type==SelectionClear) clear=1;
			if ((context==clipboard_XCLIB_XCIN_NONE)&&clear) {LHENDRAW_clipboardmode=0;free(LHENDRAW_clipboardbuffer);LHENDRAW_clipboardbuffer=NULL;return;}
			if (finished) break;
		}
	}
	#endif
}
