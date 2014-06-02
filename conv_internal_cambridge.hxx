//Converts the internal format to the cambridge structure both for cdx as cdxml.
//Note: The created CAMBRIDGE structure is created as IMMUTABLE structure, for example, pointers in in may point to other buffers which may not be changed just in order to change the CAMBRIDGE structure.
//If these data changes because the other datastructure (usually the corresponding internal one) changes, they go out of scope, so the CAMBRIDGE structure is temporary only.
#define CONVCAMBRIDGE_COLORCONV(AUTOPARM) \
{\
	int tl_color;\
	unsigned char r,g,b;\
	AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_ ## AUTOPARM ## _instance,color);\
	tl_color=(*tl_ ## AUTOPARM ## _instance).color;\
	(*tl_CAMBRIDGE_ ## AUTOPARM ## _instance).color=0;\
	for (int ilv1=0;ilv1<(*glob_CAMBRIDGE_color_multilist).filllevel;ilv1++)\
	{\
		r=(tl_color&0xFF0000)>>16;\
		g=(tl_color&0xFF00)>>8;\
		b=(tl_color&0xFF);\
		if ((((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).r*255))==r) && \
		(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).g*255))==g) && \
		(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).b*255))==b))\
		{\
			(*tl_CAMBRIDGE_ ## AUTOPARM ## _instance).color=ilv1+2;\
		}\
	}\
}
void CONVCAMBRIDGE_atoms(CAMBRIDGE_fragment_instance * master,cdx_Rectangle * iBoundingBox)
{
	_u32 icompare=1<<STRUCTURE_OBJECTTYPE_n;
	multilist<CAMBRIDGE_n_instance> * tl_CAMBRIDGE_n_multilist=retrievemultilist<CAMBRIDGE_n_instance>();
	multilist<n_instance> * tl_n_multilist=retrievemultilist<n_instance>();
	for (int ilv1=0;ilv1<(*tl_n_multilist).filllevel;ilv1++)
	{
		if (selection_fragmentselection[ilv1] & icompare)
		{
			n_instance * tl_n_instance=(*tl_n_multilist).bufferlist+ilv1;
			ADD_TO_MULTILISTREFERENCE(master,n);
			CONVCAMBRIDGE_COLORCONV(n);
			(*tl_CAMBRIDGE_n_instance).id=(*tl_n_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,id);
			(*tl_CAMBRIDGE_n_instance).Z=(*tl_n_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,Z);
			(*tl_CAMBRIDGE_n_instance).xyz=(*tl_n_instance).xyz;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,xyz);
			if (iBoundingBox!=NULL)
			{
				if ((*tl_CAMBRIDGE_n_instance).xyz.x<(*iBoundingBox).left){(*iBoundingBox).left=(*tl_CAMBRIDGE_n_instance).xyz.x;}
				if ((*tl_CAMBRIDGE_n_instance).xyz.x>(*iBoundingBox).right){(*iBoundingBox).right=(*tl_CAMBRIDGE_n_instance).xyz.x;}
				if ((*tl_CAMBRIDGE_n_instance).xyz.y<(*iBoundingBox).top){(*iBoundingBox).top=(*tl_CAMBRIDGE_n_instance).xyz.y;}
				if ((*tl_CAMBRIDGE_n_instance).xyz.y>(*iBoundingBox).bottom){(*iBoundingBox).bottom=(*tl_CAMBRIDGE_n_instance).xyz.y;}
			}
		}
	}
}

void CONVCAMBRIDGE_bonds(CAMBRIDGE_fragment_instance * master)
{
	_u32 icompare=1<<STRUCTURE_OBJECTTYPE_b;
	multilist<CAMBRIDGE_b_instance> * tl_CAMBRIDGE_b_multilist=retrievemultilist<CAMBRIDGE_b_instance>();
	multilist<b_instance> * tl_b_multilist=retrievemultilist<b_instance>();
	for (int ilv1=0;ilv1<(*tl_b_multilist).filllevel;ilv1++)
	{
		if (selection_fragmentselection[ilv1] & icompare)
		{
			b_instance * tl_b_instance=(*tl_b_multilist).bufferlist+ilv1;
			ADD_TO_MULTILISTREFERENCE(master,b);
			CONVCAMBRIDGE_COLORCONV(b);
			(*tl_CAMBRIDGE_b_instance).id=(*tl_b_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,id);
			(*tl_CAMBRIDGE_b_instance).Z=(*tl_b_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,Z);
			(*tl_CAMBRIDGE_b_instance).B=(*tl_b_instance).B;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,B);
			(*tl_CAMBRIDGE_b_instance).E=(*tl_b_instance).E;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,E);
			_u8 iOrder=(*tl_b_instance).Order;
			_i32 iOrderout=1;
if (iOrder>8){ if (iOrder>16) {if (iOrder>24) {if (iOrder>32) {if (iOrder>40) {if (iOrder>48) {if (iOrder>56) {if (iOrder>64) {if (iOrder>72) {if (iOrder>80) {if (iOrder>88) {
iOrderout=0x20;}else iOrderout=0x800;}else iOrderout=0x10;}else iOrderout=0x400;}else iOrderout=0x8;}else iOrderout=0x200;}else iOrderout=0x4;}else iOrderout=0x100;}else 
iOrderout=0x2;}else iOrderout=0x80;}else iOrderout=0x1;}else iOrderout=0x40;
			(*tl_CAMBRIDGE_b_instance).Display=(*tl_b_instance).Display & 0x7F;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,Display);
			if (iOrder>16)
			{
				if ((*tl_b_instance).Display2 & 0x80)
				{
					iOrderout|=0x1000;//ChemDraw cannot process orred dative
				}
				(*tl_CAMBRIDGE_b_instance).Display2=(*tl_b_instance).Display2 & 0x7F;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,Display2);
			}
			if ((iOrder<=16))
			{
				if ((*tl_b_instance).Display & 0x80)
				{
					iOrderout=0x1000;//which is why it gets omitted at least here.
				}
			}
			(*tl_CAMBRIDGE_b_instance).Order=iOrderout;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,Order);
			(*tl_CAMBRIDGE_b_multilist).ADD(tl_CAMBRIDGE_b_instance);
		}
	}
}

void CONVCAMBRIDGE_s(CAMBRIDGE_t_instance * master,s_instance * tl_s_instance,char * CAMBRIDGE_s_instance_buffer)
{
	char backval=0;
	multilist<CAMBRIDGE_s_instance> * tl_CAMBRIDGE_s_multilist=retrievemultilist<CAMBRIDGE_s_instance>();
	ADD_TO_MULTILISTREFERENCE(master,s);
	CONVCAMBRIDGE_COLORCONV(s);
	(*tl_CAMBRIDGE_s_instance).font=(*tl_s_instance).font;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,font);
	(*tl_CAMBRIDGE_s_instance).face=(*tl_s_instance).face;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,face);
	(*tl_CAMBRIDGE_s_instance).size=(*tl_s_instance).size;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,size);
	(*tl_CAMBRIDGE_s_instance).PCTEXT.a=CAMBRIDGE_s_instance_buffer;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,PCTEXT);
	(*tl_CAMBRIDGE_s_instance).PCTEXTcounter=strlen((*tl_CAMBRIDGE_s_instance).PCTEXT.a);
	(*tl_CAMBRIDGE_s_multilist).ADD(tl_CAMBRIDGE_s_instance);
}
void CONVCAMBRIDGE_text(CAMBRIDGE_page_instance * master)
{
	char backval=0;
	multilist<CAMBRIDGE_t_instance> * tl_CAMBRIDGE_t_multilist=retrievemultilist<CAMBRIDGE_t_instance>();
	multilist<t_instance> * tl_t_multilist=retrievemultilist<t_instance>();
	for (int ilv1=0;ilv1<(*tl_t_multilist).filllevel;ilv1++)
	{
		t_instance * tl_t_instance=(*tl_t_multilist).bufferlist+ilv1;
		
		ADD_TO_MULTILISTREFERENCE(master,t);
		CONVCAMBRIDGE_COLORCONV(t);
		TELESCOPE_aggressobject(tl_t_multilist,ilv1);
		backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
		while (backval)
		{
			s_instance * nd_s_instance=(s_instance*)TELESCOPE_getproperty();
			CONVCAMBRIDGE_s(tl_CAMBRIDGE_t_instance,(s_instance*)TELESCOPE_getproperty(),(char*)TELESCOPE_getproperty_contents());
			backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		}
		CONVCAMBRIDGE_COLORCONV(t);
		(*tl_CAMBRIDGE_t_instance).BoundingBox=(*tl_t_instance).BoundingBox;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,BoundingBox);
		(*tl_CAMBRIDGE_t_instance).Justification=(*tl_t_instance).Justification;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,Justification);
		(*tl_CAMBRIDGE_t_instance).LabelAlignment=(*tl_t_instance).LabelAlignment;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,LabelAlignment);
		(*tl_CAMBRIDGE_t_instance).p.x=(*tl_t_instance).xyz.x;//BACKWARDS COMPATIBILITY
		(*tl_CAMBRIDGE_t_instance).p.y=(*tl_t_instance).xyz.y;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,p);//BACKWARDS COMPATIBILITY
		(*tl_CAMBRIDGE_t_instance).xyz=(*tl_t_instance).xyz;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,xyz);
		(*tl_CAMBRIDGE_t_instance).IGOTYOU=(*tl_t_instance).IGOTYOU;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,IGOTYOU);
		(*tl_CAMBRIDGE_t_instance).id=(*tl_t_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,id);
		(*tl_CAMBRIDGE_t_instance).Z=(*tl_t_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,Z);
		(*tl_CAMBRIDGE_t_multilist).ADD(tl_CAMBRIDGE_t_instance);
	}
}

void CONVCAMBRIDGE_fragments(CAMBRIDGE_page_instance * master)
{
	int iatomnr=0;
	multilist<CAMBRIDGE_fragment_instance> * tl_CAMBRIDGE_fragment_multilist=retrievemultilist<CAMBRIDGE_fragment_instance>();
	selection_clearselection(selection_clickselection);
	selection_clickselection_found=0;
	iback:
	selection_clearselection(selection_fragmentselection);
	selection_fragmentselection_found=0;
	iatomnr=findunfragmentedatom();
	if (iatomnr!=-1)
	{
		select_fragment_by_atom(iatomnr);

		ADD_TO_MULTILISTREFERENCE(master,fragment);
		tl_CAMBRIDGE_fragment_instance->BoundingBox.left=32767;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_fragment_instance,BoundingBox);
		tl_CAMBRIDGE_fragment_instance->BoundingBox.right=0;
		tl_CAMBRIDGE_fragment_instance->BoundingBox.top=32767;
		tl_CAMBRIDGE_fragment_instance->BoundingBox.bottom=0;
		CONVCAMBRIDGE_atoms(tl_CAMBRIDGE_fragment_instance,&(tl_CAMBRIDGE_fragment_instance->BoundingBox));
		CONVCAMBRIDGE_bonds(tl_CAMBRIDGE_fragment_instance);
		goto iback;
	}
}

void CONVCAMBRIDGE_arrow(CAMBRIDGE_page_instance * master)
{
	multilist<CAMBRIDGE_arrow_instance> * tl_CAMBRIDGE_arrow_multilist=retrievemultilist<CAMBRIDGE_arrow_instance>();
	multilist<arrow_instance> * tl_arrow_multilist=retrievemultilist<arrow_instance>();
	for (int ilv1=0;ilv1<(*tl_arrow_multilist).filllevel;ilv1++)
	{
		arrow_instance * tl_arrow_instance=(*tl_arrow_multilist).bufferlist+ilv1;
		ADD_TO_MULTILISTREFERENCE(master,arrow);
		CONVCAMBRIDGE_COLORCONV(arrow);
		(*tl_CAMBRIDGE_arrow_instance).Head3D=(*tl_arrow_instance).Head3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,Head3D);
		(*tl_CAMBRIDGE_arrow_instance).Tail3D=(*tl_arrow_instance).Tail3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,Tail3D);
		(*tl_CAMBRIDGE_arrow_instance).Center3D=(*tl_arrow_instance).Center3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,Center3D);
		(*tl_CAMBRIDGE_arrow_instance).LineType=(*tl_arrow_instance).LineType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,LineType);
		(*tl_CAMBRIDGE_arrow_instance).AngularSize=(*tl_arrow_instance).AngularSize;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,AngularSize);
		(*tl_CAMBRIDGE_arrow_instance).MajorAxisEnd3D=(*tl_arrow_instance).MajorAxisEnd3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,MajorAxisEnd3D);
		(*tl_CAMBRIDGE_arrow_instance).MinorAxisEnd3D=(*tl_arrow_instance).MinorAxisEnd3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,MinorAxisEnd3D);
		(*tl_CAMBRIDGE_arrow_instance).ArrowheadType=(*tl_arrow_instance).ArrowheadType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,ArrowheadType);
		(*tl_CAMBRIDGE_arrow_instance).ArrowheadHead=(*tl_arrow_instance).ArrowheadHead;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,ArrowheadHead);
		(*tl_CAMBRIDGE_arrow_instance).ArrowheadTail=(*tl_arrow_instance).ArrowheadTail;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,ArrowheadTail);
		(*tl_CAMBRIDGE_arrow_instance).ArrowShaftSpacing=(*tl_arrow_instance).ArrowShaftSpacing;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,ArrowShaftSpacing);
		(*tl_CAMBRIDGE_arrow_instance).Z=(*tl_arrow_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,Z);
		(*tl_CAMBRIDGE_arrow_multilist).ADD(tl_CAMBRIDGE_arrow_instance);
	}
}
void CONVCAMBRIDGE_graphic(CAMBRIDGE_page_instance * master)
{
	multilist<CAMBRIDGE_graphic_instance> * tl_CAMBRIDGE_graphic_multilist=retrievemultilist<CAMBRIDGE_graphic_instance>();
	multilist<graphic_instance> * tl_graphic_multilist=retrievemultilist<graphic_instance>();
	for (int ilv1=0;ilv1<(*tl_graphic_multilist).filllevel;ilv1++)
	{
		graphic_instance * tl_graphic_instance=(*tl_graphic_multilist).bufferlist+ilv1;
		ADD_TO_MULTILISTREFERENCE(master,graphic);
		CONVCAMBRIDGE_COLORCONV(graphic);
		(*tl_CAMBRIDGE_graphic_instance).BoundingBox=(*tl_graphic_instance).BoundingBox;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,BoundingBox);
		(*tl_CAMBRIDGE_graphic_instance).GraphicType=(*tl_graphic_instance).GraphicType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,GraphicType);
		(*tl_CAMBRIDGE_graphic_instance).SymbolType=(*tl_graphic_instance).SymbolType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,SymbolType);
		(*tl_CAMBRIDGE_graphic_instance).OvalType=(*tl_graphic_instance).OvalType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,OvalType);
		(*tl_CAMBRIDGE_graphic_instance).RectangleType=(*tl_graphic_instance).RectangleType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,RectangleType);
		(*tl_CAMBRIDGE_graphic_instance).LineType=(*tl_graphic_instance).LineType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,LineType);
		(*tl_CAMBRIDGE_graphic_instance).BracketType=(*tl_graphic_instance).BracketType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,BracketType);
		(*tl_CAMBRIDGE_graphic_instance).Center3D=(*tl_graphic_instance).Center3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,Center3D);
		(*tl_CAMBRIDGE_graphic_instance).MajorAxisEnd3D=(*tl_graphic_instance).MajorAxisEnd3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,MajorAxisEnd3D);
		(*tl_CAMBRIDGE_graphic_instance).MinorAxisEnd3D=(*tl_graphic_instance).MinorAxisEnd3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,MinorAxisEnd3D);
		(*tl_CAMBRIDGE_graphic_instance).id=(*tl_graphic_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,id);
		(*tl_CAMBRIDGE_graphic_instance).Z=(*tl_graphic_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,Z);
		(*tl_CAMBRIDGE_graphic_multilist).ADD(tl_CAMBRIDGE_graphic_instance);
	}
}

void CONVCAMBRIDGE_internaltomain(CAMBRIDGE_page_instance * master)
{
	filestructure_text_buffer.count=0;
	CONVCAMBRIDGE_fragments(master);
	CONVCAMBRIDGE_text(master);
	CONVCAMBRIDGE_arrow(master);
	CONVCAMBRIDGE_graphic(master);
}
