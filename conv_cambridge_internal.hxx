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
		tl_n_instance.Element=6;
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

void CAMBRIDGECONV_maintointernal()
{
	CAMBRIDGECONV_atoms();
	CAMBRIDGECONV_bonds();
}
