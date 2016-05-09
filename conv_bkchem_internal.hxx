void BKCHEMCONV_atom()
{
	multilist<BKCHEM_atom_instance> * tl_BKCHEM_atom_multilist=retrievemultilist<BKCHEM_atom_instance>();
	multilist<n_instance> * tl_n_multilist=retrievemultilist<n_instance>();
	for (int ilv1=0;ilv1<(*tl_BKCHEM_atom_multilist).filllevel;ilv1++)
	{
		int ihv1;
		BKCHEM_atom_instance * tl_BKCHEM_atom_instance=(*tl_BKCHEM_atom_multilist).bufferlist()+ilv1;
		multilistreference<BKCHEM_point_instance> * tl_BKCHEM_point_multilistreference=(multilistreference<BKCHEM_point_instance>*)((*tl_BKCHEM_atom_instance).point);
		n_instance tl_n_instance;
		tl_n_instance=n_instance();
		tl_n_instance.Element=constants_Element_implicitcarbon;
		tl_n_instance.protons=4;
		tl_n_instance.color=0;
		tl_n_instance.id=(*tl_BKCHEM_atom_instance).id+1;
		tl_n_instance.charge=(*tl_BKCHEM_atom_instance).charge;
		CAMBRIDGECONV_FIXID(n)
		if ((*tl_BKCHEM_atom_instance).name<bkchemids_count)
		{
			for (int ilv2=0;ilv2<element_max;ilv2++)
			{
				if (strcmp(element[ilv2].name,bkchemids[(*tl_BKCHEM_atom_instance).name].name)==0)
				{
					tl_n_instance.Element=ilv2;
					tl_n_instance.protons=element[ilv2].maxbonds;
					if (element[ilv2].maxbonds!=-1)
					{
						if (tl_n_instance.protons>8-(element[ilv2].hasVE-tl_n_instance.charge))
						{
							tl_n_instance.protons=8-(element[ilv2].hasVE-tl_n_instance.charge);
						}
						if (tl_n_instance.protons>element[ilv2].hasVE-tl_n_instance.charge)
						{
							tl_n_instance.protons=element[ilv2].hasVE-tl_n_instance.charge;
						}
					}
					goto elementfound;
				}
			}
		}
		elementfound:;
		for (int ilv2=(*tl_BKCHEM_point_multilistreference).start_in_it;ilv2<(*tl_BKCHEM_point_multilistreference).start_in_it+(*tl_BKCHEM_point_multilistreference).count_in_it;ilv2++)
		{
			(tl_n_instance).xyz.x=(*glob_BKCHEM_point_multilist)[ilv2].x;
			(tl_n_instance).xyz.y=(*glob_BKCHEM_point_multilist)[ilv2].y;
		}
		(tl_n_instance).xyz.z=0;
		(*tl_n_multilist).ADD(&tl_n_instance);
	}
}
void BKCHEMCONV_bond()
{
	multilist<BKCHEM_bond_instance> * tl_BKCHEM_bond_multilist=retrievemultilist<BKCHEM_bond_instance>();
	multilist<b_instance> * tl_b_multilist=retrievemultilist<b_instance>();
	for (int ilv1=0;ilv1<(*tl_BKCHEM_bond_multilist).filllevel;ilv1++)
	{
		int ihv1;
		BKCHEM_bond_instance * tl_BKCHEM_bond_instance=(*tl_BKCHEM_bond_multilist).bufferlist()+ilv1;
		b_instance tl_b_instance;
		tl_b_instance=b_instance();
		tl_b_instance.Order=tl_BKCHEM_bond_instance->type;
		tl_b_instance.B=tl_BKCHEM_bond_instance->start+1+glob_n_multilist->maxid;
		tl_b_instance.E=tl_BKCHEM_bond_instance->end+1+glob_n_multilist->maxid;
		(*tl_b_multilist).ADD(&tl_b_instance);
	}
}
extern int janitor_getmaxid(_u32 ino);
void BKCHEMCONV_maintointernal()
{
	janitor_getmaxid(0);
	BKCHEMCONV_atom();
	BKCHEMCONV_bond();
	janitor_getmaxid(0);//You never know what must be done with them next.
}
