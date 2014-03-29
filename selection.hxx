typedef int selection_[20000];//a 32bit int gives selection masks for 32 layers. TODO: make this grow to the current buffer size.
selection_ currentselection;//The ordinary selection, more precisely the current state of it.
selection_ clickselection;//All elements caught with the current click, or another likewise action of the mouse.
void clearselection(selection_ iselection)
{
	for (int ilv1=0;ilv1<20000;ilv1++)
	{
		iselection[ilv1]=0;
	}
}
typedef struct selection_frame_
{
	int startx,starty;
	int endx,endy;
	float left,top;
	float right,bottom;
}selection_frame_;
selection_frame_ selection_frame;
_u32 selection_found;
