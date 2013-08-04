typedef struct framestatus_
{
	int number;
	int posx,posy;
}framestatus_;
typedef struct frameinfo_
{
	int number;
	int posx,posy;
	int endx,endy;
	int width,height;
}frameinfo_;
typedef struct makeinf_sortiment_
{
	int sortiment_length;
	int height;
	int width;
	framestatus_ sortiment[255];
} makeinf_sortiment_;
makeinf_sortiment_ makeinf_sortiment;
makeinf_sortiment_ loadedmakeinf_sortiment;
FILE * INFfile;
FILE * infile;
frameinfo_ makeinf_frame[255];
int makeinf_frame_count;
int makeinf_sortimentcount;
int makeinf_width;
int makeinf_height;
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
}
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
			for (int ilv1=0;ilv1<(*tlannotationmultilistreference).count_in_it;ilv1++)
			{
				annotation_instance * tlannotation=&((*glob_annotation_multilist).bufferlist[(*tlannotationmultilistreference).start_in_it+ilv1]);
				if (strcmp((*tlannotation).Keyword.a,"Name")==0)
				{
					if ((strncmp((*tlannotation).Content.a,"Frame",5))==0)
					{
						iframenr=atoi((*tlannotation).Content.a+5);
						isfound=1;
					}
				}
			}
			if (isfound==1)
			{
				cdx_Rectangle * iBBX=&(((*glob_graphic_multilist).bufferlist[ilv1]).BoundingBox);
				addframe(*iBBX,iframenr);
			}
		}
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
void makeinf(const char * cdxname,const char * name)
{
	char tldowriteatend=makeinf_sortimentcount;
	INFfile=fopen(name,"r");
	if (INFfile!=NULL)
	{
		fread(&makeinf_frame_count,4,1,INFfile);
		fread(&makeinf_sortimentcount,4,1,INFfile);
		fread(&makeinf_width,4,1,INFfile);
		fread(&makeinf_height,4,1,INFfile);
		fread(&(makeinf_frame[0]),sizeof(frameinfo_),makeinf_frame_count,INFfile);//frameinfos are always read in.
		makeinf_fileend=8+makeinf_frame_count*sizeof(frameinfo_);
		for (int ilv1=0;ilv1<makeinf_sortimentcount;ilv1++)
		{
			fread(&loadedmakeinf_sortiment.sortiment_length,4,1,INFfile);
			fread(&loadedmakeinf_sortiment.width,4,1,INFfile);
			fread(&loadedmakeinf_sortiment.height,4,1,INFfile);
			makeinf_fileend+=12;
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
	else
	{
		INFfile=fopen(name,"w");
		fwrite(&makeinf_frame_count,4,1,INFfile);
		fwrite(&makeinf_sortimentcount,4,1,INFfile);
		fwrite(&makeinf_width,4,1,INFfile);
		fwrite(&makeinf_height,4,1,INFfile);
		infile=fopen(cdxname,"r+");
		input_fsm(infile);
		fclose(infile);
		getframes();
		for (int ilv1=0;ilv1<makeinf_frame_count;ilv1++)
		{
			fwrite(&(makeinf_frame[ilv1]),sizeof(frameinfo_),1,INFfile);
			makeinf_fileend+=sizeof(frameinfo_);
		}
		fclose(INFfile);
	}
	if (tldowriteatend==1)
	{
		INFfile=fopen(name,"r+");
		fseek(INFfile,0,SEEK_END);
		fwrite(&makeinf_sortiment.sortiment_length,4,1,INFfile);
		fwrite(&makeinf_sortiment.width,4,1,INFfile);
		fwrite(&makeinf_sortiment.height,4,1,INFfile);
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

void main_multisvg(void)
{
	
}
