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
			}
			for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
			{
				i_bond_sum+=(*glob_b_multilist)[atom_actual_node[ilv1].bonds[ilv2]].Order/16.0;
			}
			if (fmod(i_bond_sum,1.0)>0.4)
			{
				i_bond_sum=trunc(i_bond_sum)+1;
			}
			analysis_analysis[constants_Element_hydrogen]+=tl_n_instance->protons-i_bond_sum;
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
