//Turns document data into graphics
//Intended to be included several times for different graphics outputs


void MACRO_DRAWPREFIX(get_colorstring)(int number)
{
	#ifdef LENNARD_HACK
	if (LENNARD_HACK_colormode==1)
	{
		strcpy(colorstring,"FFFFFF");
		return;
	}
	if (LENNARD_HACK_colormode==2)
	{
		strcpy(colorstring,"000000");
		return;
	}
	#endif
	MACRO_DRAWPREFIX(get_colorstringv)(number);
}

inline void MACRO_DRAWPREFIX(get_colorstring_passive)(int number)
{
	if (number!=0)
	{
		MACRO_DRAWPREFIX(get_colorstringv)(number);
	}
}

inline int MACRO_DRAWPREFIX(formatfromifsmat)(int inifsmat)
{
	switch (inifsmat)
	{
		case 1: return 0x20;
		case 4: return 0x40;
	}
	return 0;
}



void MACRO_DRAWPREFIX(expresscdxcircle)(float ileft,float itop,float radius)
{
	MACRO_DRAWPREFIX(expressellipse)(ileft,itop,fabs(radius),fabs(radius));
}
#define dashdist 3
void MACRO_DRAWPREFIX(expresshashangle)(float langle,float cangle,float ix1,float iy1,float ix2,float iy2,float bonddist3,float bonddist4)
{
	float ilength=sqrt((ix2-ix1)*(ix2-ix1)+(iy2-iy1)*(iy2-iy1));
	for (float ilv1=0;ilv1<ilength;ilv1+=dashdist)
	{
		float share=(ilv1/ilength);
		float share2=share*bonddist3+(1-share)*bonddist4;
		MACRO_DRAWPREFIX(expressline)(ix1*(1-share)+ix2*share+cos(cangle)*share2,iy1*(1-share)+iy2*share+sin(langle)*share2,ix1*(1-share)+ix2*share-cos(cangle)*share2,iy1*(1-share)+iy2*share-sin(cangle)*share2);
	}
}
void MACRO_DRAWPREFIX(expresstriangle)(float ifx1,float ify1,float ifx2,float ify2,float ifx3,float ify3)
{
	LHENDRAW_inficorn[0].x=ifx1;
	LHENDRAW_inficorn[1].x=ifx2;
	LHENDRAW_inficorn[2].x=ifx3;
	LHENDRAW_inficorn[0].y=ify1;
	LHENDRAW_inficorn[1].y=ify2;
	LHENDRAW_inficorn[2].y=ify3;
	MACRO_DRAWPREFIX(expressinfinityangle)(3);
}

void MACRO_DRAWPREFIX(expresstetrangle)(float ifx1,float ify1,float ifx2,float ify2,float ifx3,float ify3,float ifx4,float ify4)
{
	LHENDRAW_inficorn[0].x=ifx1;
	LHENDRAW_inficorn[1].x=ifx2;
	LHENDRAW_inficorn[2].x=ifx3;
	LHENDRAW_inficorn[3].x=ifx4;
	LHENDRAW_inficorn[0].y=ify1;
	LHENDRAW_inficorn[1].y=ify2;
	LHENDRAW_inficorn[2].y=ify3;
	LHENDRAW_inficorn[3].y=ify4;
	MACRO_DRAWPREFIX(expressinfinityangle)(4);
}
void MACRO_DRAWPREFIX(expresshexangle)(float ifx1,float ify1,float ifx2,float ify2,float ifx3,float ify3,float ifx4,float ify4,float ifx5,float ify5,float ifx6,float ify6)
{
	LHENDRAW_inficorn[0].x=ifx1;
	LHENDRAW_inficorn[1].x=ifx2;
	LHENDRAW_inficorn[2].x=ifx3;
	LHENDRAW_inficorn[3].x=ifx4;
	LHENDRAW_inficorn[4].x=ifx5;
	LHENDRAW_inficorn[5].x=ifx6;
	LHENDRAW_inficorn[0].y=ify1;
	LHENDRAW_inficorn[1].y=ify2;
	LHENDRAW_inficorn[2].y=ify3;
	LHENDRAW_inficorn[3].y=ify4;
	LHENDRAW_inficorn[4].y=ify5;
	LHENDRAW_inficorn[5].y=ify6;
	MACRO_DRAWPREFIX(expressinfinityangle)(6);
}
void MACRO_DRAWPREFIX(drawglyph)(int unicode,int imode,int ideltax,int ideltay,int * i_txcursorx,int * i_txcursory,float angle,float size)
{
	int ino=0;
	if (unicode==32) {(*i_txcursorx)+=1230;return;}
	if (unicode==10) {(*i_txcursorx)=0;(*i_txcursory)+=2667;return;}
	if ((unicode==0xE000) || (unicode==0xE001))
	{
		int dummy_txcursorx=(*i_txcursorx)-615;
		if (gfx_txselectmode & 1)
		{
			gfx_txselectmode^=2;
		}
		else
		{
			MACRO_DRAWPREFIX(drawglyph)('|',imode,ideltax,ideltay,&dummy_txcursorx,i_txcursory,angle,size);
		}
		return;
	}
	if (unicode<0) {ino=-unicode;goto ifertig;}
	for (int ilv1=0;ilv1<glyphmemory_max;ilv1++)
	{
		if (glyphmemory[ilv1].unicode==unicode)
		{
			ino=ilv1;
			goto ifertig;
		}
	}
	ifertig:;
	if (glyphmemory[ino].units==-1)
	{
		for (int ilv2=0;ilv2<glyphmemory[ino].composite.count;ilv2++)
		{
			int deltax=*i_txcursorx;int deltay=*i_txcursory;
			if (glyphmemory[ino].composite.unit[ilv2].flags & 2)
			{
				deltax+=glyphmemory[ino].composite.unit[ilv2].arg1;
				deltay+=glyphmemory[ino].composite.unit[ilv2].arg2;
			}
			MACRO_DRAWPREFIX(drawglyph)(-glyphmemory[ino].composite.unit[ilv2].index+3,imode,ideltax,ideltay,&deltax,&deltay,angle,size);
		}
		(*i_txcursorx)+=glyphmemory[ino].offsetx;
	}
	int tl_left,tl_right,tl_top,tl_bottom;
	glyf_modglyph(ino,imode|0x80*((gfx_txselectmode&2)>>1),ideltax,ideltay,i_txcursorx,i_txcursory,size*cos(angle),size*sin(angle),-size*sin(angle),size*cos(angle),&tl_left,&tl_right,&tl_top,&tl_bottom);
	if (MACRO_DRAWPREFIX(expressgeometry_start)(tl_left-1,tl_top-1,tl_right+1,tl_bottom+1))
	{
		MACRO_DRAWPREFIX(expressgeometry_begin)(glyf_processed[0].x,glyf_processed[0].y);
		for (int ilv2=1;ilv2<glyf_processedcount;ilv2++)
		{
			char tl_postincrement=0;
			if ((glyf_processed[ilv2].flags & 1)==0)
			{
				MACRO_DRAWPREFIX(expressgeometry_line)(glyf_processed[ilv2].x,glyf_processed[ilv2].y);
			}
			else
			{
				float nextx,nexty;
				if ((glyf_processed[ilv2].flags & 2)==0)
				{
					if ((glyf_processed[ilv2+1].flags & 1)==0)
					{
						nextx=glyf_processed[ilv2+1].x;
						nexty=glyf_processed[ilv2+1].y;
						tl_postincrement=1;
					}
					else
					{
						nextx=(glyf_processed[ilv2].x+glyf_processed[ilv2+1].x)*0.5;
						nexty=(glyf_processed[ilv2].y+glyf_processed[ilv2+1].y)*0.5;
					}
				}
				else
				{
					MACRO_DRAWPREFIX(expressgeometry_bezier2)(glyf_processed[ilv2].x,glyf_processed[ilv2].y,gfx_geometry.startx,gfx_geometry.starty);
					goto icircle;
				}
				MACRO_DRAWPREFIX(expressgeometry_bezier2)(glyf_processed[ilv2].x,glyf_processed[ilv2].y,nextx,nexty);
				if (tl_postincrement) ilv2++;
			}
			if (glyf_processed[ilv2].flags & 2)
			{
				MACRO_DRAWPREFIX(expressgeometry_backline)();
				icircle:;
				ilv2++;
				MACRO_DRAWPREFIX(expressgeometry_begin)(glyf_processed[ilv2].x,glyf_processed[ilv2].y);
			}
		}
		MACRO_DRAWPREFIX(expressgeometry_end)();
	}
}
void MACRO_DRAWPREFIX(draw_printformatted)(const char * iinput,const char * parms,int imode,int start,int end)
{
	if (SDL_text_fallback==0)
	{
		MACRO_DRAWPREFIX(printformatted)(iinput,parms,imode,start,end);
	}
	else
	{
		int backcount=0;
		for (int ilv4=start;ilv4<end;ilv4+=backcount)
		{
			int ino=utf8resolve((unsigned char*)iinput + ilv4,&backcount);
			gfx_drawglyph(ino,imode,SDL_glyfstartx,SDL_glyfstarty,&SDL_txcursorx,&SDL_txcursory,SDL_txangle,0.006);
		}
	}
}

void MACRO_DRAWPREFIX(drawarrheads)(cdx_Rectangle iBBX,float langle,float cangle,float otherlangle,float othercangle,int currentArrowHeadType,int currentArrowHeadTail,int currentArrowHeadHead,float tllinedist)
{
	for (int tlarrowside=-1;tlarrowside<2;tlarrowside+=2)
	{
		int tlarrowHead=(tlarrowside==-1) ? currentArrowHeadTail : currentArrowHeadHead;
		float tlArrowTopx=(tlarrowside==-1) ? iBBX.right : iBBX.left;
		float tlArrowTopy=(tlarrowside==-1) ? iBBX.bottom : iBBX.top;
		float tlcangle,tllangle;
		tllangle=(tlarrowside==-1) ? otherlangle : langle;
		tlcangle=(tlarrowside==-1) ? othercangle : cangle;
		switch (tlarrowHead)
		{
			case 1 :
			{
				if (currentArrowHeadType==2)
				{
					MACRO_DRAWPREFIX(expressline)(tlArrowTopx+tllinedist*cos(tlcangle),tlArrowTopy+tllinedist*sin(tlcangle),tlArrowTopx-tllinedist*cos(tlcangle),tlArrowTopy-tllinedist*sin(tlcangle));
				}
				break;
			}
			case 2 :
			{
				if (currentArrowHeadType==1)
				{
					MACRO_DRAWPREFIX(stylegenestring)(2);
					MACRO_DRAWPREFIX(expresstriangle)(tlArrowTopx,tlArrowTopy,
tlArrowTopx+cos(tllangle)*arrowheadlength+cos(tlcangle)*arrowthickness,tlArrowTopy+sin(tllangle)*arrowheadlength+sin(tlcangle)*arrowthickness,
tlArrowTopx+cos(tllangle)*arrowheadlength-cos(tlcangle)*arrowthickness,tlArrowTopy+sin(tllangle)*arrowheadlength-sin(tlcangle)*arrowthickness
);
				}
				if ((currentArrowHeadType==3) || (currentArrowHeadType==2))
				{
					MACRO_DRAWPREFIX(expressline)(tlArrowTopx,tlArrowTopy,
tlArrowTopx+cos(tllangle)*tllinedist*2+cos(tlcangle)*tllinedist*2,tlArrowTopy+sin(tllangle)*tllinedist*2+sin(tlcangle)*tllinedist*2);
					MACRO_DRAWPREFIX(expressline)(tlArrowTopx,tlArrowTopy,
tlArrowTopx+cos(tllangle)*tllinedist*2-cos(tlcangle)*tllinedist*2,tlArrowTopy+sin(tllangle)*tllinedist*2-sin(tlcangle)*tllinedist*2);

				}
				if (currentArrowHeadType==2)
				{
					MACRO_DRAWPREFIX(expressline)(tlArrowTopx+cos(tllangle)*tllinedist*2+cos(tlcangle)*tllinedist,tlArrowTopy+sin(tllangle)*tllinedist*2+sin(tlcangle)*tllinedist,
tlArrowTopx+cos(tllangle)*tllinedist*2+cos(tlcangle)*tllinedist*2,tlArrowTopy+sin(tllangle)*tllinedist*2+sin(tlcangle)*tllinedist*2);
					MACRO_DRAWPREFIX(expressline)(tlArrowTopx+cos(tllangle)*tllinedist*2-cos(tlcangle)*tllinedist,tlArrowTopy+sin(tllangle)*tllinedist*2-sin(tlcangle)*tllinedist,
tlArrowTopx+cos(tllangle)*tllinedist*2-cos(tlcangle)*tllinedist*2,tlArrowTopy+sin(tllangle)*tllinedist*2-sin(tlcangle)*tllinedist*2);
				}
				break;
			}
			case 4 :
			{
				tlcangle=tlcangle+Pi;
			}
			case 3 :
			{
				MACRO_DRAWPREFIX(stylegenestring)(2);
				if (currentArrowHeadType==1)
				{
					MACRO_DRAWPREFIX(expresstriangle)(tlArrowTopx+tllinedist*cos(tlcangle),tlArrowTopy+tllinedist*sin(tlcangle),
tlArrowTopx+cos(tllangle)*arrowheadlength+(arrowthickness+tllinedist)*cos(tlcangle),tlArrowTopy+sin(tllangle)*arrowheadlength+(arrowthickness+tllinedist)*sin(tlcangle),
tlArrowTopx+cos(tllangle)*arrowheadlength+tllinedist*cos(tlcangle),tlArrowTopy+sin(tllangle)*arrowheadlength+tllinedist*sin(tlcangle));
				}
				break;
			}
		}
	}
}


#ifdef LENNARD_HACK
void LENNARD_HACK_killtext()
{
	int tl_t_number=-1;
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].instance==glob_t_multilist)
		{
			tl_t_number=ilv1;
		}
	}
	for (int ilv1=0;ilv1<(*glob_t_multilist).filllevel;ilv1++)
	{
		if (text_actual_node[ilv1].owner==-1)
		{
			deletefromZlist(tl_t_number,ilv1);
		}
	}
}
#endif

void MACRO_DRAWPREFIX(draw_Symbol)(int SymbolType)
{
	float tl_angle;
	MACRO_DRAWPREFIX(stylegenestring)(1);
	switch(SymbolType)
	{
		case 2 :
		case 3 : iBBX.left-=5;
		case 4 :
		case 5 :
		case 8 :
		case 9 : strcpy(colorstring2,"FFFFFF");goto charge;break;
		case 1 : strcpy(colorstring2,colorstring);goto radical;break;
		case 0 : strcpy(colorstring2,colorstring);goto lonepair;break;
		case 6 : strcpy(colorstring2,colorstring);goto dagger;break;
		case 7 : strcpy(colorstring2,colorstring);goto dagger2;break;
		default:
		return;
	}
	charge:
	MACRO_DRAWPREFIX(stylegenestring)(1,0xFFFFFFFF);
	if ((SymbolType==4) || (SymbolType==5))
	{
		MACRO_DRAWPREFIX(expressellipse)(iBBX.left,iBBX.top,6,6);
	}
	MACRO_DRAWPREFIX(expressline)(iBBX.left-3,iBBX.top,iBBX.left+3,iBBX.top);
	if ((SymbolType==2) || (SymbolType==4) || (SymbolType==8))
	{
		MACRO_DRAWPREFIX(expressline)(iBBX.left,iBBX.top-3,iBBX.left,iBBX.top+3);
	}
	if ((SymbolType==2) || (SymbolType==3))
	{
		iBBX.left+=10;
		goto radical;
	}
	return;
	radical:
	MACRO_DRAWPREFIX(stylegenestring)(3);
	MACRO_DRAWPREFIX(expressellipse)(iBBX.left,iBBX.top,4,4);
	return;
	lonepair:
	MACRO_DRAWPREFIX(stylegenestring)(3);
	tl_angle=getangle(iBBX.left-iBBX.right,iBBX.top-iBBX.bottom);
	MACRO_DRAWPREFIX(expressellipse)(iBBX.left+sin(tl_angle)*5,iBBX.top-cos(tl_angle)*5,4,4);
	MACRO_DRAWPREFIX(expressellipse)(iBBX.left-sin(tl_angle)*5,iBBX.top+cos(tl_angle)*5,4,4);
	return;
	dagger2:
	MACRO_DRAWPREFIX(expressline)(iBBX.left-5,iBBX.top+10,iBBX.right+5,iBBX.top+10);
	//PASSTHROUGH
	dagger:
	MACRO_DRAWPREFIX(expressline)(iBBX.left-5,iBBX.top,iBBX.right+5,iBBX.top);
	MACRO_DRAWPREFIX(expressline)(iBBX.left,iBBX.top-5,iBBX.right,iBBX.top+15);
}
void MACRO_DRAWPREFIX(controlprocedure)(bool irestriction,char hatches)
{
	int ilv3,ilv4;
	void * dummy;
	cdx_Rectangle tlBoundingBox;
	int tlGraphicType;
	float tlAngularSize;
	int ipropertyoffset;
	void * tlcurrentinstance;
	basicmultilist * tlcurrentmultilist;
	char sortback;
	char tl_connectmode,tl_connect;
	int tl_bracketlevel;
	int tl_posbeforebracket=0;
	int tl_pos2beforebracket=0;
	int tl_backvalbeforebracket=0;
	char tl_bracketmode=0;
	int tl_sort_bracketlevel;
	int tl_posbeforesort_bracket=0;
	int tl_pos2beforesort_bracket=0;
	int tl_backvalbeforesort_bracket=0;
	char tl_sort_bracketmode=0;
	#ifdef LENNARD_HACK
	char LENNARD_HACK_REPEATHOOK=0;
	color_instance* tlcolorr;
	float LENNARD_HACK_svgcurrentshiftx;
	float LENNARD_HACK_svgcurrentshifty;
	#endif
	for (ilv1=0;ilv1<objectZorderlist_count;ilv1++)
	{
		if (objectZorderlist[ilv1].listnr!=-1)
		{
			if (control_save_selection)
			{
				if ((selection_currentselection[objectZorderlist[ilv1].nr] & (1<<(*(multilistlist[objectZorderlist[ilv1].listnr].instance)).numberinlist))==0)
				{
					goto svg_main_loop;
				}
			}
			tlcurrentmultilist=multilistlist[objectZorderlist[ilv1].listnr].instance;
			index_in_buffer=objectZorderlist[ilv1].nr;
			tlcurrentinstance=((char*)((*tlcurrentmultilist).pointer))+((*tlcurrentmultilist).itemsize)*index_in_buffer;
			if ((*(basic_instance*)tlcurrentinstance).exist==0){goto svg_main_loop;}
			{//TODO**** delete instance in findaround after implementing delete routines
				ipropertyoffset=(tlcurrentmultilist)->getproperties("SupersededBy",(CDXMLREAD_functype*)&dummy);
				if (ipropertyoffset!=-1)
				{
					if ((*(int*)(((char*)tlcurrentinstance)+ipropertyoffset))>0)
					{
						goto svg_main_loop;
					}
				}
			}
			if (((tlcurrentmultilist==glob_moleculefill_multilist) && (hatches==0)) || ((tlcurrentmultilist!=glob_moleculefill_multilist) && (hatches==1)))
			{
				goto svg_main_loop;
			}
			if (irestriction)
			{
				if (tlcurrentmultilist==glob_curve_multilist)
				{
					cdx_Bezierpoints * tlBez=&((*glob_curve_multilist)[index_in_buffer].CurvePoints);
					for (int ilv2=1;ilv2<(*tlBez).count-1;ilv2+=3)
					{
						cdx_Point2D tlp2d=(*tlBez).a[ilv2];
						if (tlp2d.x<SVG_currentbasex) {goto svg_main_loop;}
						if (tlp2d.y<SVG_currentbasey) {goto svg_main_loop;}
						if (tlp2d.x>SVG_currentfringex) {goto svg_main_loop;}
						if (tlp2d.y>SVG_currentfringey) {goto svg_main_loop;}
					}
				}
				ipropertyoffset=(tlcurrentmultilist)->getproperties("BoundingBox",(CDXMLREAD_functype*)&dummy);
				if (ipropertyoffset!=-1)
				{
					tlBoundingBox=*((cdx_Rectangle*)(((char*)tlcurrentinstance)+ipropertyoffset));
					canonicalizeboundingbox(&tlBoundingBox);
					if (tlBoundingBox.left<SVG_currentbasex) {goto svg_main_loop;}
					if (tlBoundingBox.top<SVG_currentbasey) {goto svg_main_loop;}
					if (tlBoundingBox.left>SVG_currentfringex) {goto svg_main_loop;}
					if (tlBoundingBox.top>SVG_currentfringey) {goto svg_main_loop;}
				}
				else
				{
					if (tlcurrentmultilist==glob_b_multilist)
					{
						cdx_Point3D tlp3d=(*glob_n_multilist)[bond_actual_node[index_in_buffer].start].xyz;
						if (tlp3d.x<SVG_currentbasex) {goto svg_main_loop;}
						if (tlp3d.y<SVG_currentbasey) {goto svg_main_loop;}
						if (tlp3d.x>SVG_currentfringex) {goto svg_main_loop;}
						if (tlp3d.y>SVG_currentfringey) {goto svg_main_loop;}
					}
				}
			}
			#ifdef LENNARD_HACK
			ipropertyoffset=(tlcurrentmultilist)->getproperties("color",(CDXMLREAD_functype*)&dummy);
			if (ipropertyoffset!=-1)
			{
				tlcolorr=get_color((*(int*)(((char*)tlcurrentinstance)+ipropertyoffset)));
				if (tlcolorr!=NULL)
				{
					if ((fabs((*tlcolorr).r-0.1961)<0.001) && (fabs((*tlcolorr).g)<0.001) && (fabs((*tlcolorr).b-0.1961)<0.001))
					{
						LENNARD_HACK_colormode=1;
						for (ilv3=-3;ilv3<4;ilv3++)
						{
							for (ilv4=-3;ilv4<4;ilv4++)
							{
								if ((abs(ilv3)+abs(ilv4)<3) || ((ilv3==3) && (ilv4==3)))
								{
									LENNARD_HACK_colormode=1;
									LENNARD_HACK_svgcurrentshiftx=SVG_currentshiftx;
									LENNARD_HACK_svgcurrentshifty=SVG_currentshifty;
									if ((ilv3==3) && (ilv4==3))
									{
										LENNARD_HACK_colormode=2;
									}
									else
									{
										SVG_currentshiftx+=ilv4;
										SVG_currentshifty+=ilv3;
									}
									LENNARD_HACK_REPEATHOOK=1;
									if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_curve_multilist) goto svg_main_curve;
									if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_graphic_multilist) goto svg_main_graphic;
									if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_b_multilist) goto svg_main_b;
									if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_t_multilist) goto svg_main_t;
									if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_arrow_multilist) goto svg_main_arrow;
									if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_moleculefill_multilist) goto svg_main_moleculefill;
									LENNARD_HACK_REPEAT:
									LENNARD_HACK_REPEATHOOK=0;
									SVG_currentshiftx=LENNARD_HACK_svgcurrentshiftx;
									SVG_currentshifty=LENNARD_HACK_svgcurrentshifty;
								}
							}
						}
						LENNARD_HACK_colormode=0;
						goto svg_main_loop;
					}
				}
			}
			#endif
			if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_n_multilist) goto svg_main_n;
			if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_curve_multilist) goto svg_main_curve;
			if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_graphic_multilist) goto svg_main_graphic;
			if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_b_multilist) goto svg_main_b;
			if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_t_multilist) goto svg_main_t;
			if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_arrow_multilist) goto svg_main_arrow;
			if (multilistlist[objectZorderlist[ilv1].listnr].instance==glob_moleculefill_multilist) goto svg_main_moleculefill;
		}
		svg_main_loop:
		;
		if (hatches==2)
		{
			if (edit_checkclickpixels()) {selection_clickselection_found|=(selection_clickselection[index_in_buffer]|=(1<<(*(multilistlist[objectZorderlist[ilv1].listnr].instance)).numberinlist));}
		}
		#ifdef LENNARD_HACK
		if (LENNARD_HACK_REPEATHOOK) goto LENNARD_HACK_REPEAT;
		#endif
	}
	goto svg_main_end;
	svg_main_n:
	{
		i_n_instance=(n_instance*)&((*glob_n_multilist)[index_in_buffer]);
		int tlElement=((*i_n_instance).Element);
		MACRO_DRAWPREFIX(stylegenestring)(3);
		colornr=(*i_n_instance).color;
		MACRO_DRAWPREFIX(get_colorstring)(colornr);
		TELESCOPE_aggressobject(glob_n_multilist,index_in_buffer);
		int tlbackval;
		tlbackval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_Symbol);
		n_Symbol_loop:;
		if (tlbackval)
		{
			Symbol_instance * tl_Symbol_instance=(Symbol_instance*)TELESCOPE_getproperty();
			iBBX.left=i_n_instance->xyz.x+tl_Symbol_instance->dxyz.x;
			iBBX.top=i_n_instance->xyz.y+tl_Symbol_instance->dxyz.y;
			iBBX.right=i_n_instance->xyz.x;
			iBBX.bottom=i_n_instance->xyz.y;
			MACRO_DRAWPREFIX(get_colorstring)((*tl_Symbol_instance).color);
			MACRO_DRAWPREFIX(draw_Symbol)(tl_Symbol_instance->SymbolType);
			tlbackval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_Symbol);
			goto n_Symbol_loop;
		}
		MACRO_DRAWPREFIX(get_colorstring)(colornr);
		if (tlElement==-1)
		{
			sortback=atom_actual_node[index_in_buffer].labelside;
			if ((control_mousestate & 0x40) && (control_textedit_index==index_in_buffer)) sortback=0;
			gfx_txselectmode=(((control_mousestate & 0x40)>0) && (control_textedit_selectmode==1) && (control_textedit_type==STRUCTURE_OBJECTTYPE_n) && (control_textedit_index==index_in_buffer));
			MACRO_DRAWPREFIX(express_txinit)(2 | sortback,((*glob_n_multilist))[index_in_buffer].xyz.x,((*glob_n_multilist))[index_in_buffer].xyz.y,atomfontheight,0);
			TELESCOPE_aggressobject(glob_n_multilist,index_in_buffer);
			goto n_to_t_shunt;
		}
		if (tlElement==constants_Element_implicitcarbon)
		{
			#ifndef DEBUG
			if (atom_actual_node[index_in_buffer].bondcount==2)
			{
				if ((*glob_b_multilist)[atom_actual_node[index_in_buffer].bonds[0]].Order==(*glob_b_multilist)[atom_actual_node[index_in_buffer].bonds[1]].Order)
				{
					n_instance * aux_n_instance1=(*glob_n_multilist).bufferlist()+getother(index_in_buffer,atom_actual_node[index_in_buffer].bonds[0]);
					n_instance * aux_n_instance2=(*glob_n_multilist).bufferlist()+getother(index_in_buffer,atom_actual_node[index_in_buffer].bonds[1]);
					float tl_auxangle=getangle(aux_n_instance2->xyz.x-i_n_instance->xyz.x,aux_n_instance2->xyz.y-i_n_instance->xyz.y)-
					getangle(i_n_instance->xyz.x-aux_n_instance1->xyz.x,i_n_instance->xyz.y-aux_n_instance1->xyz.y);
					while (tl_auxangle<=-Pi) tl_auxangle+=2*Pi;
					while (tl_auxangle>=Pi) tl_auxangle-=2*Pi;
					if (fabs(tl_auxangle)<0.1)
					{
			#endif
						MACRO_DRAWPREFIX(expresscdxcircle)((*i_n_instance).xyz.x,(*i_n_instance).xyz.y,2);
			#ifndef DEBUG
					}
				}
			}
			#endif
		}
		else
		{
			char istring[1000];
			_u8 tl_format=0;
			edit_formatstruct * tl_formatpointer;
			edit_formatstruct last;
			_u8 imatch=1;
			int inr=0;
			int tl_fill;
			int i_bond_sum;
			i_bond_sum=getbondsum(index_in_buffer);
			last.color=(*i_n_instance).color;
			TELESCOPE_aggressobject(glob_n_multilist,index_in_buffer);
			tlbackval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s_f);
			if (tlbackval)
			{
				tl_format=(*((s_f_instance*)TELESCOPE_getproperty())).valids;
				tl_formatpointer=(edit_formatstruct*)TELESCOPE_getproperty_contents();
			}
			else
			{
				tl_format=0;
			}
			MACRO_DRAWPREFIX(express_txinit)(2 | atom_actual_node[index_in_buffer].labelside,(*i_n_instance).xyz.x,(*i_n_instance).xyz.y,atomfontheight,0);
			for (int ilv1=0;ilv1<6;ilv1++)
			{
				int actual=ilv1;
				if (atom_actual_node[index_in_buffer].labelside==1)
				{
					if (ilv1==3) actual=4;
					if (ilv1==4) actual=3;
				}
				tl_fill=0;
				for (int ilv2=0;ilv2<=actual;ilv2++)
				{
					imatch=1<<ilv2;
					if (tl_format & imatch)
					{
						last.color=tl_formatpointer[tl_fill].color;
						MACRO_DRAWPREFIX(get_colorstring)(last.color);
						tl_fill++;
					}
				}
				switch (actual)
				{
					case 0 : sprintf(istring,"%c",element[tlElement].name[0]);break;
					case 1 : sprintf(istring,"%c",element[tlElement].name[1]);break;
					case 2 : sprintf(istring,"%c",element[tlElement].name[2]);break;
					case 3 : sprintf(istring,"%s",((*i_n_instance).protons-i_bond_sum<=0)?"":"H");break;
					case 4 : if ((*i_n_instance).protons-i_bond_sum>1) sprintf(istring,"%i",(*i_n_instance).protons-i_bond_sum); else istring[0]=0;break;
					case 5 :
/*					if ((*i_n_instance).charge<0) {sprintf(istring,"%i-",-(*i_n_instance).charge);break;}
					if ((*i_n_instance).charge>0) {sprintf(istring,"%i+",(*i_n_instance).charge);break;}Charges are drawn separately with icons. TODO: add these icons if missing
					if ((*i_n_instance).charge==0) {istring[0]=0;break;}*///
					istring[0]=0;break;
				}
				if ((atom_actual_node[index_in_buffer].labelside==1) && (actual>=3))
				{
					MACRO_DRAWPREFIX(text_rewind)((unsigned char*)istring,strlen(istring));
				}
				MACRO_DRAWPREFIX(draw_printformatted)(istring,"",(0x20*(actual==4)) | (0x40*(ilv1==5)),0,strlen(istring));
			}
			MACRO_DRAWPREFIX(express_text_tail)();
		}
	}
	goto svg_main_loop;
	svg_main_curve:
	tllinedist=0;//TODO****
	i_curve_instance=(curve_instance*)&((*glob_curve_multilist)[index_in_buffer]);
	colornr=(*i_curve_instance).color;
	MACRO_DRAWPREFIX(get_colorstring)(colornr);
	MACRO_DRAWPREFIX(stylegenestring)(stylefromattrs((*i_curve_instance).LineType,(*i_curve_instance).FillType));
	tllast=1;
	{
	float tl_minx=1000000000000; float tl_miny=1000000000000; float tl_maxx=-1000000000000; float tl_maxy=-1000000000000;
	for (int ilv1=0;ilv1<(*i_curve_instance).CurvePoints.count;ilv1++)
	{
		if ((*i_curve_instance).CurvePoints.a[ilv1].x<tl_minx) tl_minx=(*i_curve_instance).CurvePoints.a[ilv1].x;
		if ((*i_curve_instance).CurvePoints.a[ilv1].x>tl_maxx) tl_maxx=(*i_curve_instance).CurvePoints.a[ilv1].x;
		if ((*i_curve_instance).CurvePoints.a[ilv1].y<tl_miny) tl_miny=(*i_curve_instance).CurvePoints.a[ilv1].y;
		if ((*i_curve_instance).CurvePoints.a[ilv1].y>tl_maxy) tl_maxy=(*i_curve_instance).CurvePoints.a[ilv1].y;
	}
	if (MACRO_DRAWPREFIX(expressgeometry_start)(tl_minx,tl_miny,tl_maxx,tl_maxy))
	{
		MACRO_DRAWPREFIX(expressgeometry_begin)((*i_curve_instance).CurvePoints.a[1].x,(*i_curve_instance).CurvePoints.a[1].y);
		for (int ilv1=2;ilv1<(*i_curve_instance).CurvePoints.count-2;ilv1+=3)
		{
			MACRO_DRAWPREFIX(expressgeometry_bezier3)((*i_curve_instance).CurvePoints.a[ilv1].x,(*i_curve_instance).CurvePoints.a[ilv1].y,(*i_curve_instance).CurvePoints.a[ilv1+1].x,(*i_curve_instance).CurvePoints.a[ilv1+1].y,(*i_curve_instance).CurvePoints.a[ilv1+2].x,(*i_curve_instance).CurvePoints.a[ilv1+2].y);
		}
		if ((*i_curve_instance).Closed)
		{
			MACRO_DRAWPREFIX(expressgeometry_bezier3)((*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-1].x,(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-1].y,(*i_curve_instance).CurvePoints.a[0].x,(*i_curve_instance).CurvePoints.a[0].y,(*i_curve_instance).CurvePoints.a[1].x,(*i_curve_instance).CurvePoints.a[1].y);
		}
		else
		{
			if ((*i_curve_instance).FillType>0)
			{
				MACRO_DRAWPREFIX(expressgeometry_backline)();
			}
		}
		MACRO_DRAWPREFIX(expressgeometry_end)();
	}
	}
	if ((*i_curve_instance).Closed==0)
	{
		currentArrowHeadType=1;
		currentArrowHeadTail=1;
		currentArrowHeadHead=1;
		tllinedist=0;
		tllefttan=0;
		tlrighttan=0;
		tllefttan2=0;
		tlrighttan2=0;
		langle=0;cangle=0;
		currentLineType=0;//0: normal 2: Bold 0x100: Double
		currentArrowHeadType=(*i_curve_instance).ArrowheadType;
		currentArrowHeadHead=(*i_curve_instance).ArrowheadHead;
		currentArrowHeadTail=(*i_curve_instance).ArrowheadTail;
		iBBX.left=(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].x;
		iBBX.top=(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].y;
		iBBX.right=(*i_curve_instance).CurvePoints.a[1].x;
		iBBX.bottom=(*i_curve_instance).CurvePoints.a[1].y;

		langle=getangle((*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-3].x-(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].x,(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-3].y-(*i_curve_instance).CurvePoints.a[(*i_curve_instance).CurvePoints.count-2].y);
		otherlangle=getangle((*i_curve_instance).CurvePoints.a[2].x-(*i_curve_instance).CurvePoints.a[1].x,(*i_curve_instance).CurvePoints.a[2].y-(*i_curve_instance).CurvePoints.a[1].y);
		cangle=langle+Pi/2;
		othercangle=otherlangle+Pi/2;
		MACRO_DRAWPREFIX(drawarrheads)(iBBX,langle,cangle,otherlangle,othercangle,currentArrowHeadType,currentArrowHeadTail,currentArrowHeadHead,tllinedist);
	}
	goto svg_main_loop;
	svg_main_graphic:
/* HOW ARROW MUST BE DONE:
1. The arrow type determiner determines arrow type and bond spacing.
2. The lines/arc are drawn. if it was something else, the next two points are skipped. 
3. Otherwise, the arrow angles are calculated
4. And the arrows are drawn.*/
	i_graphic_instance=&((*glob_graphic_multilist))[index_in_buffer];
	iBBX=(*i_graphic_instance).BoundingBox;
	colornr=(*i_graphic_instance).color;
	tlGraphicType=(*i_graphic_instance).GraphicType;
//	tlAngularSize=(*i_graphic_instance).AngularSize;//TODO: Make sure no graphics are created from something with an angular size
	currentLineType=(*i_graphic_instance).LineType;
	MACRO_DRAWPREFIX(get_colorstring)(colornr);
	ellipsoid.reset();
	currentArrowHeadType=1;
	currentArrowHeadTail=1;
	currentArrowHeadHead=1;
	tllinedist=0;
	tllefttan=0;
	tlrighttan=0;
	tllefttan2=0;
	tlrighttan2=0;
	langle=0;cangle=0;
	currentLineType=0;//0: normal 2: Bold 0x100: Double
	currentEllipsemode=0;
/*	if ((*i_graphic_instance).ArrowType & 1)
	{
		currentArrowHeadHead=2;
	}
	if ((*i_graphic_instance).ArrowType & 2)
	{
		currentArrowHeadHead=1;
	}
	if ((*i_graphic_instance).ArrowType & 4)
	{
		currentArrowHeadHead=1;
		currentArrowHeadTail=1;
	}
	if ((*i_graphic_instance).ArrowType & 8)
	{
		currentLineType=0x100;
		tllinedist=4;
		currentArrowHeadHead=2;
		currentArrowHeadTail=2;
	}
	if ((*i_graphic_instance).ArrowType & 48)
	{
		tllinedist=8;
		currentArrowHeadHead=1;//only valid when called from graphic
		if ((*i_graphic_instance).ArrowType & 16) {currentArrowHeadType=1;}
		if ((*i_graphic_instance).ArrowType & 32) {currentArrowHeadType=2;}
		currentLineType=0x100;
	}*/
	svg_graphic_arrow_shunt:
	if (tlGraphicType==1)
	{
		MACRO_DRAWPREFIX(stylegenestring)(stylefromline(currentLineType));
		cangle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top)+Pi/2;
		langle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top);
		if (currentLineType &0x100)
		{
			MACRO_DRAWPREFIX(expressline)(iBBX.left+tllinedist*(cos(cangle)+(cos(langle)*tllefttan2)),iBBX.top+tllinedist*(sin(cangle)+(sin(langle)*tllefttan2)),iBBX.right+tllinedist*(cos(cangle)-(cos(langle)*tllefttan)),iBBX.bottom+tllinedist*(sin(cangle)-(sin(langle)*tllefttan)));
			MACRO_DRAWPREFIX(expressline)(iBBX.left-tllinedist*(cos(cangle)-(cos(langle)*tllefttan2)),iBBX.top-tllinedist*(sin(cangle)-(sin(langle)*tllefttan2)),iBBX.right-tllinedist*(cos(cangle)+(cos(langle)*tllefttan)),iBBX.bottom-tllinedist*(sin(cangle)+(sin(langle)*tllefttan)));
		}
		else
		{
			MACRO_DRAWPREFIX(expressline)(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom);
		}
		othercangle=cangle+Pi;
		otherlangle=langle+Pi;
	}
	else
	if (tlGraphicType==2)
	{
		float deltax,deltay;
		float tlradius;
		float tlangle;
		deltax=iBBX.left-iBBX.right;
		deltay=iBBX.top-iBBX.bottom;
		tlangle=getangle(deltax,deltay);
		tlradius=sqrt(deltax*deltax+deltay*deltay);
		MACRO_DRAWPREFIX(stylegenestring)(stylefromline(currentLineType));
		if (currentLineType &0x100)
		{
			if (tllinedist==0) tllinedist=4;
		}
		if (currentEllipsemode)
		{
			float tla,tlb,tlc,tld,tle;
			double ellipticx[4];
			double ellipticy[8];
			ellipsoid.fill(deltax,deltay);
			float tlangle;
			tlradius=sqrt(deltax*deltax+deltay*deltay);
			if (currentLineType & 0x100)
			{
				float takefromark=0;
				float takefromark1=0;
				float takefromark2=0;
				if ((currentArrowHeadType==2)||(currentArrowHeadType==3))
				{
					takefromark=fabs(tllinedist/ellipsoid.radiusx);
					if (currentArrowHeadType==2)
					{
						takefromark*=2;
					}
					if (tlAngularSize>0)
					{
						takefromark=-takefromark;
					}
					if (currentArrowHeadHead==2)
					{
						takefromark1=takefromark;
					}
					if (currentArrowHeadTail==2)
					{
						takefromark2=takefromark;
					}
				}
				MACRO_DRAWPREFIX(expressarc_enhanced)(iBBX.right,iBBX.bottom,ellipsoid.radiusx+tllinedist,ellipsoid.radiusy+tllinedist,ellipsoid.internalangle-takefromark1,ellipsoid.internalangle+((tlAngularSize/180.0)*Pi+takefromark2),ellipsoid.axangle);
				MACRO_DRAWPREFIX(expressarc_enhanced)(iBBX.right,iBBX.bottom,ellipsoid.radiusx-tllinedist,ellipsoid.radiusy-tllinedist,ellipsoid.internalangle-takefromark1,ellipsoid.internalangle+((tlAngularSize/180.0)*Pi+takefromark2),ellipsoid.axangle);
			}
			else
			{
				MACRO_DRAWPREFIX(expressarc_enhanced)(iBBX.right,iBBX.bottom,ellipsoid.radiusx,ellipsoid.radiusy,ellipsoid.internalangle,ellipsoid.internalangle+((tlAngularSize/180.0)*Pi),ellipsoid.axangle);
			}
			for (int ilv0=0;ilv0<2;ilv0++)
			{
				double tlbest=1e20;
				int tlbestone=-1;
				tlangle=ellipsoid.internalangle+((ilv0)?((tlAngularSize/180.0)*Pi):0);
				goto stillacircle;//TODO: BUG: THE OTHER ALGORITHM IS DEFECTIVE unless the arrow is rotated by non-90° angles....
				if (tlradius<=arrowheadlength){goto stillacircle;}//Where we use tangential arrow tips
				ARROW_ELLIPTIC(ellipsoid.radiusx/arrowheadlength,ellipsoid.radiusy/arrowheadlength,cos(tlangle),sin(tlangle),tla,tlb,tlc,tld,tle);
				printf("TLA: %f;TLB: %f;TLC: %f;TLD: %f;TLB: %f;\n",tla,tlb,tlc,tld,tle);
				if (fabs(tla)<=1e-3) {goto stillacircle;}
				QUARTIC_quartic(tla,tlb,tlc,tld,tle,&(ellipticx[0]),&(ellipticx[1]),&(ellipticx[2]),&(ellipticx[3]));
				for (int ilv1=0;ilv1<4;ilv1++)
				{
					if (!(isnan(ellipticx[ilv1])))
					{
						ellipticy[ilv1]=sqrt(1.0-fsqr(ellipticx[ilv1]))*ellipsoid.radiusy;
						ellipticy[ilv1+4]=-sqrt(1.0-fsqr(ellipticx[ilv1]))*ellipsoid.radiusy;
						if (abs(ellipticx[ilv1])>1) {ellipticy[ilv1]=ellipticx[ilv1]*ellipsoid.radiusy;ellipticy[ilv1]=-ellipticx[ilv1+4]*ellipsoid.radiusy;}
						ellipticx[ilv1]*=ellipsoid.radiusx;
					}
				}
				float tlsinus=sin(tlangle+(((tlAngularSize>0)^(ilv0))?(Pi/2):(-Pi/2)));
				float tlcosinus=cos(tlangle+(((tlAngularSize>0)^(ilv0))?(Pi/2):(-Pi/2)));
				for (int ilv1=0;ilv1<8;ilv1++)
				{
					if (!((isnan(ellipticx[ilv1%4]))||(isnan(ellipticy[ilv1]))))
					{
						float tlhorz=ellipticx[ilv1%4]-cos(tlangle)*ellipsoid.radiusx;
						float tlvert=ellipticy[ilv1]-sin(tlangle)*ellipsoid.radiusy;
						float tltemp=fabs(sqrt(fsqr(tlhorz)+fsqr(tlvert))-arrowheadlength);
						if ((tlhorz*tlcosinus+tlvert*tlsinus)<0)
						{
							tltemp+=arrowheadlength*2;
						}
						if (tltemp<tlbest)
						{
							tlbestone=ilv1;
							tlbest=tltemp;
						}
					}
				}
				if (tlbestone!=-1)
				{
					(ilv0?otherlangle:langle)=getangle(ellipticx[tlbestone%4]-cos(tlangle)*ellipsoid.radiusx,ellipticy[tlbestone]-sin(tlangle)*ellipsoid.radiusy)+ellipsoid.axangle;
					(ilv0?othercangle:cangle)=(ilv0?otherlangle:langle)+Pi/2;
					printf("Ḃ%f\n",cangle);
				}
				else
				{
					printf("S%f\n",cangle);
					goto stillacircle;
				}
				if (ilv0==0)
				{
					iBBX.left=currentCenter3D.x+cos(tlangle)*ellipsoid.radiusx*cos(ellipsoid.axangle)-sin(tlangle)*ellipsoid.radiusy*sin(ellipsoid.axangle);
					iBBX.top=currentCenter3D.y+sin(tlangle)*ellipsoid.radiusy*cos(ellipsoid.axangle)+cos(tlangle)*ellipsoid.radiusx*sin(ellipsoid.axangle);
				}
				if (ilv0==1)
				{
					iBBX.right+=currentCenter3D.x+cos(tlangle)*ellipsoid.radiusx*cos(ellipsoid.axangle)-sin(tlangle)*ellipsoid.radiusy*sin(ellipsoid.axangle);
					iBBX.bottom+=currentCenter3D.y+sin(tlangle)*ellipsoid.radiusy*cos(ellipsoid.axangle)+cos(tlangle)*ellipsoid.radiusx*sin(ellipsoid.axangle);
				}
			}
		}
		else
		{
			if (currentLineType & 0x100)
			{
				MACRO_DRAWPREFIX(expressarc)(iBBX.right,iBBX.bottom,tlradius-tllinedist,tlradius-tllinedist,tlangle,tlangle+((tlAngularSize/180.0)*Pi));
				MACRO_DRAWPREFIX(expressarc)(iBBX.right,iBBX.bottom,tlradius+tllinedist,tlradius+tllinedist,tlangle,tlangle+((tlAngularSize/180.0)*Pi));
			}
			else
			{
				MACRO_DRAWPREFIX(expressarc)(iBBX.right,iBBX.bottom,tlradius,tlradius,tlangle,tlangle+((tlAngularSize/180.0)*Pi));
			}
			stillacircle:
			if (tlAngularSize>0)
			{
				 langle=(tlangle+Pi/2.0);
			}
			else
			{
				 langle=(tlangle-Pi/2.0);
			}
			cangle=langle+Pi/2.0;
			otherlangle=langle+((tlAngularSize/180.0)*Pi)+Pi;
			othercangle=otherlangle+Pi/2.0;
			float dturn;
			if (tlradius>arrowheadlength)
			{
				dturn=asin(arrowheadlength/tlradius/2);
			}
			else
			{
				dturn=0.5236;
			}
			if (tlAngularSize>0)
			{
				langle+=dturn;
				cangle+=dturn;
				otherlangle-=dturn;
				othercangle-=dturn;
			}
			else
			{
				langle-=dturn;
				cangle-=dturn;
				otherlangle+=dturn;
				othercangle+=dturn;
			}
			iBBX.right+=tlradius*cos(tlangle+((tlAngularSize/180.0)*Pi));
			iBBX.bottom+=tlradius*sin(tlangle+((tlAngularSize/180.0)*Pi));
		}
	}
	else goto skiparrows;
	MACRO_DRAWPREFIX(drawarrheads)(iBBX,langle,cangle,otherlangle,othercangle,currentArrowHeadType,currentArrowHeadTail,currentArrowHeadHead,tllinedist);
	goto skipthisgraphic;
	skiparrows:
	if ((*i_graphic_instance).GraphicType==3)
	{
		//TODO SUBJECT make LENHACK Frame omissions in conversion, not in draw
		MACRO_DRAWPREFIX(stylegenestring)(stylefromattrs((*i_graphic_instance).LineType,(*i_graphic_instance).FillType));
		float tl_corn=(*i_graphic_instance).CornerRadius;
		if ((*i_graphic_instance).CornerRadius>0)
		{
			if (iBBX.left>iBBX.right)
			{
				float swap=iBBX.left;
				iBBX.left=iBBX.right;
				iBBX.right=swap;
			}
			if (iBBX.top>iBBX.bottom)
			{
				float swap=iBBX.top;
				iBBX.top=iBBX.bottom;
				iBBX.bottom=swap;
			}
			if (MACRO_DRAWPREFIX(expressgeometry_start)(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom))
			{
				MACRO_DRAWPREFIX(expressgeometry_begin)(iBBX.left+tl_corn,iBBX.top);
				MACRO_DRAWPREFIX(expressgeometry_line)(iBBX.right-tl_corn,iBBX.top);
				MACRO_DRAWPREFIX(expressgeometry_bezier2)(iBBX.right,iBBX.top,iBBX.right,iBBX.top+tl_corn);
				MACRO_DRAWPREFIX(expressgeometry_line)(iBBX.right,iBBX.bottom-tl_corn);
				MACRO_DRAWPREFIX(expressgeometry_bezier2)(iBBX.right,iBBX.bottom,iBBX.right-tl_corn,iBBX.bottom);
				MACRO_DRAWPREFIX(expressgeometry_line)(iBBX.left+tl_corn,iBBX.bottom);
				MACRO_DRAWPREFIX(expressgeometry_bezier2)(iBBX.left,iBBX.bottom,iBBX.left,iBBX.bottom-tl_corn);
				MACRO_DRAWPREFIX(expressgeometry_line)(iBBX.left,iBBX.top+tl_corn);
				MACRO_DRAWPREFIX(expressgeometry_bezier2)(iBBX.left,iBBX.top,iBBX.left+tl_corn,iBBX.top);
				MACRO_DRAWPREFIX(expressgeometry_end)();
			}
		}
		else
		{
			MACRO_DRAWPREFIX(expresstetrangle)(iBBX.left,iBBX.top,iBBX.right,iBBX.top,iBBX.right,iBBX.bottom,iBBX.left,iBBX.bottom);
		}
	}
	if ((*i_graphic_instance).GraphicType==4)
	{
		MACRO_DRAWPREFIX(stylegenestring)(stylefromattrs((*i_graphic_instance).LineType,(*i_graphic_instance).FillType));
		ellipsoid.create((*i_graphic_instance).Center3D,(*i_graphic_instance).MajorAxisEnd3D,(*i_graphic_instance).MinorAxisEnd3D);
		MACRO_DRAWPREFIX(expressspinellipse)((*i_graphic_instance).Center3D.x,(*i_graphic_instance).Center3D.y,ellipsoid.radiusx,ellipsoid.radiusy,ellipsoid.axangle);
	}
	if ((*i_graphic_instance).GraphicType==5)
	{
		#ifdef LENNARD_HACK
		if ((*((*i_graphic_instance).annotation)).count_in_it>0)
		{
			current_hack_annotation=((*glob_annotation_multilist)[(*((*i_graphic_instance).annotation)).start_in_it].Content.a);
		}
		else
		{
			goto skipthisgraphic;
		}
		#endif
		MACRO_DRAWPREFIX(stylegenestring)(stylefromattrs((*i_graphic_instance).LineType,(*i_graphic_instance).FillType));
		ellipsoid.create((*i_graphic_instance).Center3D,(*i_graphic_instance).MajorAxisEnd3D,(*i_graphic_instance).MinorAxisEnd3D);
		MACRO_DRAWPREFIX(expressspinellipse)((*i_graphic_instance).Center3D.x,(*i_graphic_instance).Center3D.y,ellipsoid.radiusx,ellipsoid.radiusy,ellipsoid.axangle);
	}
	if ((*i_graphic_instance).GraphicType==7)
	{
		MACRO_DRAWPREFIX(draw_Symbol)(i_graphic_instance->SymbolType);
	}
	if ((*i_graphic_instance).GraphicType==6)
	{
		MACRO_DRAWPREFIX(stylegenestring)(stylefromattrs((*i_graphic_instance).LineType,(*i_graphic_instance).FillType));
		cangle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top)+Pi/2;
		langle=getangle(iBBX.right-iBBX.left,iBBX.bottom-iBBX.top);
		switch ((*i_graphic_instance).BracketType)
		{
			case 3:
			{
				MACRO_DRAWPREFIX(expressline)(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom);
				MACRO_DRAWPREFIX(expressline)(iBBX.left,iBBX.top,iBBX.left+13.4*cos(cangle),iBBX.top+13.4*sin(cangle));
				MACRO_DRAWPREFIX(expressline)(iBBX.right,iBBX.bottom,iBBX.right+13.4*cos(cangle),iBBX.bottom+13.4*sin(cangle));
				break;
			}
			case 4:
			{
				MACRO_DRAWPREFIX(expressline)(iBBX.left+13.4*cos(langle),iBBX.top+13.4*sin(langle),(iBBX.left+iBBX.right)/2-13.4*cos(langle),(iBBX.top+iBBX.bottom)/2-13.4*sin(langle));
				MACRO_DRAWPREFIX(expressline)((iBBX.left+iBBX.right)/2+13.4*cos(langle),(iBBX.top+iBBX.bottom)/2+13.4*sin(langle),iBBX.right-13.4*cos(langle),iBBX.bottom-13.4*sin(langle));
				MACRO_DRAWPREFIX(expressbezier)(iBBX.left+13.4*cos(langle),iBBX.top+13.4*sin(langle),
				iBBX.left+6.7*cos(langle),iBBX.top+6.7*sin(langle),
				iBBX.left+6.7*cos(cangle),iBBX.top+6.7*sin(cangle),
				iBBX.left+13.4*cos(cangle),iBBX.top+13.4*sin(cangle));
				MACRO_DRAWPREFIX(expressbezier)((iBBX.left+iBBX.right)/2+13.4*cos(langle),(iBBX.top+iBBX.bottom)/2+13.4*sin(langle),
				(iBBX.left+iBBX.right)/2+6.7*cos(langle),(iBBX.top+iBBX.bottom)/2+6.7*sin(langle),
				(iBBX.left+iBBX.right)/2-6.7*cos(cangle),(iBBX.top+iBBX.bottom)/2-6.7*sin(cangle),
				(iBBX.left+iBBX.right)/2-13.4*cos(cangle),(iBBX.top+iBBX.bottom)/2-13.4*sin(cangle));
				MACRO_DRAWPREFIX(expressbezier)(iBBX.right-13.4*cos(langle),iBBX.bottom-13.4*sin(langle),
				iBBX.right-6.7*cos(langle),iBBX.bottom-6.7*sin(langle),
				iBBX.right+6.7*cos(cangle),iBBX.bottom+6.7*sin(cangle),
				iBBX.right+13.4*cos(cangle),iBBX.bottom+13.4*sin(cangle));
				MACRO_DRAWPREFIX(expressbezier)((iBBX.left+iBBX.right)/2-13.4*cos(langle),(iBBX.top+iBBX.bottom)/2-13.4*sin(langle),
				(iBBX.left+iBBX.right)/2-6.7*cos(langle),(iBBX.top+iBBX.bottom)/2-6.7*sin(langle),
				(iBBX.left+iBBX.right)/2-6.7*cos(cangle),(iBBX.top+iBBX.bottom)/2-6.7*sin(cangle),
				(iBBX.left+iBBX.right)/2-13.4*cos(cangle),(iBBX.top+iBBX.bottom)/2-13.4*sin(cangle));
				break;
			}
			case 5:
			{
				MACRO_DRAWPREFIX(expressbow)(iBBX.left,iBBX.top,iBBX.right,iBBX.bottom,1.8);
				break;
			}
		}
	}
	skipthisgraphic:
	;//always run over here. One might need this.
	goto svg_main_loop;
	svg_main_arrow:
	i_arrow_instance=&((*glob_arrow_multilist))[index_in_buffer];
	colornr=(*i_arrow_instance).color;
	MACRO_DRAWPREFIX(get_colorstring)(colornr);
	currentArrowHeadType=(*i_arrow_instance).ArrowheadType;
	currentArrowHeadTail=(*i_arrow_instance).ArrowheadTail;
	currentArrowHeadHead=(*i_arrow_instance).ArrowheadHead;
	tllinedist=0;
	tllefttan=0;
	tlrighttan=0;
	tllefttan2=0;
	tlrighttan2=0;
	langle=0;cangle=0;
	currentLineType=(*i_arrow_instance).LineType;//0: normal 2: Bold 0x100: Double
	currentEllipsemode=0;
	if (((*i_arrow_instance).ArrowShaftSpacing)>0)
	{
		tllinedist=4;
		currentLineType|=0x100;
	}
		if (currentArrowHeadHead==2)
		{
			tllefttan2=1+(currentArrowHeadType==2);
		}
		if (currentArrowHeadTail==2)
		{
			tllefttan=1+(currentArrowHeadType==2);
		}
	if (currentArrowHeadType & 2)
	{
		tllinedist=8;
		currentLineType|=0x100;
	}
	tlAngularSize=(*i_arrow_instance).AngularSize;
	if (tlAngularSize!=0)
	{
		char tlcounter=(tlAngularSize<0);
		tlGraphicType=2;
		iBBX.right=(*i_arrow_instance).Center3D.x;
		iBBX.bottom=(*i_arrow_instance).Center3D.y;
		currentCenter3D=(*i_arrow_instance).MajorAxisEnd3D;
		iBBX.top=(*i_arrow_instance).MajorAxisEnd3D.y;
		ellipsoid.create((*i_arrow_instance).Center3D,(*i_arrow_instance).MajorAxisEnd3D,(*i_arrow_instance).MinorAxisEnd3D);
		ellipsoid.fill((*i_arrow_instance).Tail3D.x-(*i_arrow_instance).Center3D.x,(*i_arrow_instance).Tail3D.y-(*i_arrow_instance).Center3D.y);
		tlAngularSize=ellipsoid.internalangle;
		ellipsoid.fill((*i_arrow_instance).Head3D.x-(*i_arrow_instance).Center3D.x,(*i_arrow_instance).Head3D.y-(*i_arrow_instance).Center3D.y);
		tlAngularSize-=ellipsoid.internalangle;
		tlAngularSize=fmod(tlAngularSize+(4*Pi),2*Pi)*180.0/Pi;
		if (tlcounter)
		{
			tlAngularSize=-360.0+tlAngularSize;
		}
		currentEllipsemode=1;
	}
	else
	{
		tlGraphicType=1;
		iBBX.right=(*i_arrow_instance).Tail3D.x;
		iBBX.bottom=(*i_arrow_instance).Tail3D.y;
	}
	iBBX.left=(*i_arrow_instance).Head3D.x;
	iBBX.top=(*i_arrow_instance).Head3D.y;
	goto svg_graphic_arrow_shunt;
	goto svg_main_loop;
	svg_main_b:
	_small inr_E,inr_S;
	i_b_instance=&(((*glob_b_multilist))[index_in_buffer]);
	colornr=(*i_b_instance).color;
	MACRO_DRAWPREFIX(get_colorstring)(colornr);
	inr_S=bond_actual_node[index_in_buffer].start;
	inr_E=bond_actual_node[index_in_buffer].end;
	startnode=((*glob_n_multilist).bufferlist())+inr_S;
	endnode=((*glob_n_multilist).bufferlist())+inr_E;
	ibonddist=0;ibonddist2=0;
	if ((*i_b_instance).Order>16)
	{
		switch((*i_b_instance).DoublePosition & 0xFF)
		{
			case 0 : ibonddist=bonddist/2;ibonddist2=-bonddist/2;break;
			case 1 : ibonddist=bonddist;break;
			case 2 : ibonddist=-bonddist;break;
		}
		if ((*i_b_instance).Order>32)
		{
			ibonddist=bonddist;ibonddist2=0;
			if ((*i_b_instance).Order>48)
			{
				ibonddist=(bonddist/2)*3;ibonddist2=(bonddist/2);
			}
		}
	}
	langle=getangle((*endnode).xyz.x-(*startnode).xyz.x,(*endnode).xyz.y-(*startnode).xyz.y);
	cangle=langle+Pi/2;
	if ((hatches==2) && ((selection_clickabilitymatrix.types2[0]|selection_clickabilitymatrix.types2[1]) & (1<<STRUCTURE_OBJECTTYPE_n)))
	{
		textdeltax=7;
		textdeltay=7;
		return;
	}
	else
	{
		calcdelta(&textdeltax,&textdeltay,(*endnode).xyz.x-(*startnode).xyz.x,(*endnode).xyz.y-(*startnode).xyz.y);
	}
	specialS=((*glob_n_multilist)[inr_S].Element!=constants_Element_implicitcarbon);
	specialE=((*glob_n_multilist)[inr_E].Element!=constants_Element_implicitcarbon);
	iBBX.left=(*startnode).xyz.x+textdeltax*specialS;
	iBBX.top=(*startnode).xyz.y+textdeltay*specialS;
	iDisplaytype1=((*glob_b_multilist)[index_in_buffer].Display);
	iBBX.right=(*endnode).xyz.x-textdeltax*specialE;
	iBBX.bottom=(*endnode).xyz.y-textdeltay*specialE;
	if (iswedgenr(iDisplaytype1)>0)
	{
		ibonddist3=0;
		ibonddist4=0;
		if (iswedgenr(iDisplaytype1) & 2)
		{
			ibonddist3=boldwidth/2;
		}
		if (iswedgenr(iDisplaytype1) & 1)
		{
			ibonddist4=boldwidth/2;
		}
		MACRO_DRAWPREFIX(stylegenestring)(
(((iDisplaytype1==5) || (iDisplaytype1==6) || (iDisplaytype1==7))?2:0) |
1);
		float tllefttan=0;
		float tlrighttan=0;
		float tllefttan2=0;
		float tlrighttan2=0;
		if (specialE==0)
		{
			if ((bond_actual_node[index_in_buffer]).numberleft[0]!=-1) {tllefttan=tan(Pi/2-bond_actual_node[index_in_buffer].cotanleft[0]);}else{if ((bond_actual_node[index_in_buffer]).numberright[0]!=-1){tllefttan=tan(bond_actual_node[index_in_buffer].xcotanright[0]/2);}}
			if ((bond_actual_node[index_in_buffer]).numberright[0]!=-1) {tlrighttan=tan(Pi/2-bond_actual_node[index_in_buffer].cotanright[0]);}else{if ((bond_actual_node[index_in_buffer]).numberleft[0]!=-1){tlrighttan=tan(bond_actual_node[index_in_buffer].xcotanleft[0]/2);}}
		}
		if (specialS==0)
		{
			if ((bond_actual_node[index_in_buffer]).numberleft[1]!=-1) {tllefttan2=tan(Pi/2-bond_actual_node[index_in_buffer].cotanleft[1]);}else{if ((bond_actual_node[index_in_buffer]).numberright[1]!=-1){tllefttan2=tan(bond_actual_node[index_in_buffer].xcotanright[1]/2);}}
			if ((bond_actual_node[index_in_buffer]).numberright[1]!=-1) {tlrighttan2=tan(Pi/2-bond_actual_node[index_in_buffer].cotanright[1]);}else{if ((bond_actual_node[index_in_buffer]).numberleft[1]!=-1){tlrighttan2=tan(bond_actual_node[index_in_buffer].xcotanleft[1]/2);}}
		}
		if ((iDisplaytype1==2) || (iDisplaytype1==3) || (iDisplaytype1==4))
		{
			MACRO_DRAWPREFIX(expresshashangle)(langle,cangle,
			iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle),
			iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),
			ibonddist3,ibonddist4
			);
		}
		else
		{
			MACRO_DRAWPREFIX(expresshexangle)(
iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle),
iBBX.right+ibonddist2*cos(cangle)+ibonddist4*(-cos(cangle)-(cos(langle)*tllefttan)),iBBX.bottom+ibonddist2*sin(cangle)+ibonddist4*(-sin(cangle)-(sin(langle)*tllefttan)),
iBBX.left+ibonddist2*cos(cangle)+ibonddist3*(-cos(cangle)+(cos(langle)*tlrighttan2)),iBBX.top+ibonddist2*sin(cangle)+ibonddist3*(-sin(cangle)+(sin(langle)*tlrighttan2)),
iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),
iBBX.left+ibonddist2*cos(cangle)+ibonddist3*(cos(cangle)+(cos(langle)*tllefttan2)),iBBX.top+ibonddist2*sin(cangle)+ibonddist3*(sin(cangle)+(sin(langle)*tllefttan2)),
iBBX.right+ibonddist2*cos(cangle)+ibonddist4*(cos(cangle)-(cos(langle)*tlrighttan)),iBBX.bottom+ibonddist2*sin(cangle)+ibonddist4*(sin(cangle)-(sin(langle)*tlrighttan))
			);
			ibonddist+=fabs(bonddist)*((ibonddist>0) ? 1 : -1);
		}
	}
	else
	{
		MACRO_DRAWPREFIX(stylegenestring)(((iDisplaytype1==1) ?8:0) | 1);
		MACRO_DRAWPREFIX(expressline)(iBBX.left+ibonddist2*cos(cangle),iBBX.top+ibonddist2*sin(cangle),iBBX.right+ibonddist2*cos(cangle),iBBX.bottom+ibonddist2*sin(cangle));
	}
	if ((*i_b_instance).Order>16)
	{
		MACRO_DRAWPREFIX(stylegenestring)((((*glob_b_multilist)[index_in_buffer].Display2==1)?8:0)|1);
		MACRO_DRAWPREFIX(expressline)(iBBX.left+ibonddist*cos(cangle),iBBX.top+ibonddist*sin(cangle),iBBX.right+ibonddist*cos(cangle),iBBX.bottom+ibonddist*sin(cangle));
		if ((*i_b_instance).Order>32)
		{
			MACRO_DRAWPREFIX(expressline)(iBBX.left-ibonddist*cos(cangle),iBBX.top-ibonddist*sin(cangle),iBBX.right-ibonddist*cos(cangle),iBBX.bottom-ibonddist*sin(cangle));
			if ((*i_b_instance).Order>48)
			{
				MACRO_DRAWPREFIX(expressline)(iBBX.left-ibonddist2*cos(cangle),iBBX.top-ibonddist2*sin(cangle),iBBX.right-ibonddist2*cos(cangle),iBBX.bottom-ibonddist2*sin(cangle));
			}
		}
	}
	for (int ilv0=0;ilv0<4;ilv0++)
	{
		n_instance * tlnode=(ilv0/2)?startnode:endnode;
		MACRO_DRAWPREFIX(get_colorstring)((*tlnode).color);
		MACRO_DRAWPREFIX(stylegenestring)(1);
/*		if ((*tlnode).ExternalConnectionType==4)
		{
			float tlposx=(*tlnode).xyz.x;
			float tlposy=(*tlnode).xyz.y;
			float tlsin=sin(langle+((ilv0%2)?(Pi/2):(-Pi/2)))*4;
			float tlcos=cos(langle+((ilv0%2)?(Pi/2):(-Pi/2)))*4;
			MACRO_DRAWPREFIX(expressxbezier)(10,tlposx,tlposy,tlposx+tlsin,tlposy-tlcos,tlposx+tlsin+tlcos,tlposy-tlcos+tlsin,tlposx+tlcos,tlposy+tlsin,
tlposx+tlcos-tlsin,tlposy+tlsin+tlcos,tlposx+2*tlcos-tlsin,tlposy+2*tlsin+tlcos,tlposx+2*tlcos,tlposy+2*tlsin
,tlposx+2*tlcos+0.25*tlsin,tlposy+2*tlsin-0.25*tlcos,tlposx+2.25*tlcos+0.5*tlsin,tlposy+2.25*tlsin-0.5*tlcos,tlposx+2.5*tlcos+0.5*tlsin,tlposy+2.5*tlsin-0.5*tlcos);
		}*///TODO SUBJECT: Buffered external connection Type
	}
	goto svg_main_loop;
	svg_main_t:
	sortback=0;
	owner=text_actual_node[index_in_buffer].owner;
	colornr=0;
	MACRO_DRAWPREFIX(get_colorstring)(colornr);
	if (owner!=-1)
	{
		if (atom_actual_node[owner].labelside==1)
		{
			sortback=1;
		}
		colornr=(*glob_n_multilist)[text_actual_node[index_in_buffer].owner].color;
		MACRO_DRAWPREFIX(get_colorstring)(colornr);
	}
	basecolornr=((*glob_t_multilist))[index_in_buffer].color;
	MACRO_DRAWPREFIX(get_colorstring_passive)(colornr);

	gfx_txselectmode=(((control_mousestate & 0x40)>0) && (control_textedit_selectmode==1) && (control_textedit_type==STRUCTURE_OBJECTTYPE_t) && (control_textedit_index==index_in_buffer));
	MACRO_DRAWPREFIX(express_txinit)(((*glob_t_multilist)[index_in_buffer].LabelAlignment==-1),((*glob_t_multilist))[index_in_buffer].xyz.x,((*glob_t_multilist))[index_in_buffer].xyz.y,atomfontheight,(*glob_t_multilist)[index_in_buffer].RotationAngle);
	TELESCOPE_aggressobject(glob_t_multilist,index_in_buffer);
	n_to_t_shunt:
	int tlbackval;
	tlbackval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	string_resorted=0;
	ifsmat=0;//0: nothing //1: on a subscript number; 2: on text; 3: on a superscript
	tl_bracketmode=0;
	goto svg_text_introskip;
	svg_text_back:
	if (!tlbackval)
	{
		goto svg_text_finished;
	}
	colornr=(*((s_instance*)TELESCOPE_getproperty())).color;
	fontnr=(*((s_instance*)TELESCOPE_getproperty())).font;
	finalstring=((char*)TELESCOPE_getproperty_contents());
	char * iparms;
	iparms=STRINGOUTPUT_emptyformat;
	{
		int tlformlabeltype=(*((s_instance*)TELESCOPE_getproperty())).face;
		currentsetfontsize=(*((s_instance*)TELESCOPE_getproperty())).size;
		currentsetfontsize=16;
		#ifdef LENNARD_HACK
		if (currentsetfontsize<=99.0)
		{
			currentsetfontsize=24;
			if (tlformlabeltype & 0x1)
			{
				if (((SVG_width>1280) && (irestriction==0)) || ((SVG_currentfringex-SVG_currentbasex>1280) && (irestriction==0)))
				{
					currentsetfontsize=38;
				}
				iparms=STRINGOUTPUT_bold;
			}
		}
		#endif
		#ifdef CAMBRIDGESOFT_CONFORMING
		sprintf(iparmsstring,"%s",((tlformlabeltype & 0x1) ? STRINGOUTPUT_bold : STRINGOUTPUT_emptyformat),currentsetfontsize);
		iparms=iparmsstring;
		#endif
	}
	if (colornr==0) colornr=basecolornr;
	MACRO_DRAWPREFIX(get_colorstring)(colornr);
	if ((((*((s_instance*)TELESCOPE_getproperty())).connect & 0x6)!=0) && (sortback))
	{
		switch (finalstring[0])
		{
			case '(' : finalstring=(char*)")";break;
			case ')' : finalstring=(char*)"(";break;
			case '[' : finalstring=(char*)"]";break;
			case ']' : finalstring=(char*)"[";break;
			case '{' : finalstring=(char*)"}";break;
			case '}' : finalstring=(char*)"{";break;
		}
	}
	{
		int tlformlabeltype=(*((s_instance*)TELESCOPE_getproperty())).face;
		currentsetfontsize=(*((s_instance*)TELESCOPE_getproperty())).size;
		currentsetfontsize=16;
		if (((*((s_instance*)TELESCOPE_getproperty())).face & 0x60)==0x60)
		{
			tlformlabeltype=(*((s_instance*)TELESCOPE_getproperty())).effface;
		}
		#ifdef CAMBRIDGESOFT_CONFORMING
		sprintf(iparmsstring,"%s",((tlformlabeltype & 0x1) ? STRINGOUTPUT_bold : STRINGOUTPUT_emptyformat),currentsetfontsize);
		iparms=iparmsstring;
		#endif
	//MACRO_DRAWPREFIX(formatfromifsmat(ifsmat))
		MACRO_DRAWPREFIX(draw_printformatted)(finalstring,iparms,tlformlabeltype,0,strlen(finalstring));
	}
/*	if (sortback)
	{
		text_rewind((unsigned char*)finalstring,strlen(finalstring));
	}*/
	if ((tl_bracketmode==2) && (sortback)) goto tl_bracketmode_shortcut;
	tl_connectmode=((((s_instance*)TELESCOPE_getproperty())->connect & 1)>0);
	if ((sortback) && ((((s_instance*)TELESCOPE_getproperty())->connect & 2)>0))
	{
		while (tl_connectmode)
		{
			tlbackval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
			tl_connectmode=((((s_instance*)TELESCOPE_getproperty())->connect & 1)>0);
			if (tlbackval==0) goto svg_text_finished;
		}
	}
	if (tl_connectmode==0) ifsmat=0;
	svg_text_retryafterbracket:;
	tlbackval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
	if (tlbackval==0) {if (tl_bracketmode==1) goto svg_text_bracket_finished; else goto svg_text_finished;}
	if ((tl_connectmode) && (sortback))
	{
		goto svg_text_back;
	}
	if (tl_bracketmode==1) goto svg_text_bracket_finished;
	if (sortback)
	{
		int tl_TELESCOPE_lastpos=TELESCOPE_tempvar.subpos;
		int tl_TELESCOPE_lastpos2=TELESCOPE_tempvar.subpos2;
		int tl_sort_backval;
		tl_sort_bracketmode=0;
		tl_connectmode=1;
		tl_sort_backval=1;
		goto svg_text_sort_introskip;
		svg_text_sort_back:;
		MACRO_DRAWPREFIX(text_rewind)(((unsigned char*)TELESCOPE_getproperty_contents()),strlen(((char*)TELESCOPE_getproperty_contents())));
		if ((tl_sort_bracketmode==2) && (sortback)) goto tl_sort_bracketmode_shortcut;
		tl_connectmode=((((s_instance*)TELESCOPE_getproperty())->connect & 1)>0);
		if ((sortback) && ((((s_instance*)TELESCOPE_getproperty())->connect & 2)>0))
		{
			while (tl_connectmode)
			{
				tl_sort_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
				tl_connectmode=((((s_instance*)TELESCOPE_getproperty())->connect & 1)>0);
				if (tl_sort_backval==0) goto svg_text_finished;
			}
		}
		tl_sort_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		if (tl_connectmode==0) goto svg_text_sortback_finished;
		svg_text_sort_introskip:;
		if (tl_sort_backval==0) goto svg_text_sortback_finished;
		if (((sortback) && ((((s_instance*)TELESCOPE_getproperty())->connect & 4)>0)))//this routine manages jumps when numbers are telling the count of the sort_bracket's contents
		{
			tl_sort_bracketmode_shortcut:;
			int tl_sort_bracket_backval;
			tl_posbeforesort_bracket=TELESCOPE_tempvar.subpos;
			tl_pos2beforesort_bracket=TELESCOPE_tempvar.subpos2;
			tl_backvalbeforesort_bracket=tl_sort_backval;
			tl_sort_bracketlevel=1;
			svg_text_sort_bracket_back:;
			tl_sort_bracket_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
			if (tl_sort_bracket_backval==0) goto svg_text_sort_bracket_finished;
			tl_connect=((s_instance*)TELESCOPE_getproperty())->connect;
			if ((tl_connect & 4)>0) {tl_sort_bracketlevel++;}
			if ((tl_connect & 2)>0) {tl_sort_bracketlevel--;}
			if (tl_sort_bracketlevel==0)
			{
				if ((tl_connect & 1)==0)
				{
					tl_sort_bracketmode=0;
					tl_sort_backval=tl_backvalbeforesort_bracket;
					TELESCOPE_tempvar.subpos=tl_posbeforesort_bracket;
					TELESCOPE_tempvar.subpos2=tl_pos2beforesort_bracket;
					tl_connectmode=0;
					goto svg_text_sort_back;
				}
				if (tl_sort_bracketmode==2)
				{
					tl_sort_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
					if (tl_sort_backval==0) goto svg_text_sort_bracket_finished;
					tl_sort_bracketmode=1;
				}
				else
				{
					tl_sort_backval=tl_backvalbeforesort_bracket;
					TELESCOPE_tempvar.subpos=tl_posbeforesort_bracket;
					TELESCOPE_tempvar.subpos2=tl_pos2beforesort_bracket;
					tl_sort_bracketmode=2;
				}
				goto svg_text_sort_back;
			}
			goto svg_text_sort_bracket_back;
			svg_text_sort_bracket_finished:;
			tl_sort_bracketmode=0;
			tl_sort_backval=tl_backvalbeforesort_bracket;
			TELESCOPE_tempvar.subpos=tl_posbeforesort_bracket;
			TELESCOPE_tempvar.subpos2=tl_pos2beforesort_bracket;
			tl_connectmode=0;
			goto svg_text_sortback_finished;
		}
		if (tl_sort_bracketmode>0) goto svg_text_sort_bracket_finished;
		if (tl_connectmode)
		{
			goto svg_text_sort_back;
		}
		svg_text_sortback_finished:;
		TELESCOPE_tempvar.subpos=tl_TELESCOPE_lastpos;
		TELESCOPE_tempvar.subpos2=tl_TELESCOPE_lastpos2;
	}
	svg_text_introskip:;
	if (((sortback) && ((((s_instance*)TELESCOPE_getproperty())->connect & 4)>0)))//this routine manages jumps when numbers are telling the count of the bracket's contents
	{
		tl_bracketmode_shortcut:;
		int tl_bracket_backval;
		tl_posbeforebracket=TELESCOPE_tempvar.subpos;
		tl_pos2beforebracket=TELESCOPE_tempvar.subpos2;
		tl_backvalbeforebracket=tlbackval;
		tl_bracketlevel=1;
		svg_text_bracket_back:;
		tl_bracket_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		if (tl_bracket_backval==0) goto svg_text_bracket_finished;
		tl_connect=((s_instance*)TELESCOPE_getproperty())->connect;
		if ((tl_connect & 4)>0) {tl_bracketlevel++;}
		if ((tl_connect & 2)>0) {tl_bracketlevel--;}
		if (tl_bracketlevel==0)
		{
			if ((tl_connect & 1)==0)
			{
				tl_bracketmode=0;
				tlbackval=tl_backvalbeforebracket;
				TELESCOPE_tempvar.subpos=tl_posbeforebracket;
				TELESCOPE_tempvar.subpos2=tl_pos2beforebracket;
				tl_connectmode=0;
				goto svg_text_back;
			}
			if (tl_bracketmode==2)
			{
				tlbackval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
				if (tlbackval==0) goto svg_text_bracket_finished;
				tl_bracketmode=1;
			}
			else
			{
				tlbackval=tl_backvalbeforebracket;
				TELESCOPE_tempvar.subpos=tl_posbeforebracket;
				TELESCOPE_tempvar.subpos2=tl_pos2beforebracket;
				tl_bracketmode=2;
				ifsmat=2;
			}
			goto svg_text_back;
		}
		goto svg_text_bracket_back;
		svg_text_bracket_finished:;
		tl_bracketmode=0;
		tlbackval=tl_backvalbeforebracket;
		TELESCOPE_tempvar.subpos=tl_posbeforebracket;
		TELESCOPE_tempvar.subpos2=tl_pos2beforebracket;
		tl_connectmode=0;
		goto svg_text_retryafterbracket;
	}
	if (tl_bracketmode>0) goto svg_text_bracket_finished;
	goto svg_text_back;
	svg_text_finished:
	MACRO_DRAWPREFIX(express_text_tail)();
	
	//TODO SUBJECT: TEXT ist completely broken now without a buffer to read it from.
	goto svg_main_loop;
	svg_main_moleculefill:
/*	{
		moleculefill_instance * tlmoleculefill=&((*glob_moleculefill_multilist)[index_in_buffer]);
		fprintf(outfile,"<path d=\" ");
		for (int ilv2=0;ilv2<(*tlmoleculefill).Points.count;ilv2++)
		{
			int iid=(*tlmoleculefill).Points.a[ilv2];
			int ilv3;
			for (ilv3=0;ilv3<(*glob_n_multilist).filllevel;ilv3++)
			{
				if ((*glob_n_multilist)[ilv3].id==iid)
				{
					goto ifertig;
				}
			}
			goto dontputthispoint;
			ifertig:
			;
			{
				n_instance * i_n_instance=&((*glob_n_multilist)[ilv3]);
				
				fprintf(outfile,"%c %f,%f ",(ilv2==0)?'M':'L',(*i_n_instance).xyz.x+SVG_currentshiftx,(*i_n_instance).xyz.y+SVG_currentshifty);
			}
			dontputthispoint:
			;
		}
		fprintf(outfile,"z \" style=\"fill:#%06X;color:none;\" opacity=\"0.9\"/>\n",(*tlmoleculefill).RGB);
	}*///TODO SUBJECT: MOLECULEFILL must also use a buffer to list its points.
	goto svg_main_loop;
	svg_main_end:
	;
}
void Lennard_hatch();

