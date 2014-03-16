int hit(n_instance * iinstance,float ix,float iy)
{
	if (sqrt(sqr(ix-(*iinstance).xyz.x)+sqr(iy-(*iinstance).xyz.y))<10)
	{
		return 1;
	}
	return 0;
}
int hit(b_instance * iinstance,float ix,float iy)
{
	n_instance *iinstance1=0;
	n_instance *iinstance2=0;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if ((*tlinstance).id==(*iinstance).B)
			{
				iinstance1=tlinstance;
			}
			if ((*tlinstance).id==(*iinstance).E)
			{
				iinstance2=tlinstance;
			}
		}
	}
	if (iinstance1==NULL) {return -3;}
	if (iinstance2==NULL) {return -3;}
	if (sqrt(sqr(ix-((*iinstance1).xyz.x+(*iinstance2).xyz.x)*0.5)+sqr(iy-((*iinstance1).xyz.y+(*iinstance1).xyz.y)*0.5))<10)
	{
		return 1;
	}
	return 0;
}
template <class thisinstance> inline void clickfor_template(float posx,float posy,int objecttype)
{
	multilist<thisinstance> * imultilist=retrievemultilist<thisinstance>();
	for (int ilv1=0;ilv1<(*imultilist).filllevel;ilv1++)
	{
		thisinstance * tlinstance=&((*imultilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if (hit(tlinstance,posx,posy)>0)
			{
				clickselection[ilv1]|=(1<<objecttype);
			}
		}
	}
}
inline int retrievepoints(n_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>0) return 0;
	(*ix)=(*iinstance).xyz.x;
	(*iy)=(*iinstance).xyz.y;
	return 1;
}
inline int retrievepoints(b_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>0) return 0;
	n_instance *iinstance1=0;
	n_instance *iinstance2=0;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if ((*tlinstance).id==(*iinstance).B)
			{
				iinstance1=tlinstance;
			}
			if ((*tlinstance).id==(*iinstance).E)
			{
				iinstance2=tlinstance;
			}
		}
	}
	if (iinstance1==NULL) {return -3;}
	if (iinstance2==NULL) {return -3;}
	(*ix)=((*iinstance1).xyz.x+(*iinstance2).xyz.x)*0.5;
	(*iy)=((*iinstance1).xyz.y+(*iinstance1).xyz.y)*0.5;
	return 1;
}

#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return retrievepoints((whatabout ## _instance*)iinstance,ix,iy,inumber);break;
int retrievepoints_basic(basic_instance * iinstance,float * ix,float * iy,int inumber,int objecttype)
{
	switch (objecttype)
	{
		LOCALMACRO_1(n)
		LOCALMACRO_1(b)
	}
	return 0;
}
#undef LOCALMACRO_1
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return clickfor_template<whatabout ## _instance>(ix,iy,objecttype);break;
void clickfor(float ix,float iy,int objecttype)
{
	switch (objecttype)
	{
		LOCALMACRO_1(n)
		LOCALMACRO_1(b)
	}
}
#undef LOCALMACRO_1

template <class whatabout> inline int retrievepoints_relay(whatabout * iinstance,float * ix,float * iy,int inumber)//template may become unnecessary
{
	return retrievepoints(iinstance,ix,iy,inumber);
}
