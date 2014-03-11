typedef int selection_[20000];//a 32bit int gives selection masks for 32 layers. TODO: make this grow to the current buffer size.
selection_ currentselection;//The ordinary selection, more precisely the current state of it.
selection_ clickselection;//All elements caught with the current click, or another likewise action of the mouse.
#define CLIBOARDINDEX_n_instance 1
#define CLIBOARDINDEX_b_instance 2
void clearselection(selection_ iselection)
{
	for (int ilv1=0;ilv1<20000;ilv1++)
	{
		iselection[ilv1]=0;
	}
}
int hit(n_instance * iinstance,float ix, float iy)
{
	if (sqrt(sqr(ix-(*iinstance).xyz.x)+sqr(iy-(*iinstance).xyz.y))<10)
	{
		return 1;
	}
	return 0;
}
int hit(b_instance * iinstance,float ix, float iy)
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

template <class thisinstance> void clickfor(int posx,int posy)
{
	multilist<thisinstance> * imultilist=retrievemultilist<thisinstance>();
	for (int ilv1=0;ilv1<(*imultilist).filllevel;ilv1++)
	{
		thisinstance * tlinstance=&((*imultilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if (hit(tlinstance,posx,posy))
			{
				clickselection[ilv1]|=(1<<(CLIBOARDINDEX_n_instance-1));
			}
		}
	}
}
