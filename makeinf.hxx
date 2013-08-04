typedef struct framestatus_
{
	int posx,posy;
	int number;
}framestatus_;
typedef struct picturesortiment_
{
	int sortiment_length;
	int height;
	int width;
	framestatus_ sortiment[255];
} picturesortiment_;
picturesortiment_ picturesortiment;
picturesortiment_ loadedpicturesortiment;
FILE * INFfile;
int makeinf_framecount;
int makeinf_sortimentcount;
int makeinf_width;
int makeinf_height;
long long makeinf_fileend;
char comparepicturesortiment(picturesortiment_ & in1, picturesortiment_ & in2)
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
void makeinf(const char * name)
{
	char tldowriteatend=makeinf_sortimentcount;
	INFfile=fopen(name,"r");
	if (INFfile!=NULL)
	{
		fread(&makeinf_framecount,4,1,INFfile);
		fread(&makeinf_sortimentcount,4,1,INFfile);
		fread(&makeinf_width,4,1,INFfile);
		fread(&makeinf_height,4,1,INFfile);

		makeinf_fileend=8;
		for (int ilv1=0;ilv1<makeinf_sortimentcount;ilv1++)
		{
			fread(&loadedpicturesortiment.sortiment_length,4,1,INFfile);
			fread(&loadedpicturesortiment.width,4,1,INFfile);
			fread(&loadedpicturesortiment.height,4,1,INFfile);
			makeinf_fileend+=12;
			for (int ilv2=0;ilv2<loadedpicturesortiment.sortiment_length;ilv2++)
			{
				fread(&(loadedpicturesortiment.sortiment[ilv2]),12,1,INFfile);
				makeinf_fileend+=12;
			}
			if (comparepicturesortiment(loadedpicturesortiment,picturesortiment))
			{
				tldowriteatend=0;
			}
		}
		fclose(INFfile);
	}
	else
	{
		INFfile=fopen(name,"w");
		fwrite(&makeinf_framecount,4,1,INFfile);
		fwrite(&makeinf_sortimentcount,4,1,INFfile);
		fwrite(&makeinf_width,4,1,INFfile);
		fwrite(&makeinf_height,4,1,INFfile);
		if (makeinf_sortimentcount==1)
		{
			
		}
		fclose(INFfile);
	}
	if (tldowriteatend==1)
	{
		INFfile=fopen(name,"r+");
		fseek(INFfile,0,SEEK_END);
		fwrite(&picturesortiment.sortiment_length,4,1,INFfile);
		fwrite(&picturesortiment.width,4,1,INFfile);
		fwrite(&picturesortiment.height,4,1,INFfile);
		makeinf_fileend+=12;
		for (int ilv2=0;ilv2<picturesortiment.sortiment_length;ilv2++)
		{
			fwrite(&(picturesortiment.sortiment[ilv2]),12,1,INFfile);
			makeinf_fileend+=12;
		}
		fseek(INFfile,4L,SEEK_SET);
		makeinf_sortimentcount++;
		fwrite(&makeinf_sortimentcount,4,1,INFfile);
	}
}

void main_multisvg(void)
{
	
}
