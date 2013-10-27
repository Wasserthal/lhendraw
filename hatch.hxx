struct HATCH_atom_
{
	float x,y;
	int bonds[6];//to be sorted from left to right
	int bond_count;
	int relate;
};
HATCH_atom_ HATCH_atom[1000];
int HATCH_atom_count=0;
float getangle(float dx,float dy);
void HATCH_intorder_atom(HATCH_atom_ &iinput)
{
	float iangle=getangle(HATCH_atom[iinput.bonds[0]].x-iinput.x,HATCH_atom[iinput.bonds[0]].y-iinput.y);
	again:
	for (int ilv1=1;ilv1<iinput.bond_count;ilv1++)
	{
		float iangle2=getangle(HATCH_atom[iinput.bonds[ilv1]].x-iinput.x,HATCH_atom[iinput.bonds[ilv1]].y-iinput.y);
		float iangle3=(fmod((iangle2-iangle+Pi),Pi));
		for (int ilv2=ilv1+1;ilv2<iinput.bond_count;ilv2++)
		{
			float iangle4=getangle(HATCH_atom[iinput.bonds[ilv2]].x-iinput.x,HATCH_atom[iinput.bonds[ilv2]].y-iinput.y);
			float iangle5=(fmod((iangle4-iangle+Pi),Pi));
			if (iangle5<iangle3)
			{
				int tllocal=iinput.bonds[ilv1];
				iinput.bonds[ilv1]=iinput.bonds[ilv2];
				iinput.bonds[ilv2]=tllocal;
				goto again;
			}
		}
	}
}
int HATCH_add_bond(int number1,int number2)
{
	if (HATCH_atom[number1].bond_count<6)
	{
		HATCH_atom[number1].bonds[HATCH_atom[number1].bond_count++]=number2;
		return 1;
	}
	else
	{
		return 0;
	}
}
int HATCH_add_atom(int inr)
//Must insert the atoms in ascending order, i.e. with ascending inr
//Constraint: actualnode has the same number as its chemdraw counterpart.
//Constraint: Actualnode has same start as actual conterpart start
{
	HATCH_atom_ * ihatchatom=&(HATCH_atom[HATCH_atom_count++]);
	n_instance *iinstance=&((*glob_n_multilist).bufferlist[inr]);
	HATCH_atom[HATCH_atom_count].x=(*iinstance).p.x;
	HATCH_atom[HATCH_atom_count].y=(*iinstance).p.y;
	atom_actual_node_ * irelate=&(atom_actual_node[inr]);
	(*ihatchatom).relate=(*iinstance).id;
	(*ihatchatom).bond_count=0;
	for (int ilv1=0;ilv1<(*irelate).bondcount;ilv1++)
	{
		int tlbondnr=(*irelate).bonds[ilv1];
		bond_actual_node_ * ibond=&(bond_actual_node[tlbondnr]);
		if ((*ibond).start==inr)
		{
			if ((*ibond).end<(*ibond).start)
			{
				for (int ilv2=0;ilv2<HATCH_atom_count;ilv2++)
				{
					if (HATCH_atom[ilv2].relate==(*glob_b_multilist).bufferlist[tlbondnr].E)//theoretically, I could use their inr...
					{
						if (HATCH_add_bond(HATCH_atom_count,ilv2))
						{
							if (HATCH_add_bond(ilv2,HATCH_atom_count)==0)
							{
								HATCH_atom[HATCH_atom_count].bond_count--;
							}
						}
					}
				}
			}
		}
		if ((*ibond).end==inr)
		{
			if ((*ibond).start<(*ibond).end)
			{
				for (int ilv2=0;ilv2<HATCH_atom_count;ilv2++)
				{
					if (HATCH_atom[ilv2].relate==(*glob_b_multilist).bufferlist[tlbondnr].B)//theoretically, I could use their inr...
					{
						if (HATCH_add_bond(HATCH_atom_count,ilv2))
						{
							if (HATCH_add_bond(ilv2,HATCH_atom_count)==0)
							{
								HATCH_atom[HATCH_atom_count].bond_count--;//If too many bonds, omit the other side, too!							}
							}
						}
					}
				}
			}
		}
	}
	HATCH_atom_count++;
}
void HATCH_deletebond(int atnr,int bondnr)//must be called twice for each atom
{
	for (int ilv1=bondnr;ilv1<HATCH_atom[atnr].bond_count-1;ilv1++)
	{
		HATCH_atom[atnr].bonds[ilv1]=HATCH_atom[atnr].bonds[ilv1+1];
	}
	HATCH_atom[atnr].bond_count--;
}
void HATCH_deleteatom(int index)
{
	for (int ilv1=index;ilv1<HATCH_atom_count-1;ilv1++)
	{
		HATCH_atom[ilv1]=HATCH_atom[ilv1+1];
	}
	HATCH_atom_count--;
	for (int ilv1=0;ilv1<HATCH_atom_count;ilv1++)
	{
		for (int ilv2=0;ilv2<HATCH_atom[ilv1].bond_count;ilv2++)
		{
			if (HATCH_atom[ilv1].bonds[ilv2]==index)
			{
				HATCH_deletebond(ilv1,ilv2);
				ilv2--;//to try again, because the bonds sunk now.
			}
			else
			{
				if (HATCH_atom[ilv1].bonds[ilv2]>index)
				{
					HATCH_atom[ilv1].bonds[ilv2]--;
				}
			}
		}
	}
}
