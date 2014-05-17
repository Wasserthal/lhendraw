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
void CONVCAMBRIDGE_atoms()
{
	multilist<CAMBRIDGE_n_instance> * tl_CAMBRIDGE_n_multilist=retrievemultilist<CAMBRIDGE_n_instance>();
	multilist<n_instance> * tl_n_multilist=retrievemultilist<n_instance>();
	(*tl_CAMBRIDGE_n_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_n_multilist).filllevel;ilv1++)
	{
		int ihv1;
		n_instance * tl_n_instance=(*tl_n_multilist).bufferlist+ilv1;
		
		CAMBRIDGE_n_instance tl_CAMBRIDGE_n_instance;
		tl_CAMBRIDGE_n_instance=CAMBRIDGE_n_instance();
		tl_CAMBRIDGE_n_instance.id=(*tl_n_instance).id;
		tl_CAMBRIDGE_n_instance.Z=(*tl_n_instance).Z;
		(*tl_CAMBRIDGE_n_multilist).ADD(&tl_n_instance);
	}
}

void CONVCAMBRIDGE_bonds()
{
	multilist<CAMBRIDGE_b_instance> * tl_CAMBRIDGE_b_multilist=retrievemultilist<CAMBRIDGE_b_instance>();
	multilist<b_instance> * tl_b_multilist=retrievemultilist<b_instance>();
	(*tl_CAMBRIDGE_b_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_b_multilist).filllevel;ilv1++)
	{
		int ihv1;
		b_instance * tl_b_instance=(*tl_b_multilist).bufferlist+ilv1;
		
		CAMBRIDGE_b_instance tl_CAMBRIDGE_b_instance;
		tl_CAMBRIDGE_b_instance=CAMBRIDGE_b_instance();
		tl_CAMBRIDGE_b_instance.id=(*tl_b_instance).id;
		tl_CAMBRIDGE_b_instance.Z=(*tl_b_instance).Z;
		tl_CAMBRIDGE_b_instance.B=(*tl_b_instance).B;
		tl_CAMBRIDGE_b_instance.E=(*tl_b_instance).E;
		(*tl_CAMBRIDGE_b_multilist).ADD(&tl_b_instance);
	}
}

void CONVCAMBRIDGE_texts()
{
	multilist<CAMBRIDGE_t_instance> * tl_CAMBRIDGE_t_multilist=retrievemultilist<CAMBRIDGE_t_instance>();
	multilist<t_instance> * tl_t_multilist=retrievemultilist<t_instance>();
	(*tl_CAMBRIDGE_t_multilist).filllevel=0;
	for (int ilv1=0;ilv1<(*tl_CAMBRIDGE_t_multilist).filllevel;ilv1++)
	{
		int ihv1;
		t_instance * tl_t_instance=(*tl_t_multilist).bufferlist+ilv1;
		
		CAMBRIDGE_t_instance tl_CAMBRIDGE_t_instance;
		tl_CAMBRIDGE_t_instance=CAMBRIDGE_t_instance();
		tl_CAMBRIDGE_t_instance.id=(*tl_t_instance).id;
		tl_CAMBRIDGE_t_instance.Z=(*tl_t_instance).Z;
		(*tl_CAMBRIDGE_t_multilist).ADD(&tl_t_instance);
	}
}

void CONVCAMBRIDGE_maintointernal()
{
	CONVCAMBRIDGE_atoms();
	CONVCAMBRIDGE_bonds();
	CONVCAMBRIDGE_texts();
}
