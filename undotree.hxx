void undo_printslab(int x, int y,undo_undostep_ * iundostep,int type)//0:past 1: future 2:alternate branch 3: present
{
	int inumber;
	int toolnr;
	if(x<0) return;	
	if(y<0) return;	
	if(x+32>=gfx_screensizex) return;	
	if(y+32>=gfx_screensizey) return;	
	switch (type)
	{
		case 0:
		sdl_buttondraw(x,y,14);break;
		case 1:
		case 2:
		case 3:
		sdl_buttondraw(x,y,17);break;

	}
	inumber=0;
	if ((iundostep->commandname[0]>='0') && (iundostep->commandname[0]<='9'))
	{
		toolnr=atoi(iundostep->commandname);
		menugfx_menudescription[0]=0;
		for (int ilv1=0;ilv1<AUTOSTRUCT_PULLOUTLISTING_toolbox_Size;ilv1++)
		{
			if (AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].lmbmode==1)
			{
				if (AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].toolnr==toolnr)
				{
					inumber=AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].picno+1;
					goto ifound;
				}
			}
		}
	}
	printmenutext(x,y,iundostep->commandname,strlen(iundostep->commandname),0);
	return;
	ifound:;
	sdl_buttondraw(x,y,inumber);
}
void draw_undotree()	
{
	int verticalmiddle=gfx_screensizey/2;
	int ilv1=0;
	int current=currentundostep;
	iback:;
	ilv1++;
	if (current==-1) goto loopend1;
	undo_printslab(100,verticalmiddle+ilv1*32,undosteps+current,0);
	current=undosteps[current].parent;
	goto iback;

	loopend1:;
	current=currentundostep;
	iback2:;
	if (current==-1) return;
	undo_undostep_ iundostep;
	strcpy(iundostep.commandname,undo_nextcommandname);
	undo_printslab(100,verticalmiddle,&iundostep,3);
	ilv1=0;
	for (int ilv2=0;ilv2<undosteps_count;ilv2++)
	{
		print("|,%i,c%i",undosteps[ilv2].parent,current);
		if (undosteps[ilv2].parent==current)
		{
			ilv1++;
			undo_printslab(100+ilv1*32,verticalmiddle-32,undosteps+ilv2,1);
		}
	}
	print("\n");
}
void control_undotree()
{
	SDL_Event control_Event;
	while ( SDL_PollEvent(&control_Event) ) 
	{
		switch (control_Event.type) 
		{
			case SDL_MOUSEMOTION:
			{
				control_mousex=control_Event.motion.x;
				control_mousey=control_Event.motion.y;
			}
			break;
			case SDL_KEYDOWN:
			{
				LHENDRAW_filedlgmode=0;
			}
			break;
		}
	}
}
