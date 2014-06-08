#define CAMBRIDGECONV_EXISTSTHEN(MACROPARAM_TYPE,MACROPARAM_PARAM) \
if (AUTOSTRUCT_EXISTS(CAMBRIDGE_ ## MACROPARAM_TYPE ## _instance,(*tl_CAMBRIDGE_ ## MACROPARAM_TYPE ## _instance),MACROPARAM_PARAM))\
{\
	tl_ ## MACROPARAM_TYPE ## _instance.MACROPARAM_PARAM=(*tl_CAMBRIDGE_ ## MACROPARAM_TYPE ## _instance).MACROPARAM_PARAM;\
}
#define CAMBRIDGECONV_COLORCONV(MACROPARAM_TYPE) \
{\
	__label__ color_fertig;\
	if (AUTOSTRUCT_EXISTS(CAMBRIDGE_ ## MACROPARAM_TYPE ## _instance,(*tl_CAMBRIDGE_ ## MACROPARAM_TYPE ## _instance),color))\
	{\
		int tlcolor=(*tl_CAMBRIDGE_ ## MACROPARAM_TYPE ## _instance).color-2;\
		if (tlcolor==-2) {tl_ ## MACROPARAM_TYPE ## _instance.color=0x000000; goto color_fertig; }\
		if (tlcolor==-1) {tl_ ## MACROPARAM_TYPE ## _instance.color=0xFFFFFF; goto color_fertig; }\
		if (tlcolor<(*glob_CAMBRIDGE_color_multilist).filllevel)\
		{\
			tl_ ## MACROPARAM_TYPE ## _instance.color=((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].r*255)*65536)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].g*255)*256)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].b*255));\
		}\
		else\
		{\
			tl_ ## MACROPARAM_TYPE ## _instance.color=0x000000;\
		}\
	}\
	else\
	{\
		tl_ ## MACROPARAM_TYPE ## _instance.color=0x000000;\
	}\
	color_fertig:;\
}
CAMBRIDGE_font_instance * getfont(_small iid)
{
	for (int ilv1=0;ilv1<(*glob_CAMBRIDGE_font_multilist).filllevel;ilv1++)
	{
		if (((*glob_CAMBRIDGE_font_multilist).bufferlist[ilv1]).id==iid)
		{
			return &((*glob_CAMBRIDGE_font_multilist).bufferlist[ilv1]);
		}
	}
	return NULL;
}
void CAMBRIDGECONV_atom()
{
	multilist<CAMBRIDGE_n_instance> * tl_CAMBRIDGE_n_multilist=retrievemultilist<CAMBRIDGE_n_instance>();
	multilist<n_instance> * tl_n_multilist=retrievemultilist<n_instance>();
	(*tl_n_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_n_multilist).filllevel;ilv1++)
	{
		int ihv1;
		CAMBRIDGE_n_instance * tl_CAMBRIDGE_n_instance=(*tl_CAMBRIDGE_n_multilist).bufferlist+ilv1;
		multilistreference<CAMBRIDGE_t_instance> * tl_CAMBRIDGE_t_multilistreference=(multilistreference<CAMBRIDGE_t_instance>*)((*tl_CAMBRIDGE_n_instance).t);
		n_instance tl_n_instance;
		tl_n_instance=n_instance();
		tl_n_instance.Element=9;
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_n_instance,(*tl_CAMBRIDGE_n_instance),Element))
		{
			tl_n_instance.Element=(*tl_CAMBRIDGE_n_instance).Element;
			tl_n_instance.Element+=1;
			if ((tl_n_instance).Element>3)
			{
				(tl_n_instance).Element+=2;
			}
			if ((tl_n_instance).Element>9)
			{
				(tl_n_instance).Element++;
			}
		}
		for (int ilv2=(*tl_CAMBRIDGE_t_multilistreference).start_in_it;ilv2<(*tl_CAMBRIDGE_t_multilistreference).start_in_it+(*tl_CAMBRIDGE_t_multilistreference).count_in_it;ilv2++)
		{
			(*glob_CAMBRIDGE_t_multilist).bufferlist[ilv2].relN=(*glob_n_multilist).filllevel;
			tl_n_instance.Element=-1;
		}
		(tl_n_instance).protons=(*tl_CAMBRIDGE_n_instance).NumHydrogens;
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_n_instance,(*tl_CAMBRIDGE_n_instance),p))
		{
			tl_n_instance.xyz.x=(*tl_CAMBRIDGE_n_instance).p.x;
			tl_n_instance.xyz.y=(*tl_CAMBRIDGE_n_instance).p.y;
			tl_n_instance.xyz.z=0;
		}
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_n_instance,(*tl_CAMBRIDGE_n_instance),xyz))
		{
			tl_n_instance.xyz=(*tl_CAMBRIDGE_n_instance).xyz;
		}
		CAMBRIDGECONV_COLORCONV(n);
		//TODO: ExternalConnectionType, and respecting this enumerated property in draw
		tl_n_instance.id=(*tl_CAMBRIDGE_n_instance).id;
		tl_n_instance.Z=(*tl_CAMBRIDGE_n_instance).Z;
		(*tl_n_multilist).ADD(&tl_n_instance);
	}
}

void CAMBRIDGECONV_bond()
{
	multilist<CAMBRIDGE_b_instance> * tl_CAMBRIDGE_b_multilist=retrievemultilist<CAMBRIDGE_b_instance>();
	multilist<b_instance> * tl_b_multilist=retrievemultilist<b_instance>();
	(*tl_b_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_b_multilist).filllevel;ilv1++)
	{
		CAMBRIDGE_b_instance * tl_CAMBRIDGE_b_instance=(*tl_CAMBRIDGE_b_multilist).bufferlist+ilv1;
		b_instance tl_b_instance;
		tl_b_instance=b_instance();
		CAMBRIDGECONV_COLORCONV(b);
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),Display))
		{
			tl_b_instance.Display=(*tl_CAMBRIDGE_b_instance).Display;
		}
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),Display2))
		{
			tl_b_instance.Display2=(*tl_CAMBRIDGE_b_instance).Display2;
		}
		_u32 tl_outorder=0x10;
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),Order))
		{
			_u32 iOrder=(*tl_CAMBRIDGE_b_instance).Order;
			switch (iOrder & 0xFFF)
			{
				case 1: tl_outorder=0x10;break;
				case 2: tl_outorder=0x20;break;
				case 4: tl_outorder=0x30;break;
				case 8: tl_outorder=0x40;break;
				case 0x10: tl_outorder=0x50;break;
				case 0x20: tl_outorder=0x60;break;
				case 0x40: tl_outorder=0x8;break;
				case 0x80: tl_outorder=0x18;break;
				case 0x100: tl_outorder=0x28;break;
				case 0x200: tl_outorder=0x38;break;
				case 0x400: tl_outorder=0x48;break;
				case 0x800: tl_outorder=0x58;break;
			}
			if (iOrder & 0x1000)
			{
				if (tl_outorder<=0x10)
				{
					tl_b_instance.Display|=0x80;
				}
				else
				{
					tl_b_instance.Display2|=0x80;
				}
			}
		}
		tl_b_instance.Order=tl_outorder;
		tl_b_instance.B=(*tl_CAMBRIDGE_b_instance).B;
		tl_b_instance.E=(*tl_CAMBRIDGE_b_instance).E;
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),Z))
		{
			tl_b_instance.Z=(*tl_CAMBRIDGE_b_instance).Z;
		}
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),DoublePosition))
		{
			tl_b_instance.DoublePosition=(*tl_CAMBRIDGE_b_instance).DoublePosition;
		}
		else
		{
			tl_b_instance.DoublePosition=0;
		}
		(*tl_b_multilist).ADD(&tl_b_instance);
	}
}

void CAMBRIDGECONV_text()
{
	multilist<CAMBRIDGE_t_instance> * tl_CAMBRIDGE_t_multilist=retrievemultilist<CAMBRIDGE_t_instance>();
	multilist<CAMBRIDGE_s_instance> * tl_CAMBRIDGE_s_multilist=retrievemultilist<CAMBRIDGE_s_instance>();
	multilist<t_instance> * tl_t_multilist=retrievemultilist<t_instance>();
	multilist<n_instance> * tl_n_multilist=retrievemultilist<n_instance>();
	t_instance tl_t_instance;
	s_instance tl_s_instance;
	(*tl_t_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_t_multilist).filllevel;ilv1++)
	{
		CAMBRIDGE_t_instance * tl_CAMBRIDGE_t_instance=(*tl_CAMBRIDGE_t_multilist).bufferlist+ilv1;
		if ((*tl_CAMBRIDGE_t_instance).master!=NULL)
		{
			if (strcmp(((*tl_CAMBRIDGE_t_instance).master)->getName(),"n")==0)
			{
				CAMBRIDGE_n_instance * tl_CAMBRIDGE_n_instance=(CAMBRIDGE_n_instance*)((*tl_CAMBRIDGE_t_instance).master);
				TELESCOPE_aggressobject(tl_n_multilist,(*tl_CAMBRIDGE_t_instance).relN);
				goto i_n_only;
			}
		}
		tl_t_instance=t_instance();
		CAMBRIDGECONV_COLORCONV(t)
		CAMBRIDGECONV_EXISTSTHEN(t,xyz);
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_t_instance,(*tl_CAMBRIDGE_t_instance),p))
		{
			tl_t_instance.xyz.x=(*tl_CAMBRIDGE_t_instance).p.x;
			tl_t_instance.xyz.y=(*tl_CAMBRIDGE_t_instance).p.y;
			tl_t_instance.xyz.z=0;
		}
		CAMBRIDGECONV_EXISTSTHEN(t,id);
		CAMBRIDGECONV_EXISTSTHEN(t,Z);
		CAMBRIDGECONV_EXISTSTHEN(t,BoundingBox);
		CAMBRIDGECONV_EXISTSTHEN(t,Justification);
		CAMBRIDGECONV_EXISTSTHEN(t,LabelAlignment);
		CAMBRIDGECONV_EXISTSTHEN(t,color);
		CAMBRIDGECONV_EXISTSTHEN(t,IGOTYOU);
		(*tl_t_multilist).ADD(&tl_t_instance);
		TELESCOPE_aggressobject(tl_t_multilist,(*tl_t_multilist).filllevel-1);
		i_n_only:;
		for (int ilv2=(*((*tl_CAMBRIDGE_t_instance).s)).start_in_it;ilv2<(*((*tl_CAMBRIDGE_t_instance).s)).start_in_it+(*((*tl_CAMBRIDGE_t_instance).s)).count_in_it;ilv2++)
		{
			CAMBRIDGE_s_instance * tl_CAMBRIDGE_s_instance=(*tl_CAMBRIDGE_s_multilist).bufferlist+ilv2;
			tl_s_instance.font=(*tl_CAMBRIDGE_s_instance).font;
			tl_s_instance.face=(*tl_CAMBRIDGE_s_instance).face;
			tl_s_instance.size=(*tl_CAMBRIDGE_s_instance).size;
			CAMBRIDGECONV_COLORCONV(s)
			tl_s_instance.type=TELESCOPE_ELEMENTTYPE_s;
			if (strcmp((*getfont((*tl_CAMBRIDGE_s_instance).font)).name.a,"Symbol")==0)
			{
				char tl_char1[2];
				char * tl_text;
				tl_char1[1]=0;
				char tl_length=strlen((*tl_CAMBRIDGE_s_instance).PCTEXT.a);
				tl_s_instance.length=sizeof(tl_s_instance)+1;//trailing zero.
				for (int ilv1=0;ilv1<tl_length;ilv1++)
				{
					tl_char1[0]=(*tl_CAMBRIDGE_s_instance).PCTEXT.a[ilv1];
					tl_text=tl_char1;
					tl_text=get_list_greeklist((*tl_CAMBRIDGE_s_instance).PCTEXT.a+ilv1);
					if (tl_text==0)
					{
						tl_text=tl_char1;
					}
					if (ilv1==0)
					{
						TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s,tl_text,strlen(tl_text)+((ilv1+1==tl_length)?1:0));
					}
					else
					{
						TELESCOPE_insertintoproperties(TELESCOPE_ELEMENTTYPE_s,tl_text,strlen(tl_text)+((ilv1+1==tl_length)?1:0));
					}
					tl_s_instance.length+=strlen(tl_text)+((ilv1+1==tl_length)?1:0);
				}
			}
			else
			{
				tl_s_instance.length=sizeof(s_instance)+strlen((*tl_CAMBRIDGE_s_instance).PCTEXT.a)+1;//trailing 0
				TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s,(*tl_CAMBRIDGE_s_instance).PCTEXT.a,tl_s_instance.length-sizeof(s_instance));
			}
			*((s_instance*)TELESCOPE_getproperty())=tl_s_instance;
		}
	}
}
void CAMBRIDGECONV_graphic()
{
	multilist<CAMBRIDGE_graphic_instance> * tl_CAMBRIDGE_graphic_multilist=retrievemultilist<CAMBRIDGE_graphic_instance>();
	multilist<graphic_instance> * tl_graphic_multilist=retrievemultilist<graphic_instance>();
	(*tl_graphic_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_graphic_multilist).filllevel;ilv1++)
	{
		CAMBRIDGE_graphic_instance * tl_CAMBRIDGE_graphic_instance=(*tl_CAMBRIDGE_graphic_multilist).bufferlist+ilv1;
		graphic_instance tl_graphic_instance;
		tl_graphic_instance=graphic_instance();
		CAMBRIDGECONV_COLORCONV(graphic);
		CAMBRIDGECONV_EXISTSTHEN(graphic,BoundingBox);
		CAMBRIDGECONV_EXISTSTHEN(graphic,GraphicType);
		CAMBRIDGECONV_EXISTSTHEN(graphic,SymbolType);
		CAMBRIDGECONV_EXISTSTHEN(graphic,OvalType);
		CAMBRIDGECONV_EXISTSTHEN(graphic,RectangleType);
		CAMBRIDGECONV_EXISTSTHEN(graphic,LineType);
		CAMBRIDGECONV_EXISTSTHEN(graphic,BracketType);
		CAMBRIDGECONV_EXISTSTHEN(graphic,Center3D);
		CAMBRIDGECONV_EXISTSTHEN(graphic,MajorAxisEnd3D);
		CAMBRIDGECONV_EXISTSTHEN(graphic,MinorAxisEnd3D);
		CAMBRIDGECONV_EXISTSTHEN(graphic,id);//TODO: possibly not needed. Remove?
		CAMBRIDGECONV_EXISTSTHEN(graphic,Z);
		(*tl_graphic_multilist).ADD(&tl_graphic_instance);
	}
}
void CAMBRIDGECONV_arrow()
{
	multilist<CAMBRIDGE_arrow_instance> * tl_CAMBRIDGE_arrow_multilist=retrievemultilist<CAMBRIDGE_arrow_instance>();
	multilist<arrow_instance> * tl_arrow_multilist=retrievemultilist<arrow_instance>();
	(*tl_arrow_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_arrow_multilist).filllevel;ilv1++)
	{
		CAMBRIDGE_arrow_instance * tl_CAMBRIDGE_arrow_instance=(*tl_CAMBRIDGE_arrow_multilist).bufferlist+ilv1;
		arrow_instance tl_arrow_instance;
		tl_arrow_instance=arrow_instance();
		CAMBRIDGECONV_COLORCONV(arrow);
		CAMBRIDGECONV_EXISTSTHEN(arrow,Head3D);
		CAMBRIDGECONV_EXISTSTHEN(arrow,Tail3D);
		CAMBRIDGECONV_EXISTSTHEN(arrow,Center3D);
		CAMBRIDGECONV_EXISTSTHEN(arrow,LineType);
		CAMBRIDGECONV_EXISTSTHEN(arrow,AngularSize);
		CAMBRIDGECONV_EXISTSTHEN(arrow,MajorAxisEnd3D);
		CAMBRIDGECONV_EXISTSTHEN(arrow,MinorAxisEnd3D);
		CAMBRIDGECONV_EXISTSTHEN(arrow,ArrowheadType);
		CAMBRIDGECONV_EXISTSTHEN(arrow,ArrowheadHead);
		CAMBRIDGECONV_EXISTSTHEN(arrow,ArrowheadTail);
		CAMBRIDGECONV_EXISTSTHEN(arrow,ArrowShaftSpacing);
		CAMBRIDGECONV_EXISTSTHEN(arrow,Z);
//graphicType???
		(*tl_arrow_multilist).ADD(&tl_arrow_instance);
	}
}
#define WORKIFIX_REGISTERED_TRADEMARK_workthrough_variables\
	_u32 icompare;\
	int isize;\
	char * ibufferpos;\
	int ioffset;
#define WORKIFIX_REGISTERED_TRADEMARK_workthrough(MACROPARAM_PROPERTY)\
				for (int ilv1=0;ilv1<sizeof(STRUCTURE_OBJECTTYPE_List)/sizeof(trienum);ilv1++)\
				{\
					icompare=1<<ilv1;\
					int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;\
					basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);\
					if (tlmultilist==NULL) goto i_macrointernal_fertig;\
					CDXMLREAD_functype tldummy;\
					ioffset=(*tlmultilist).getproperties(# MACROPARAM_PROPERTY,&tldummy);\
					if (ioffset<0) goto i_macrointernal_fertig;\
					ibufferpos=(char*)((*tlmultilist).pointer);\
					cdx_Point2D * tlpoint2d;\
					for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)\
					{\
						if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)\
						{

void CAMBRIDGECONV_maintointernal()
{
	CAMBRIDGECONV_atom();//atoms before text, or some atoms get no label.
	CAMBRIDGECONV_bond();
	CAMBRIDGECONV_text();
	CAMBRIDGECONV_graphic();
	CAMBRIDGECONV_arrow();
}
