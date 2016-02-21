int undo_tree_backval_type=0;
int undo_tree_examinedundostep=-1;
int undo_tree_backval_step_no=-1;
void undo_printslab(int x, int y,undo_undostep_ * iundostep,int itype,int imode)//type: 0:present 1: past 2: future 3:alternate branch
{
	int inumber;
	int toolnr;
	if(x<0) return;	
	if(y<0) return;	
	if(x+32>=gfx_screensizex) return;	
	if(y+32>=gfx_screensizey) return;	
	if (imode==1)
	{
		if ((control_mousex>=x) && (control_mousex<x+32) && (control_mousey>=y) && (control_mousey<y+32) && (itype!=3))
		{
			undo_tree_backval_type=itype;
			undo_tree_backval_step_no=undo_tree_examinedundostep;
		}
		return;
	}
	switch (itype)
	{
		case 0:
		sdl_buttondraw(x,y,17);break;
		case 1:
		sdl_buttondraw(x,y,14);break;
		case 2:
		sdl_buttondraw_coloring(x,y,14,0x100,0x10001);break;
		case 3:
		sdl_buttondraw_coloring(x,y,14,0x1,0x10100);break;

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
void draw_undotree(int imode)
{
	int verticalmiddle=gfx_screensizey/2;
	int ilv1=0;
	undo_tree_examinedundostep=currentundostep;
	iback:;
	ilv1++;
	if (undo_tree_examinedundostep==-1) goto loopend1;
	undo_printslab(100,verticalmiddle+ilv1*32,undosteps+undo_tree_examinedundostep,1,imode);
	undo_tree_examinedundostep=undosteps[undo_tree_examinedundostep].parent;
	goto iback;

	loopend1:;
	undo_tree_examinedundostep=currentundostep;
	iback2:;
	if (undo_tree_examinedundostep==-1) return;
	undo_undostep_ iundostep;
	strcpy(iundostep.commandname,undo_nextcommandname);
	undo_printslab(100,verticalmiddle,&iundostep,0,imode);
	ilv1=0;
	for (undo_tree_examinedundostep=0;undo_tree_examinedundostep<undosteps_count;undo_tree_examinedundostep++)
	{
		print("|,%i,c%i",undosteps[undo_tree_examinedundostep].parent,undo_tree_examinedundostep);
		if (undosteps[undo_tree_examinedundostep].parent==currentundostep)
		{
			ilv1++;
			undo_printslab(100+ilv1*32,verticalmiddle-32,undosteps+undo_tree_examinedundostep,2,imode);
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
			case SDL_MOUSEBUTTONDOWN:
			{
				undo_tree_backval_type=-1;
				draw_undotree(1);
				if (undo_tree_backval_type>0)
				{
					if (userwarning("Do you want to risk loosing data?\n The walkable Undo tree\nis highly experimental")>0)
					{
						currentundostep=undo_tree_backval_step_no;
						restoreundo(~0,0);
					}
					break;
				}
			}
			break;
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
