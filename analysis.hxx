int analysis_analysis[element_max];
float element_mass(int input)
{
	float wert=0;
	for (int ilv1=0;element[input].isotopes[ilv1]>0;ilv1+=2)
	{
		wert+=element[input].isotopes[ilv1]*element[input].isotopes[ilv1+1];
	}
	return wert;
}
int edit_interpretasmoleculechainformula(multilist<n_instance> * imultilist,int inumber);
void analysis_ElementalAnalysis()
{
	for (int ilv1=0;ilv1<element_max;ilv1++)
	{
		analysis_analysis[ilv1]=0;
	}
	int icompare=1<<STRUCTURE_OBJECTTYPE_n;
	for (int ilv1=0;ilv1<glob_n_multilist->filllevel;ilv1++)
	{
		n_instance * tl_n_instance=&((*glob_n_multilist).bufferlist()[ilv1]);
		if (selection_currentselection[ilv1] & icompare)
		{
			float i_bond_sum=0;
			if ((tl_n_instance->Element>=0) && (tl_n_instance->Element<element_max))
			{
				analysis_analysis[tl_n_instance->Element]++;
				for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
				{
					i_bond_sum+=(*glob_b_multilist)[atom_actual_node[ilv1].bonds[ilv2]].Order/16.0;
				}
				if (fmod(i_bond_sum,1.0)>0.4)
				{
					i_bond_sum=trunc(i_bond_sum)+1;
				}
				if (tl_n_instance->protons>i_bond_sum)
				{
					analysis_analysis[constants_Element_hydrogen]+=tl_n_instance->protons-i_bond_sum;
				}
			}
			if ((tl_n_instance->Element==-1))
			{
/*				for (int ilv1=0;ilv1<glob_t_multilist->filllevel;ilv1++)
				{
					if (glob_t_multilist->bufferlist()[ilv1].exist)
					{
						if (text_actual_node[ilv1]
					}
				}*/
				edit_interpretasmoleculechainformula(glob_n_multilist,ilv1);
			}
		}
	}
	analysis_analysis[constants_Element_implicitcarbon+1]+=analysis_analysis[constants_Element_implicitcarbon];
	analysis_analysis[constants_Element_implicitcarbon]=0;
}
float analysis_mass()
{
	float wert=0;
	for (int ilv1=0;ilv1<element_max;ilv1++)
	{
		wert+=analysis_analysis[ilv1]*element_mass(ilv1);
	}
	return wert;
}
/*int edit_interpretasmoleculechainformula(multilist<n_instance> * imultilist,int inumber)
{
	char * ipointer;
	int i_backval=0;
	int fsm=0;//0: nothing 1: One big letter 2: One big Letter, and also small letters 3: Hydrogens-H 4: Number reached 5: other symbol, done 6: Invalid-cant interpret
	i_backval=TELESCOPE_aggressobject(imultilist,inumber);
	if (i_backval)
	{
		i_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval) ipointer=(char*)TELESCOPE_getproperty_contents();
	}
	else
	{
		edit_scoop_numhydrogens=4;
		(*imultilist)[inumber].Element=constants_Element_implicitcarbon;
		goto yes_its_an_element;
	}
	edit_brackets_allowed=1;
	edit_grouplabels_allowed=1;
	edit_dashes_allowed=1;
	edit_hydrogens_allowed=0;
	inextelement:;
	edit_scoop_valids=0;
	(*(_u32*)edit_scoop_atomstring)=0;
	edit_scoop_numhydrogens=0;
	edit_scoop_charge=0;
	edit_scoop_atomcount=0;
	iback:
	if (i_backval)
	{
		processatomsymbol(&fsm,ipointer,(s_instance*)TELESCOPE_getproperty());
		if (fsm>=7) goto enoughtointerpret;
		if (((*(s_instance*)TELESCOPE_getproperty()).face & 0x60)!=0x60)
		{
			fsm=7;
		}
		i_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval)
		{
			ipointer=(char*)TELESCOPE_getproperty_contents();
			goto iback;
		}
	}
	enoughtointerpret:;
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		if (strcmp(edit_scoop_atomstring,element[ilv1].name)==0)
		{

			goto yes_its_an_element;
		}
	}
	for (int ilv1=0;ilv1<sizeof(element_abbreviation)/sizeof(element_abbreviation_);ilv1++)
	{
		if (strcmp(edit_scoop_atomstring,element_abbreviation[ilv1].name)==0)
		{
			for (int ilv2=0;element_abbreviation[ilv1].composition[ilv2]!=0;ilv2++)
			{
				for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
				{
					if (strcmp(edit_scoop_atomstring,element_abbreviation[ilv1].composition[ilv2+1])==0)
					{
						(*imultilist)[inumber].Element=ilv1;
						goto yes_its_an_element;
					}
				}
			}
			goto yes_its_an_element;
		}
	}
	return 0;
	yes_its_an_element:;
	return 1;
}*/
