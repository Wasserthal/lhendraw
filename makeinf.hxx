#include <math.h>
typedef struct framestatus_
{
	int number;
	int posx,posy;
}framestatus_;
typedef struct frameinfo_
{
	int number;
	float posx,posy;
	float endx,endy;
	float width,height;
}frameinfo_;
typedef struct makeinf_sortiment_
{
	int sortiment_length;
	float height;
	float width;
	framestatus_ sortiment[255];
} makeinf_sortiment_;
makeinf_sortiment_ makeinf_sortiment;
makeinf_sortiment_ loadedmakeinf_sortiment;
FILE * INFfile;
FILE * infile;
frameinfo_ makeinf_frame[255];
int makeinf_frame_count;
int makeinf_sortimentcount;
float makeinf_width;
float makeinf_height;
long long makeinf_fileend;
char comparemakeinf_sortiment(makeinf_sortiment_ & in1, makeinf_sortiment_ & in2)
{
	if (in1.sortiment_length!=in2.sortiment_length)
	{
		return 0;
	}
	for (int ilv1=0;ilv1<in1.sortiment_length;ilv1++)
	{
		for (int ilv2=0;ilv2<in2.sortiment_length;ilv2++)
		{
			if (in1.sortiment[ilv1].number==in2.sortiment[ilv2].number)
			{
				goto ifound;
			}
		}
		return 0;
		ifound:
		;
	}
	return 1;
}
void addframe(cdx_Rectangle & irect,int nr)
{
	makeinf_frame[makeinf_frame_count].number=nr;
	makeinf_frame[makeinf_frame_count].posx=irect.left;
	makeinf_frame[makeinf_frame_count].posy=irect.top;
	makeinf_frame[makeinf_frame_count].endx=irect.right;
	makeinf_frame[makeinf_frame_count].endy=irect.bottom;
	makeinf_frame[makeinf_frame_count].width=irect.right-irect.left;
	makeinf_frame[makeinf_frame_count].height=irect.bottom-irect.top;
	makeinf_frame_count++;
}
#ifndef CDXMAKEINF_READONLY
void getframes()
{
	char isfound;
	int iframenr;
	for (int ilv1=0;ilv1<(*glob_graphic_multilist).filllevel;ilv1++)
	{
		if ((*glob_graphic_multilist).bufferlist[ilv1].GraphicType==3)
		{
			multilistreference<annotation_instance> * tlannotationmultilistreference=dynamic_cast<multilistreference<annotation_instance>*>((*glob_graphic_multilist).bufferlist[ilv1].annotation);
			isfound=0;
			for (int ilv2=0;ilv2<(*tlannotationmultilistreference).count_in_it;ilv2++)
			{
				annotation_instance * tlannotation=&((*glob_annotation_multilist).bufferlist[(*tlannotationmultilistreference).start_in_it+ilv2]);
				if (strcmp((*tlannotation).Keyword.a,"Name")==0)
				{
					if ((strncmp((*tlannotation).Content.a,"Frame",5))==0)
					{
						iframenr=atoi((*tlannotation).Content.a+5);
						isfound=1;
					}
					else
					{
						goto NameNotFrame;
					}
				}
			}
			if (isfound==1)
			{
				cdx_Rectangle * iBBX=&(((*glob_graphic_multilist).bufferlist[ilv1]).BoundingBox);
				canonicalizeboundingbox(iBBX);
				addframe(*iBBX,iframenr);
			}
		}
		NameNotFrame:
		;
	}
}
void svg_prescan()
{
	getcaptions(&SVG_width,&SVG_height,&SVG_ileft,&SVG_itop);
	SVG_ileft-=10;
	SVG_itop-=10;
	SVG_width+=20;
	SVG_height+=20;
	makeinf_width=SVG_width-SVG_ileft;
	makeinf_height=SVG_height-SVG_itop;
	getframes();
}

void scanfortext(FILE * fileforthatpurpose,float x,float y,float x2,float y2)
{
	for (int ilv1=0;ilv1<(*glob_t_multilist).filllevel;ilv1++)
	{
		int tlbestnr; float tlbestval;
		tlbestnr=-1;
		tlbestval=2000000000;
		for (int ilv2=0;ilv2<(*glob_t_multilist).filllevel;ilv2++)
		{
			t_instance * tl_t_instance=&((*glob_t_multilist).bufferlist[ilv2]);
			if (text_actual_node[ilv2].owner==-1)
			{
				if ((*tl_t_instance).IGOTYOU==0)
				{
					if (((*tl_t_instance).p.x>x) &&
					((*tl_t_instance).p.y>y) &&
					((*tl_t_instance).p.x<x2) &&
					((*tl_t_instance).p.y<y2))
					{
						float currentval=(*tl_t_instance).p.y*5+(*tl_t_instance).p.x;
						if (currentval<tlbestval)
						{
							tlbestval=currentval;
							tlbestnr=ilv2;
						}
					}
				}
			}
		}
		if (tlbestnr!=-1)
		{
			t_instance *tl_t_instance=&((*glob_t_multilist).bufferlist[tlbestnr]);
			(*tl_t_instance).IGOTYOU=1;
			multilistreference<s_instance> * tl_s_multilistreference=dynamic_cast<multilistreference<s_instance>*>((*tl_t_instance).s);
			for (int ilv3=(*tl_s_multilistreference).start_in_it;ilv3<(*tl_s_multilistreference).start_in_it+(*tl_s_multilistreference).count_in_it;ilv3++)
			{
				fprintf(fileforthatpurpose,"%s",(*glob_s_multilist).bufferlist[ilv3].PCTEXT.a);
			}
		}
	}
}
void scanfortext_frame(FILE * fileforthatpurpose,int inumber)
{
	frameinfo_ tlframeinfo;
	tlframeinfo=makeinf_frame[inumber];
	scanfortext(fileforthatpurpose,tlframeinfo.posx,tlframeinfo.posy,tlframeinfo.endx,tlframeinfo.endy);
}
void scanfortext_picture(FILE * fileforthatpurpose)
{
	for (int ilv1=0;ilv1<makeinf_sortiment.sortiment_length;ilv1++)
	{
		frameinfo_ tlframeinfo;
		for (int ilv2=0;ilv2<makeinf_frame_count;ilv2++)
		{
			if (makeinf_frame[ilv2].number==makeinf_sortiment.sortiment[ilv1].number)
			{
				tlframeinfo=makeinf_frame[ilv2];
			}
		}
		scanfortext(fileforthatpurpose,tlframeinfo.posx,tlframeinfo.posy,tlframeinfo.endx,tlframeinfo.endy);
	}
}
#endif
char truefilename[stringlength];
/*char tetrify()
{
	float currenty,currentmaxx;
	int rows;
	int currentnr;
	currenty=0;
	currentmaxx=0;
	rows=0;
	for (int ilv1=0;ilv1<makeinf_sortiment.sortiment_length;ilv1++)
	{
		makeinf_sortiment.sortiment[ilv1].posx=currentmaxx;
		makeinf_sortiment.sortiment[ilv1].posy=currenty;
		currentnr=makeinf_sortiment.sortiment[ilv1].number;
		for (int ilv2=0;ilv2<makeinf_frame_count;ilv2++)
		{
			if (makeinf_frame[ilv2]==currentnr)
			{
				currentnr=ilv2;
				goto ifertig;
			}
		}
		ifertig:
		currenty+=makeinf_frame[ilv1].height;
		maxup(currentmaxx,makeinf_frame[ilv1].width);
	}
}*/
void makeinf(const char * cdxname,const char * name)
{
	char tldowriteatend=makeinf_sortimentcount;
	strcpy(truefilename,name);
	strcat(truefilename,".inf");
	INFfile=fopen(truefilename,"r");
	if (INFfile!=NULL)
	{
		fread(&makeinf_frame_count,4,1,INFfile);
		fread(&makeinf_sortimentcount,4,1,INFfile);
		fread(&makeinf_width,sizeof(float),1,INFfile);
		fread(&makeinf_height,sizeof(float),1,INFfile);
		fread(&(makeinf_frame[0]),sizeof(frameinfo_),makeinf_frame_count,INFfile);//frameinfos are always read in.
		makeinf_fileend=8+makeinf_frame_count*sizeof(frameinfo_);
		for (int ilv1=0;ilv1<makeinf_sortimentcount;ilv1++)
		{
			fread(&loadedmakeinf_sortiment.sortiment_length,4,1,INFfile);
			fread(&loadedmakeinf_sortiment.width,sizeof(float),1,INFfile);
			fread(&loadedmakeinf_sortiment.height,sizeof(float),1,INFfile);
			makeinf_fileend+=4+2*sizeof(float);
			for (int ilv2=0;ilv2<loadedmakeinf_sortiment.sortiment_length;ilv2++)
			{
				fread(&(loadedmakeinf_sortiment.sortiment[ilv2]),sizeof(framestatus_),1,INFfile);
				makeinf_fileend+=sizeof(framestatus_);
			}
			if (comparemakeinf_sortiment(loadedmakeinf_sortiment,makeinf_sortiment))
			{
				tldowriteatend=0;
			}
		}
		fclose(INFfile);
	}
	#ifndef CDXMAKEINF_READONLY
	else
	{
		infile=fopen(cdxname,"r+");
		input_fsm(infile);
		fclose(infile);
		svg_findaround();
		getframes();
		getatoms();
		makeinf_width=SVG_width-SVG_ileft;
		makeinf_height=SVG_height-SVG_itop;
		printf("w%fŋ",makeinf_width);
		strcpy(truefilename,name);
		strcat(truefilename,".inf");
		INFfile=fopen(truefilename,"w");
		fwrite(&makeinf_frame_count,4,1,INFfile);
		fwrite(&makeinf_sortimentcount,4,1,INFfile);
		fwrite(&makeinf_width,sizeof(float),1,INFfile);
		fwrite(&makeinf_height,sizeof(float),1,INFfile);
		FILE * scraptext;
		if (makeinf_frame_count>0)
		{
			for (int ilv1=0;ilv1<makeinf_frame_count;ilv1++)
			{
				strcpy(truefilename,name);
				strcat(truefilename,"(");
				sprintf(truefilename+strlen(truefilename),"%i",makeinf_frame[ilv1].number);
				strcat(truefilename,")");
				strcat(truefilename,".txt");
				scraptext=fopen(truefilename,"w");
				scanfortext_frame(scraptext,ilv1);
				fclose(scraptext);
			}
		}
		else
		{
			strcpy(truefilename,name);
			strcat(truefilename,".txt");
			scraptext=fopen(truefilename,"w");
			scanfortext(scraptext,-INFINITY,-INFINITY,INFINITY,INFINITY);
			fclose(scraptext);
		}
		for (int ilv1=0;ilv1<makeinf_frame_count;ilv1++)
		{
			fwrite(&(makeinf_frame[ilv1]),sizeof(frameinfo_),1,INFfile);
			makeinf_fileend+=sizeof(frameinfo_);
		}
		fclose(INFfile);
	}
	#endif
	if (tldowriteatend==1)
	{
		strcpy(truefilename,name);
		strcat(truefilename,".inf");
		INFfile=fopen(truefilename,"r+");
		fseek(INFfile,0,SEEK_END);
		fwrite(&makeinf_sortiment.sortiment_length,4,1,INFfile);
		fwrite(&makeinf_sortiment.width,sizeof(float),1,INFfile);
		fwrite(&makeinf_sortiment.height,sizeof(float),1,INFfile);
		makeinf_fileend+=12;
		for (int ilv2=0;ilv2<makeinf_sortiment.sortiment_length;ilv2++)
		{
			fwrite(&(makeinf_sortiment.sortiment[ilv2]),sizeof(framestatus_),1,INFfile);
			makeinf_fileend+=sizeof(framestatus_);
		}
		fseek(INFfile,4L,SEEK_SET);
		makeinf_sortimentcount++;
		fwrite(&makeinf_sortimentcount,4,1,INFfile);
	}
}
#ifndef CDXMAKEINF_READONLY

void main_multisvg(int icount,char * * args)//tetrify
{
	makeinf_sortimentcount=0;
	makeinf_sortiment.height=0;
	makeinf_sortiment.width=0;
	for (int ilv1=0;ilv1<icount;ilv1++)
	{
		int tlnr=atoi(args[ilv1]);
		int tlnr2;
		for (int ilv2=0;ilv2<makeinf_frame_count;ilv2++)
		{
			if (makeinf_frame[ilv2].number==tlnr)
			{
				tlnr2=ilv2;
				goto ifertig;
			}
		}
		ifertig:
		;
		makeinf_sortiment.sortiment[makeinf_sortimentcount].number=tlnr;
		makeinf_sortiment.sortiment[makeinf_sortimentcount].posx=0;
		makeinf_sortiment.sortiment[makeinf_sortimentcount].posy=makeinf_sortiment.height;
		makeinf_sortiment.height+=makeinf_frame[tlnr2].height+10;
		makeinf_sortiment.width=fmax(makeinf_sortiment.width,makeinf_frame[tlnr2].width);
		makeinf_sortimentcount++;
	}
}

void Lennard_hatch()
{
	int ilv1;
	get_colorstring(0);
	stylegenestring(1);
	for (ilv1=0;ilv1<(*glob_graphic_multilist).filllevel;ilv1++)
	{
		graphic_instance * i_graphic_instance=(*glob_graphic_multilist).bufferlist+ilv1;
		if (((*i_graphic_instance).GraphicType==4) || ((*i_graphic_instance).GraphicType==5))
		{
			if ((*i_graphic_instance).OvalType==3)
			{
				if (((*i_graphic_instance).BoundingBox.right>SVG_currentbasex) && ((*i_graphic_instance).BoundingBox.bottom>SVG_currentbasey) &&
				((*i_graphic_instance).BoundingBox.right<SVG_currentfringex) && ((*i_graphic_instance).BoundingBox.bottom<SVG_currentfringey))
				{
					float tlradiussqr=sqr((*i_graphic_instance).BoundingBox.left-(*i_graphic_instance).BoundingBox.right)+sqr((*i_graphic_instance).BoundingBox.top-(*i_graphic_instance).BoundingBox.bottom);
					float tlradius=sqrt(tlradiussqr);
					HATCH_atom_count=0;
					for (int ilv2=0;ilv2<(*glob_n_multilist).filllevel;ilv2++)
					{
						n_instance * i_n_instance=(*glob_n_multilist).bufferlist+ilv2;
						if (sqr((*i_n_instance).p.x-(*i_graphic_instance).BoundingBox.right)+
	sqr((*i_n_instance).p.y-(*i_graphic_instance).BoundingBox.bottom)<tlradiussqr+600)
						{
							HATCH_add_atom(ilv2);
						}
					}
					HATCH_main((*i_graphic_instance).BoundingBox.right,(*i_graphic_instance).BoundingBox.bottom,tlradius,tlradius);
				}
			}
		}
	}
}

void svg_main2(const char * filename,int count,char * * args)
{
	char ifilename[stringlength];
	strcpy(ifilename,filename);
/*	strcat(ifilename,"(");
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		sprintf(ifilename+strlen(ifilename),(ilv1)?"+%s":"%s",args[ilv1]);
	}
	strcat(ifilename,")");*/
	main_multisvg(count,args);
	svg_head(filename,makeinf_sortiment.width,makeinf_sortiment.height);
	initZlist();
	#ifdef LENNARD_HACK
	char tlKLUDGEdontenumerate=0;
	for (int ilv1=0;ilv1<(*glob_annotation_multilist).filllevel;ilv1++)// a Kludge
	{
		annotation_instance * tlannotation=&((*glob_annotation_multilist).bufferlist[ilv1]);
		if (strncmp((*tlannotation).Content.a,"unnumbered",10)==0)
		{
			tlKLUDGEdontenumerate=1;
		}
	}
	#endif
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		int tlnr=atoi(args[ilv1]);
		int tlothernr;
		for (int ilv2=0;ilv2<makeinf_frame_count;ilv2++)
		{
			if (makeinf_frame[ilv2].number==tlnr)
			{
				tlnr=ilv2;
				goto ifertig;
			}
		}
		ifertig:
		SVG_currentbasex=makeinf_frame[tlnr].posx;
		SVG_currentbasey=makeinf_frame[tlnr].posy;
		SVG_currentshiftx=makeinf_sortiment.sortiment[ilv1].posx-SVG_currentbasex;
		SVG_currentshifty=makeinf_sortiment.sortiment[ilv1].posy-SVG_currentbasey;
		SVG_currentfringex=makeinf_frame[tlnr].endx;
		SVG_currentfringey=makeinf_frame[tlnr].endy;
		#ifdef LENNARD_HACK
		Lennard_hatch();
		#endif
		svg_controlprocedure(1,1);
		svg_controlprocedure(1,0);
		#ifdef LENNARD_HACK
		if ((count>1) && (!tlKLUDGEdontenumerate))
		{
			fprintf(outfile,"<text fill=\"#000000\" stroke=\"none\" transform=\"translate(%f,%f)\" font-size=\"36\" font-weight=\"bold\"><tspan>%c)</tspan></text>",(float)makeinf_sortiment.sortiment[ilv1].posx,(float)makeinf_sortiment.sortiment[ilv1].posy+30,'a'+ilv1);
		}
		#endif
	}
	svg_tail();
}
#endif
