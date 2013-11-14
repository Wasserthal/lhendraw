struct HATCH_atom_
{
	float x,y;
	int bonds[6];//to be sorted from left to right
	int bondfriends[6];//number of the corresponding bond from the other atom
	int bondpassed[6];
	int bond_count;
	int relate;
	char isrim;
};
HATCH_atom_ HATCH_atom[1000];
int HATCH_atom_count=0;
float getangle(float dx,float dy);
int currenthatchlist[250];
int currenthatchlist_count;
void HATCH_intorder_atom(HATCH_atom_ &iinput)
{
	float iangle=getangle(HATCH_atom[iinput.bonds[0]].x-iinput.x,HATCH_atom[iinput.bonds[0]].y-iinput.y);
	again:
	for (int ilv1=1;ilv1<iinput.bond_count;ilv1++)
	{
		float iangle2=getangle(HATCH_atom[iinput.bonds[ilv1]].x-iinput.x,HATCH_atom[iinput.bonds[ilv1]].y-iinput.y);
		float iangle3=(fmod((iangle2-iangle+4*Pi),2*Pi));
		if (iangle3>Pi) iangle3-=2*Pi;
		for (int ilv2=ilv1+1;ilv2<iinput.bond_count;ilv2++)
		{
			float iangle4=getangle(HATCH_atom[iinput.bonds[ilv2]].x-iinput.x,HATCH_atom[iinput.bonds[ilv2]].y-iinput.y);
			float iangle5=(fmod((iangle4-iangle+4*Pi),2*Pi));
			if (iangle5>Pi) iangle5-=2*Pi;
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
	HATCH_atom_ * ihatchatom=&(HATCH_atom[HATCH_atom_count]);
	n_instance *iinstance=&((*glob_n_multilist).bufferlist[inr]);
	HATCH_atom[HATCH_atom_count].x=(*iinstance).p.x;
	HATCH_atom[HATCH_atom_count].y=(*iinstance).p.y;
	atom_actual_node_ * irelate=&(atom_actual_node[inr]);
	(*ihatchatom).relate=(*iinstance).id;
	(*ihatchatom).bond_count=0;
	multilistreference<annotation_instance> * tlannotationmultilistreference=dynamic_cast<multilistreference<annotation_instance>*>((*glob_n_multilist).bufferlist[inr].annotation);
	for (int ilv2=0;ilv2<(*tlannotationmultilistreference).count_in_it;ilv2++)
	{
		annotation_instance * tlannotation=&((*glob_annotation_multilist).bufferlist[(*tlannotationmultilistreference).start_in_it+ilv2]);
		if (strcmp((*tlannotation).Keyword.a,"Name")==0)
		{
			if ((strcmp((*tlannotation).Content.a,"Dont"))==0)
			{
				return 0;
			}
		}
	}
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
								HATCH_atom[HATCH_atom_count].bond_count--;//If too many bonds, omit the other side, too!
							}
						}
					}
				}
			}
		}
	}
	HATCH_atom_count++;
	return 1;
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
void HATCH_shrink()
{
	iback:
	char ichanged;
	ichanged=0;
	for (int ilv1=0;ilv1<HATCH_atom_count;ilv1++)
	{
		if (HATCH_atom[ilv1].bond_count<2)
		{
			HATCH_deleteatom(ilv1);
			ilv1--;//as the next atom sunk here
			ichanged=1;
		}
	}
	if (ichanged)
	{
		goto iback;
	}
}
int HATCH_followborder(char inverse,int starting,int next,_u32 maytouchrim,char force=0)//maytouchrim: Bit0: not_rim_points allowed Bit1: not_rim points allowed when hadrim Bit2: rim_points allowed when not had rim Bit3: rim_points allowed when had rim. shl4: after having left rim
{
	int tllast=0;
	int icurrentpos;
	int ileftmost,leftmost_bond;
	float minangle;
	float iangle;
	char hadrim=0;
	char hadleftrim=0;
	currenthatchlist_count=0;
	hadrim|=HATCH_atom[starting].isrim|HATCH_atom[next].isrim;
	hadleftrim=(HATCH_atom[starting].isrim==1)&(HATCH_atom[next].isrim==0);
	icurrentpos=next;
	tllast=starting;
	iback:
	currenthatchlist[currenthatchlist_count++]=icurrentpos;
	iangle=getangle(HATCH_atom[icurrentpos].x-HATCH_atom[tllast].x,HATCH_atom[icurrentpos].y-HATCH_atom[tllast].y);
	ileftmost=-1;
	if (inverse==0)
	{
		minangle=-4*Pi;
	}
	else
	{
		minangle=4*Pi;
	}
	for (int ilv2=0;ilv2<HATCH_atom[icurrentpos].bond_count;ilv2++)
	{
		if (HATCH_atom[icurrentpos].bonds[ilv2]!=tllast)
		{
			if ((HATCH_atom[icurrentpos].bonds[ilv2]!=starting))
			{
				if (!((((maytouchrim>>((HATCH_atom[HATCH_atom[icurrentpos].bonds[ilv2]].isrim==1)*2+hadrim+hadleftrim*4)) & 1))>0))
				{
					goto notthis;
				}
			}
			else
			{
				if (HATCH_atom[icurrentpos].bondpassed[ilv2]==0)
				{
					leftmost_bond=ilv2;
					ileftmost=HATCH_atom[icurrentpos].bonds[ilv2];
					minangle=0;
					goto mustbethis;
				}
			}
			if (HATCH_atom[icurrentpos].bondpassed[ilv2]==0)
			{
				for (int ilv3=0;ilv3<currenthatchlist_count;ilv3++)
				{
					if (HATCH_atom[icurrentpos].bonds[ilv2]==currenthatchlist[ilv3])
					{
						goto notthis;
					}
				}
				float iangle2=getangle(HATCH_atom[HATCH_atom[icurrentpos].bonds[ilv2]].x-HATCH_atom[icurrentpos].x,HATCH_atom[HATCH_atom[icurrentpos].bonds[ilv2]].y-HATCH_atom[icurrentpos].y);
				float iangle3=(fmod((iangle2-iangle+4*Pi),2*Pi));
				if (iangle3>=Pi) iangle3-=2*Pi;
				if ((iangle3>minangle) ^ (inverse))
				{
					leftmost_bond=ilv2;
					ileftmost=HATCH_atom[icurrentpos].bonds[ilv2];
					minangle=iangle3;
				}
			}
			notthis:
			;
		}
	}
	mustbethis:
	if (ileftmost==-1)
	{
		if (force)
		{
			printf("forcing!!!!\n");
			fprintf(outfile,"<path d=\" ");
			fprintf(outfile,"M %f,%f ",HATCH_atom[starting].x+SVG_currentshiftx,HATCH_atom[starting].y+SVG_currentshifty);
			for (int ilv1=0;ilv1<currenthatchlist_count;ilv1++)
			{
				fprintf(outfile,"L %f,%f ",HATCH_atom[currenthatchlist[ilv1]].x+SVG_currentshiftx,HATCH_atom[currenthatchlist[ilv1]].y+SVG_currentshifty);
			}
			fprintf(outfile,"z \" style=\"fill:#FF0000;color:none;\" opacity=\"0.8\"/>\n",inverse?"00FF00":"007F00");
			return 0;
		}
		return -1;
	}
	if (HATCH_atom[icurrentpos].isrim==1)
	{
		hadrim=1;
	}
	else
	{
		if (hadrim)
		{
			hadleftrim=1;
		}
	}
	tllast=icurrentpos;
	icurrentpos=ileftmost;
	if (icurrentpos!=starting)
	{
		goto iback;
	}
	idone:
	for (int ilv1=0;ilv1<currenthatchlist_count-1;ilv1++)
	{
		for (int ilv2=0;ilv2<HATCH_atom[currenthatchlist[ilv1]].bond_count;ilv2++)
		{
			if (HATCH_atom[currenthatchlist[ilv1]].bonds[ilv2]==currenthatchlist[ilv1+1])
			{
				HATCH_atom[currenthatchlist[ilv1]].bondpassed[ilv2]=1;
			}
		}
	}
	for (int ilv2=0;ilv2<HATCH_atom[starting].bond_count;ilv2++)
	{
		if (HATCH_atom[starting].bonds[ilv2]==currenthatchlist[0])
		{
			HATCH_atom[starting].bondpassed[ilv2]=1;
		}
	}
	for (int ilv2=0;ilv2<HATCH_atom[currenthatchlist[currenthatchlist_count-1]].bond_count;ilv2++)
	{
		if (HATCH_atom[currenthatchlist[currenthatchlist_count-1]].bonds[ilv2]==starting)
		{
			HATCH_atom[currenthatchlist[currenthatchlist_count-1]].bondpassed[ilv2]=1;
		}
	}
	fprintf(outfile,"<path d=\" ");
	fprintf(outfile,"M %f,%f ",HATCH_atom[starting].x+SVG_currentshiftx,HATCH_atom[starting].y+SVG_currentshifty);
	for (int ilv1=0;ilv1<currenthatchlist_count;ilv1++)
	{
		fprintf(outfile,"L %f,%f ",HATCH_atom[currenthatchlist[ilv1]].x+SVG_currentshiftx,HATCH_atom[currenthatchlist[ilv1]].y+SVG_currentshifty);
	}
	fprintf(outfile,"z \" style=\"fill:#%s;color:none;\" opacity=\"0.8\"/>\n",inverse?"00FF00":"007F00");
	return 0;
}
void HATCH_follownextborder(char inverse,char startonrim,int maytouchrim,char force=0)
{
	iback:
	char changed=0;
	for (int ilv1=0;ilv1<HATCH_atom_count;ilv1++)
	{
		for (int ilv2=0;ilv2<HATCH_atom[ilv1].bond_count;ilv2++)
		{
			if (HATCH_atom[ilv1].bondpassed[ilv2]==1)
			{
				int tlotheratom=HATCH_atom[ilv1].bonds[ilv2];
				if (!(((HATCH_atom[ilv1].isrim==1) ? (startonrim & 1) : (startonrim & 2)) || ((HATCH_atom[tlotheratom].isrim==1) ? (startonrim & 4) : (startonrim & 8))))
				{
					int tlotherbond=HATCH_atom[ilv1].bondfriends[ilv2];
					if (HATCH_atom[tlotheratom].bondpassed[tlotherbond]==0)
					{
						if (HATCH_followborder(inverse,tlotheratom,ilv1,maytouchrim,force)>=0)
						{
							changed=1;
						}
					}
				}
			}
		}
	}
	if (changed)
	{
		goto iback;
	}
}
void HATCH_getborders(float centerx,float centery,int * first,int * second)
{
	int farmost,farmost2;
	float maxdist;
	maxdist=0;
	float minangle,maxangle;
	char irechtsrum;
	int leftmost,rightmost;
	int icurrentpos,ilastpos;
//	fprintf(outfile,"<path d=\" ");
	for (int ilv1=0;ilv1<HATCH_atom_count;ilv1++)
	{
		float currentdist=(sqr(HATCH_atom[ilv1].x-centerx)+sqr(HATCH_atom[ilv1].y-centery));
		if (currentdist>maxdist)
		{
			farmost=ilv1;
			maxdist=currentdist;
		}
		HATCH_atom[ilv1].isrim=0;
	}
	HATCH_atom[farmost].isrim=1;
	(*first)=farmost;
	float iangle=getangle(HATCH_atom[farmost].x-centerx,HATCH_atom[farmost].y-centery);
	minangle=2*Pi;
	maxangle=2*Pi;
//	fprintf(outfile,"M %f,%f ",HATCH_atom[farmost].x+SVG_currentshiftx,HATCH_atom[farmost].y+SVG_currentshifty);
	for (int ilv2=0;ilv2<HATCH_atom[farmost].bond_count;ilv2++)
	{
		float iangle2=getangle(HATCH_atom[HATCH_atom[farmost].bonds[ilv2]].x-HATCH_atom[farmost].x,HATCH_atom[HATCH_atom[farmost].bonds[ilv2]].y-HATCH_atom[farmost].y);
		float iangle3=(fmod((iangle2-iangle+4*Pi),2*Pi));
		if (iangle3>=Pi) iangle3-=2*Pi;
		if (iangle3>0)
		{
			if (iangle3<maxangle)
			{
				rightmost=HATCH_atom[farmost].bonds[ilv2];
				maxangle=iangle3;
			}
		}
		if (iangle3<minangle)
		{
			leftmost=HATCH_atom[farmost].bonds[ilv2];
			minangle=iangle3;
		}
	}
	if (maxangle<Pi)
	{
		icurrentpos=rightmost;
	}
	else
	{
		icurrentpos=leftmost;
	}
	(*second)=icurrentpos;
	HATCH_atom[icurrentpos].isrim=1;
	ilastpos=farmost;
	iback:
//	fprintf(outfile,"L %f,%f ",HATCH_atom[icurrentpos].x+SVG_currentshiftx,HATCH_atom[icurrentpos].y+SVG_currentshifty);
	iangle=getangle(HATCH_atom[icurrentpos].x-HATCH_atom[ilastpos].x,HATCH_atom[icurrentpos].y-HATCH_atom[ilastpos].y);
	minangle=4*Pi;
	for (int ilv2=0;ilv2<HATCH_atom[icurrentpos].bond_count;ilv2++)
	{
		if (HATCH_atom[icurrentpos].bonds[ilv2]!=ilastpos)
		{
			float iangle2=getangle(HATCH_atom[HATCH_atom[icurrentpos].bonds[ilv2]].x-HATCH_atom[icurrentpos].x,HATCH_atom[HATCH_atom[icurrentpos].bonds[ilv2]].y-HATCH_atom[icurrentpos].y);
			float iangle3=(fmod((iangle2-iangle+4*Pi),2*Pi));
			if (iangle3>=Pi) iangle3-=2*Pi;
			if (iangle3<minangle)
			{
				leftmost=HATCH_atom[icurrentpos].bonds[ilv2];
				minangle=iangle3;
			}
		}
	}
	ilastpos=icurrentpos;
	icurrentpos=leftmost;
	HATCH_atom[ilastpos].isrim=1;
	if (HATCH_atom[icurrentpos].isrim==0)
	{
		goto iback;
	}
//	fprintf(outfile,"z \" style=\"fill:#EE8E50;color:none;\" opacity=\"0.5\"/>\n");
}
void HATCH_friend_bonds()
{
	for (int ilv1=0;ilv1<HATCH_atom_count;ilv1++)
	{
		for (int ilv2=0;ilv2<HATCH_atom[ilv1].bond_count;ilv2++)
		{
			HATCH_atom[ilv1].bondpassed[ilv2]=0;
			int relay=HATCH_atom[ilv1].bonds[ilv2];
			if (relay<ilv1)
			{
				for (int ilv3=0;ilv3<HATCH_atom[relay].bond_count;ilv3++)
				{
					if (HATCH_atom[relay].bonds[ilv3]==ilv1)
					{
						HATCH_atom[ilv1].bondfriends[ilv2]=ilv3;
						HATCH_atom[relay].bondfriends[ilv3]=ilv2;
					}
				}
			}
		}
	}
}
void HATCH_main(float centerx,float centery)
{
	HATCH_shrink();
	HATCH_friend_bonds();//Speedhack: executing this after the previous speeds program up!
	if (HATCH_atom_count>1)
	{
		int tlfirst; int tlsecond;
		HATCH_getborders(centerx,centery,&tlfirst,&tlsecond);
		HATCH_followborder(0,tlfirst,tlsecond,0xFF);
		HATCH_follownextborder(0,0x5,0x3);

		HATCH_follownextborder(0,0x5,0xD);
		HATCH_followborder(1,tlsecond,tlfirst,0xFF);
		HATCH_follownextborder(1,0x5,0xD);
		HATCH_follownextborder(1,0xA,0xFF,true);
		for (int ilv1=0;ilv1<HATCH_atom_count;ilv1++)
		{
			get_colorstring(4);
			stylegenestring(1);
			for (int ilv2=0;ilv2<HATCH_atom[ilv1].bond_count;ilv2++)
			{
				get_colorstring(4+(ilv1<HATCH_atom[ilv1].bonds[ilv2]));
				stylegenestring(1);
				HATCH_atom_ iHATCH_atom=HATCH_atom[HATCH_atom[ilv1].bonds[ilv2]];
				if (HATCH_atom[ilv1].bondpassed[ilv2]==0)
				{
					expressline(HATCH_atom[ilv1].x+(ilv1<HATCH_atom[ilv1].bonds[ilv2]),HATCH_atom[ilv1].y,iHATCH_atom.x,iHATCH_atom.y);
				}
			}
		}
	}
}
