#define constants_MSPoints 64
float analysis_MSSpectrum[constants_MSPoints];
float analysis_MSSplit[constants_MSPoints];
float analysis_MSNext[constants_MSPoints];
float analysis_MSPower[constants_MSPoints];
int analysis_MSmin,analysis_MSmax;
float analysis_MS100;
int analysis_analysis[element_max];
int analysis_charge;
float element_mass(int input)
{
	float wert=0;
	for (int ilv1=0;element[input].isotopes[ilv1]>0;ilv1+=2)
	{
		wert+=element[input].isotopes[ilv1]*element[input].isotopes[ilv1+1];
	}
	return wert;
}
float element_isotopecount(int input)
{
	int wert=0;
	for (int ilv1=0;element[input].isotopes[ilv1]>0;ilv1+=2)
	{
		wert+=1;
	}
	return wert;
}
float element_exactmass(int input)
{
	float bestmass=0;
	float bestabundancy=0;
	for (int ilv1=0;element[input].isotopes[ilv1]>0;ilv1+=2)
	{
		if (bestabundancy<element[input].isotopes[ilv1])
		{
			bestmass=element[input].isotopes[ilv1+1];
			bestabundancy=element[input].isotopes[ilv1];
		}
	}
	return bestmass;
}
float element_minmass(int input)
{
	return element[input].isotopes[1];
}
float element_maxmass(int input)
{
	float wert=0;
	int ilv1;
	for (ilv1=0;element[input].isotopes[ilv1]>0;ilv1+=2)
	{
	}
	return element[input].isotopes[ilv1-1];
}
int edit_interpretasmoleculechainformula(multilist<n_instance> * imultilist,int inumber);
void analysis_ElementalAnalysis()
{
	analysis_charge=0;
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
			analysis_charge+=tl_n_instance->charge;
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
void analysis_MSadd(float sum,float intensity)
{
	analysis_MSSpectrum[(int)((sum-analysis_MSmin)/(analysis_MSmax-analysis_MSmin))]+=intensity;
}
void analysis_MSmul(float * factor1,float * factor2,float * result)//both factors may be identical or identical with the result
{
	for (int ilv1=0;ilv1<constants_MSPoints;ilv1++)
	{
		analysis_MSNext[ilv1]=0;
	}
	for (int ilv1=0;ilv1<constants_MSPoints;ilv1++)
	{
		for (int ilv2=0;ilv2<constants_MSPoints-ilv1;ilv2++)
		{
			analysis_MSNext[ilv2+ilv1]+=factor1[ilv1]*factor2[ilv2];
		}
	}
	for (int ilv1=0;ilv1<constants_MSPoints;ilv1++)
	{
		result[ilv1]=analysis_MSNext[ilv1];
	}
}
float analysis_MSPivot(float * input)
{
	float result=0;
	float sum=0;
	for (int ilv1=0;ilv1<constants_MSPoints;ilv1++)
	{
		sum+=input[ilv1];
		result+=input[ilv1]*ilv1;
	}
	result/=sum;
	return result;
}
void analysis_MSMove(float * input,int amount,int * shift)
{
	shift-=amount;
	if (amount<0)
	{
		for (int ilv1=0;ilv1<constants_MSPoints+amount;ilv1++)
		{
			input[ilv1]=input[ilv1-amount];
		}
		for (int ilv1=constants_MSPoints+amount;ilv1<constants_MSPoints;ilv1++)
		{
			input[ilv1]=0;
		}
	}
	if (amount>0)
	{
		for (int ilv1=constants_MSPoints-1;ilv1>=amount;ilv1--)
		{
			input[ilv1]=input[ilv1-amount];
		}
		for (int ilv1=0;ilv1<amount;ilv1++)
		{
			input[ilv1]=0;
		}
	}
}
void analysis_mulmass(int nr)
{
	int isotopecount=element_isotopecount(nr);
	for (int ilv1=0;ilv1<constants_MSPoints;ilv1++)
	{
		analysis_MSSplit[ilv1]=0;
	}
	analysis_MSSplit[0]=1;
	for (int ilv1=0;ilv1<constants_MSPoints;ilv1++)
	{
		analysis_MSPower[ilv1]=0;
	}
	analysis_MSPower[0]=1;
	int element_basicmass=round(element[nr].isotopes[1]);
	int element_maxdeltamass=round(element[nr].isotopes[isotopecount*2-1]);
	int element_powerdeltamass=element_basicmass;
	//generates an isotope-split for ONE Element.
	for (int ilv1=0;element[nr].isotopes[ilv1]!=0;ilv1+=2)
	{
		analysis_MSPower[(int)round(element[nr].isotopes[ilv1+1])-element_basicmass]+=element[nr].isotopes[ilv1];
	}
	//Then, the split is applied to its powers of two who match the number of elements present in that analysis
	for (int ilv1=0;ilv1<analysis_analysis[nr];ilv1++)
	{
		if ((1<<ilv1) & analysis_analysis[nr])
		{
			analysis_MSmin+=element_powerdeltamass;
			analysis_MSmul(analysis_MSSplit,analysis_MSPower,analysis_MSSplit);
		}
		analysis_MSmul(analysis_MSPower,analysis_MSPower,analysis_MSPower);
		element_powerdeltamass+=element_powerdeltamass;
		int tl_pivot=analysis_MSPivot(analysis_MSPower);
/*			if (tl_pivot>(constants_MSPoints>>1))
		{
			analysis_MSMove(analysis_MSPower,(constants_MSPoints>>1)-tl_pivot,&element_powerdeltamass);
		}*/
	}
	analysis_MSmul(analysis_MSSpectrum,analysis_MSSplit,analysis_MSSpectrum);
	int tl_pivot=analysis_MSPivot(analysis_MSSpectrum);
/*		if (tl_pivot>(constants_MSPoints>>1))
	{
		analysis_MSMove(analysis_MSSpectrum,(constants_MSPoints>>1)-tl_pivot,&analysis_MSmin);
	}*/
}
void analysis_MS()
{
	for (int ilv1=0;ilv1<constants_MSPoints;ilv1++)
	{
		analysis_MSSpectrum[ilv1]=0;
	}
	analysis_MSSpectrum[0]=1;
	analysis_MSmin=0;
	analysis_MSmax=0;
/*      for (int ilv1=0;ilv1<element_max;ilv1++)
	{
		analysis_MSmin+=analysis_analysis[ilv1]*element_minmass(ilv1);
	}
	for (int ilv1=0;ilv1<element_max;ilv1++)
	{
		analysis_MSmax+=analysis_analysis[ilv1]*element_maxmass(ilv1);
	}*/
	for (int ilv1=0;ilv1<element_max;ilv1++)
	{
		if (analysis_analysis[ilv1]>0)
		{
			analysis_mulmass(ilv1);
		}
	}
	analysis_MS100=0;
	for (int ilv1=0;ilv1<constants_MSPoints;ilv1++)
	{
		if (analysis_MS100<analysis_MSSpectrum[ilv1]) analysis_MS100=analysis_MSSpectrum[ilv1];
	}
}
