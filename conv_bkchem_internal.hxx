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
		if (strcmp((tl_BKCHEM_bond_instance)->master->getName(),"standard")==0) continue;
		b_instance tl_b_instance;
		tl_b_instance=b_instance();
		tl_b_instance.Order=tl_BKCHEM_bond_instance->type;
		tl_b_instance.B=tl_BKCHEM_bond_instance->start+1+glob_n_multilist->maxid;
		tl_b_instance.E=tl_BKCHEM_bond_instance->end+1+glob_n_multilist->maxid;
		(*tl_b_multilist).ADD(&tl_b_instance);
	}
}
void BKCHEMCONV_arrow()
{
	multilist<BKCHEM_arrow_instance> * tl_BKCHEM_arrow_multilist=retrievemultilist<BKCHEM_arrow_instance>();
	multilist<arrow_instance> * tl_arrow_multilist=retrievemultilist<arrow_instance>();
	multilist<curve_instance> * tl_curve_multilist=retrievemultilist<curve_instance>();
	for (int ilv1=0;ilv1<(*tl_BKCHEM_arrow_multilist).filllevel;ilv1++)
	{
		int ihv1;
		BKCHEM_arrow_instance * tl_BKCHEM_arrow_instance=(*tl_BKCHEM_arrow_multilist).bufferlist()+ilv1;
		multilistreference<BKCHEM_point_instance> * tl_BKCHEM_point_multilistreference=(multilistreference<BKCHEM_point_instance>*)((*tl_BKCHEM_arrow_instance).point);
		if (((*tl_BKCHEM_point_multilistreference).count_in_it==2)||(((*tl_BKCHEM_point_multilistreference).count_in_it==3)&&(tl_BKCHEM_arrow_instance->spline==1)))
		{
			arrow_instance tl_arrow_instance;
			int tl_counter=0;
			tl_arrow_instance=arrow_instance();
			for (int ilv2=(*tl_BKCHEM_point_multilistreference).start_in_it;ilv2<(*tl_BKCHEM_point_multilistreference).start_in_it+(*tl_BKCHEM_point_multilistreference).count_in_it;ilv2++)
			{
				if (tl_counter==0)
				{
					(tl_arrow_instance).Head3D.x=(*glob_BKCHEM_point_multilist)[ilv2].x;
					(tl_arrow_instance).Head3D.y=(*glob_BKCHEM_point_multilist)[ilv2].y;
					(tl_arrow_instance).Head3D.z=0;
				}
				if (tl_counter==1)
				{
					(tl_arrow_instance).Tail3D.x=(*glob_BKCHEM_point_multilist)[ilv2].x;
					(tl_arrow_instance).Tail3D.y=(*glob_BKCHEM_point_multilist)[ilv2].y;
					(tl_arrow_instance).Tail3D.z=0;
					(tl_arrow_instance).Center3D.x=((tl_arrow_instance).Head3D.x+(tl_arrow_instance).Tail3D.x)/2;
					(tl_arrow_instance).Center3D.y=((tl_arrow_instance).Head3D.y+(tl_arrow_instance).Tail3D.y)/2;
					(tl_arrow_instance).Center3D.z=((tl_arrow_instance).Head3D.z+(tl_arrow_instance).Tail3D.z)/2;
				}
				if (tl_counter==2)
				{
					cdx_Point3D ilast=(tl_arrow_instance).Tail3D;
					cdx_Point3D ivector,iorthovector;
					(tl_arrow_instance).Tail3D.x=(*glob_BKCHEM_point_multilist)[ilv2].x;
					(tl_arrow_instance).Tail3D.y=(*glob_BKCHEM_point_multilist)[ilv2].y;
					(tl_arrow_instance).Tail3D.z=0;
					ivector.x=(tl_arrow_instance).Tail3D.x-(tl_arrow_instance).Head3D.x;
					ivector.y=(tl_arrow_instance).Tail3D.y-(tl_arrow_instance).Head3D.y;
					ivector.z=(tl_arrow_instance).Tail3D.z-(tl_arrow_instance).Head3D.z;
					(tl_arrow_instance).Center3D.x=((tl_arrow_instance).Head3D.x+(tl_arrow_instance).Tail3D.x)/2;
					(tl_arrow_instance).Center3D.y=((tl_arrow_instance).Head3D.y+(tl_arrow_instance).Tail3D.y)/2;
					(tl_arrow_instance).Center3D.z=((tl_arrow_instance).Head3D.z+(tl_arrow_instance).Tail3D.z)/2;
					double tl_length=sqrt(fsqr(ivector.x)+fsqr(ivector.y)+fsqr(ivector.z));
					ivector.x/=tl_length;
					ivector.y/=tl_length;
					ivector.z/=tl_length;
					ilast.x-=(tl_arrow_instance).Center3D.x;
					ilast.y-=(tl_arrow_instance).Center3D.y;
					ilast.z-=(tl_arrow_instance).Center3D.z;
					iorthovector.x=-ivector.y;
					iorthovector.y=ivector.x;
					iorthovector.z=0;
					double tl_olength=sqrt(fsqr(iorthovector.x)+fsqr(iorthovector.y)+fsqr(iorthovector.z));
					if (tl_olength>0.00001)
					{
						iorthovector.x/=tl_olength;
						iorthovector.y/=tl_olength;
						iorthovector.z/=tl_olength;
					}
					else
					{
						iorthovector.x=1;
						iorthovector.y=0;
						iorthovector.z=0;
					}
					double tl_distance=iorthovector.x*ilast.x+iorthovector.y*ilast.y+iorthovector.z*ilast.z;
					(tl_arrow_instance).AngularSize=-atan(tl_distance/tl_length)*2*180.0/Pi;
					double tl_centerdistance=-tl_length*tl_length/tl_distance;
					printf("CD:%f\n",tl_centerdistance);
					double tl_radius=fabs(tl_distance)+fabs(tl_centerdistance);
					(tl_arrow_instance).Center3D.x+=iorthovector.x*tl_centerdistance;
					(tl_arrow_instance).Center3D.y+=iorthovector.y*tl_centerdistance;
					(tl_arrow_instance).Center3D.z+=iorthovector.z*tl_centerdistance;
					tl_radius=sqrt(fsqr(tl_arrow_instance.Head3D.x-tl_arrow_instance.Center3D.x)+fsqr(tl_arrow_instance.Head3D.y-tl_arrow_instance.Center3D.y)+fsqr(tl_arrow_instance.Head3D.z-tl_arrow_instance.Center3D.z));
					(tl_arrow_instance).MinorAxisEnd3D=(tl_arrow_instance).Center3D;
					(tl_arrow_instance).MinorAxisEnd3D.x+=iorthovector.x*tl_radius;
					(tl_arrow_instance).MinorAxisEnd3D.y+=iorthovector.y*tl_radius;
					(tl_arrow_instance).MinorAxisEnd3D.z+=iorthovector.z*tl_radius;
					(tl_arrow_instance).MajorAxisEnd3D.x=(tl_arrow_instance).Center3D.x+ivector.x*tl_radius;
					(tl_arrow_instance).MajorAxisEnd3D.y=(tl_arrow_instance).Center3D.y+ivector.y*tl_radius;
					(tl_arrow_instance).MajorAxisEnd3D.z=(tl_arrow_instance).Center3D.z+ivector.z*tl_radius;
				}
				tl_counter++;
			}
			int tl_headtype=2;
			switch ((tl_BKCHEM_arrow_instance)->type)
			{
				case BKCHEM_type_equilibrium: tl_arrow_instance.ArrowShaftSpacing=4; tl_arrow_instance.ArrowheadType=1;tl_headtype=3;tl_arrow_instance.ArrowheadHead=tl_headtype;tl_arrow_instance.ArrowheadTail=tl_headtype;break;
				case BKCHEM_type_retro2:
				case BKCHEM_type_retro: tl_arrow_instance.ArrowShaftSpacing=8; tl_arrow_instance.ArrowheadType=3;break;
				case BKCHEM_type_electron: tl_headtype=3;break;
			}
			if (tl_BKCHEM_arrow_instance->start==1)
			{
				tl_arrow_instance.ArrowheadHead=tl_headtype;
			}
			if (tl_BKCHEM_arrow_instance->end==1)
			{
				tl_arrow_instance.ArrowheadTail=tl_headtype;
			}
			tl_arrow_instance.color=tl_BKCHEM_arrow_instance->color;
			(*tl_arrow_multilist).ADD(&tl_arrow_instance);
		}
		else
		{
			curve_instance tl_curve_instance;
			//TODO curve-arrows
			continue;
		}
	}
}
extern int janitor_getmaxid(_u32 ino);
void BKCHEMCONV_maintointernal()
{
	janitor_getmaxid(0);
	BKCHEMCONV_atom();
	BKCHEMCONV_bond();
	BKCHEMCONV_arrow();
	janitor_getmaxid(0);//You never know what must be done with them next.
}
