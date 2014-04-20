void CAMBRIDGECONV_atoms()
{
	multilist<CAMBRIDGE_n_instance> * tl_CAMBRIDGE_n_multilist=retrievemultilist<CAMBRIDGE_n_instance>();
	multilist<n_instance> * tl_n_multilist=retrievemultilist<n_instance>();
	(*tl_n_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_n_multilist).filllevel;ilv1++)
	{
		int ihv1;
		CAMBRIDGE_n_instance * tl_CAMBRIDGE_n_instance=(*tl_CAMBRIDGE_n_multilist).bufferlist+ilv1;
		
		n_instance tl_n_instance;
		tl_n_instance=n_instance();
		tl_n_instance.Element=9;
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_n_instance,(*tl_CAMBRIDGE_n_instance),Element))
		{
			tl_n_instance.Element=(*tl_CAMBRIDGE_n_instance).Element;
		}
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
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_n_instance,(*tl_CAMBRIDGE_n_instance),color))
		{
			int tlcolor=(*tl_CAMBRIDGE_n_instance).color-2;
			if (tlcolor==-2) {tl_n_instance.color=0x000000; goto color_fertig; }
			if (tlcolor==-1) {tl_n_instance.color=0xFFFFFF; goto color_fertig; }
			if (tlcolor<(*glob_CAMBRIDGE_color_multilist).filllevel)
			{
				tl_n_instance.color=((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].r*255)*65536)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].g*255)*256)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].b*255));
			}
			else
			{
				tl_n_instance.color=0x000000;
			}
		}
		else
		{
			tl_n_instance.color=0x000000;
		}
		color_fertig:
		//TODO: ExternalConnectionType, and respecting this enumerated property in draw
		tl_n_instance.id=(*tl_CAMBRIDGE_n_instance).id;
		tl_n_instance.Z=(*tl_CAMBRIDGE_n_instance).Z;
		(*tl_n_multilist).ADD(&tl_n_instance);
	}
}

void CAMBRIDGECONV_bonds()
{
	multilist<CAMBRIDGE_b_instance> * tl_CAMBRIDGE_b_multilist=retrievemultilist<CAMBRIDGE_b_instance>();
	multilist<b_instance> * tl_b_multilist=retrievemultilist<b_instance>();
	(*tl_b_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_b_multilist).filllevel;ilv1++)
	{
		CAMBRIDGE_b_instance * tl_CAMBRIDGE_b_instance=(*tl_CAMBRIDGE_b_multilist).bufferlist+ilv1;
		b_instance tl_b_instance;
		tl_b_instance=b_instance();
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),color))
		{
			int tlcolor=(*tl_CAMBRIDGE_b_instance).color-2;
			if (tlcolor==-2) {tl_b_instance.color=0x000000; goto color_fertig; }
			if (tlcolor==-1) {tl_b_instance.color=0xFFFFFF; goto color_fertig; }
			if (tlcolor<(*glob_CAMBRIDGE_color_multilist).filllevel)
			{
				tl_b_instance.color=((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].r*255)*65536)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].g*255)*256)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].b*255));
			}
			else
			{
				tl_b_instance.color=0x000000;
			}
		}
		else
		{
			tl_b_instance.color=0x000000;
		}
		color_fertig:
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),Order))
		{
			tl_b_instance.Order=(*tl_CAMBRIDGE_b_instance).Order;
		}
		else
		{
			tl_b_instance.Order=1;
		}
		tl_b_instance.B=(*tl_CAMBRIDGE_b_instance).B;
		tl_b_instance.E=(*tl_CAMBRIDGE_b_instance).E;
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),Z))
		{
			tl_b_instance.Z=(*tl_CAMBRIDGE_b_instance).Z;
		}
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),Display))
		{
			tl_b_instance.Display=(*tl_CAMBRIDGE_b_instance).Display;
		}
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_b_instance,(*tl_CAMBRIDGE_b_instance),Display2))
		{
			tl_b_instance.Display2=(*tl_CAMBRIDGE_b_instance).Display2;
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

void CAMBRIDGECONV_texts()
{
	multilist<CAMBRIDGE_t_instance> * tl_CAMBRIDGE_t_multilist=retrievemultilist<CAMBRIDGE_t_instance>();
	multilist<CAMBRIDGE_s_instance> * tl_CAMBRIDGE_s_multilist=retrievemultilist<CAMBRIDGE_s_instance>();
	multilist<t_instance> * tl_t_multilist=retrievemultilist<t_instance>();
	(*tl_t_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_t_multilist).filllevel;ilv1++)
	{
		CAMBRIDGE_t_instance * tl_CAMBRIDGE_t_instance=(*tl_CAMBRIDGE_t_multilist).bufferlist+ilv1;
		t_instance tl_t_instance;
		s_instance tl_s_instance;
		tl_t_instance=t_instance();
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_t_instance,(*tl_CAMBRIDGE_t_instance),color))
		{
			int tlcolor=(*tl_CAMBRIDGE_t_instance).color-2;
			if (tlcolor==-2) {tl_t_instance.color=0x000000; goto color_fertig; }
			if (tlcolor==-1) {tl_t_instance.color=0xFFFFFF; goto color_fertig; }
			if (tlcolor<(*glob_CAMBRIDGE_color_multilist).filllevel)
			{
				tl_t_instance.color=((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].r*255)*65536)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].g*255)*256)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].b*255));
			}
			else
			{
				tl_t_instance.color=0x000000;
			}
		}
		else
		{
			tl_t_instance.color=0x000000;
		}
		color_fertig:
		if (AUTOSTRUCT_EXISTS(CAMBRIDGE_t_instance,(*tl_CAMBRIDGE_t_instance),p))
		{
			tl_t_instance.xyz.x=(*tl_CAMBRIDGE_t_instance).p.x;
			tl_t_instance.xyz.y=(*tl_CAMBRIDGE_t_instance).p.y;
			tl_t_instance.xyz.z=0;
		}
		tl_t_instance.id=(*tl_CAMBRIDGE_t_instance).id;
		tl_t_instance.Z=(*tl_CAMBRIDGE_t_instance).Z;
		(*tl_t_multilist).ADD(&tl_t_instance);
		TELESCOPE_aggressobject(tl_t_multilist,ilv1);
		for (int ilv2=(*((*tl_CAMBRIDGE_t_instance).s)).start_in_it;ilv2<(*((*tl_CAMBRIDGE_t_instance).s)).start_in_it+(*((*tl_CAMBRIDGE_t_instance).s)).count_in_it;ilv2++)
		{
			CAMBRIDGE_s_instance * tl_CAMBRIDGE_s_instance=(*tl_CAMBRIDGE_s_multilist).bufferlist+ilv2;
			tl_s_instance.font=(*tl_CAMBRIDGE_s_instance).font;
			tl_s_instance.face=(*tl_CAMBRIDGE_s_instance).face;
			if (AUTOSTRUCT_EXISTS(CAMBRIDGE_s_instance,(*tl_CAMBRIDGE_s_instance),color))
			{
				int tlcolor=(*tl_CAMBRIDGE_s_instance).color-2;
				if (tlcolor==-2) {tl_s_instance.color=0x000000; goto color_fertig; }
				if (tlcolor==-1) {tl_s_instance.color=0xFFFFFF; goto color_fertig; }
				if (tlcolor<(*glob_CAMBRIDGE_color_multilist).filllevel)
				{
					tl_s_instance.color=((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].r*255)*65536)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].g*255)*256)|((_u8)(((*glob_CAMBRIDGE_color_multilist).bufferlist)[tlcolor].b*255));
				}
				else
				{
					tl_s_instance.color=0x000000;
				}
			}
			else
			{
				tl_s_instance.color=0x000000;
			}
			tl_s_instance.length=sizeof(s_instance)+strlen((*tl_CAMBRIDGE_s_instance).PCTEXT.a)+1;//TODO: remove trailing 0, but not before it is no longer needed by draw!
			tl_s_instance.type=TELESCOPE_ELEMENTTYPE_s;
			TELESCOPE_add(TELESCOPE_ELEMENTTYPE_s,(*tl_CAMBRIDGE_s_instance).PCTEXT.a,tl_s_instance.length-sizeof(s_instance));
			*((s_instance*)TELESCOPE_getproperty())=tl_s_instance;
		}
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
	CAMBRIDGECONV_atoms();
	CAMBRIDGECONV_bonds();
	CAMBRIDGECONV_texts();
}
