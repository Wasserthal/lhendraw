/*
gap design
each gap contains a pointer to the next and last one, each item contains a pointer to the next and last one
Why do I need another sort list?
That list would contain the depth order.
Why not order by depth order?
This would make rearrangements necessary, which is time-consuming
Why not make a depth order link variable?
From a certain point of view, a list is nothing else.
What is better, ordered list or depth links?
links are easier to be kept in order when objects are changed.
Also take in account that there are different types of objects, but only one depth for all.
=> The depth links must be inside these objects anyway.
Ordered list has the advantage that the processor works on it more easily, and insertion on a certain position requires no search.
As it requires a pre-ordering, latter is no good advantage.
if back effects should arise, the objects must be linked back to ordered list.This is impossible if Objects are reordered without being changed.
Either we need backlink lists(overkill), or NO back effects may arise. OK, no backeffects!!!
The ordered list will have to be gappy, too, for that reason!
Gaps will disappear on mergesort.
If we make a mergesort everytime an object is deleted, we need no gaps. But that would produce lags.
the sortlist doesnt get more items than any other lists altogether. 
As long the objects are only appended on the end, we can do a list sort which would go with n.(no exclamation mark here, please!)
*/
/*struct multi_objref
{
	int listnr;//number of multilist, -1 means empty 
	int nr;
	int next;//-1 means final
	int last;//-1 means first
	int Z;//Or better let that out? either link ratio or actual order will be relevant!
}multi_objref;*/
typedef struct multi_objref
{
	int listnr;//number of multilist, -1 means empty 
	int nr;//-1 means empty
}multi_objref;
#define multilistZcount 100//TODO: calculate properly
multi_objref objectZorderlist[bufferlistsize*multilistZcount];
/*
how is the depth order list created for the first time?
The objects are dumbly listed. Then we do a merge sort.

*/
//After each merge sort, the Items are renumbered.
//After a list sort, the Items are only renumbered if some were deleted, or inserted in the middle.
//Problem: Z-Order is changed of Objects that are technically unchanged.
//Consequence: Z-Order must not be inside of the objects, but in the order list.
//No back effects!
//If objects should be inserted at a determined POSITION, The ordering list is ordered FIRST, 
//Howto retrieve partner numbers on a freshly inserted/deleted object?
//Then anything is <i>ad felis</i>, sortlist gotta be rebuild after each change, we omit storing it...
//Storing the Z order in the Objects, gotta rebuild each time anyway.
//We dont need to sort if anything fits, but renumbering might be still necessary.

char somewhereinZlist(int listnr, int nr)
{
	for (int ilv1=0;ilv1<bufferlistsize*multilistZcount;ilv1++)
	{
		if (objectZorderlist[ilv1].listnr==-1)
		{
			objectZorderlist[ilv1].listnr=listnr;
			objectZorderlist[ilv1].nr=nr;
			return 1;
		}
	}
	return 0;
}
char insertinZlist(int tlthisZ, int listnr, int nr)
{
	if ((tlthisZ<0) || (tlthisZ>=bufferlistsize*multilistZcount))
	{
		return 0;
	}
	if (objectZorderlist[tlthisZ].listnr==-1)
	{
		objectZorderlist[tlthisZ].listnr=listnr;
		objectZorderlist[tlthisZ].nr=nr;
		return 1;
	}
	return 0;
}
multi_objref sortlist()
{
}
multi_objref initZlist()
{
	char iZorderbroken=0;
	for (int ilv1=0;ilv1<bufferlistsize*multilistZcount;ilv1++)
	{
		objectZorderlist[ilv1].nr=-1;objectZorderlist[ilv1].listnr=-1;
/*		objectZorderlist[ilv1].last=ilv1-1;objectZorderlist[ilv1].next=ilv1+1;*/
	}
/*	objectZorderlist[0].last=-1;
	objectZorderlist[bufferlistsize*multilistZcount-1].last=-1;*/
 	for (int ilv1=0;ilv1<xml_set_register_count;ilv1++)
	{
		CDXMLREAD_functype thisfunc;
		int propertypos;
		basic_xml_element_set * thisset=((((basic_xml_element_set**)xml_set_register)[ilv1]));
		propertypos=thisset->getproperties("Z",&thisfunc);
		basicmultilist * thismultilist=((*thisset).hismultilist);
		if (propertypos!=-1)
		{
			for (int ilv1=0;ilv1<(*thismultilist).filllevel;ilv1++)
			{
				int tlthisZ=*(int*)((((char*)((*thismultilist).pointer)))+ilv1*((*thismultilist).itemsize)+propertypos);
				if (!insertinZlist(tlthisZ, (*thismultilist).index,ilv1))
				{
					if (somewhereinZlist((*thismultilist).index,ilv1))
					{
						iZorderbroken=1;
					}
					else
					{
						fprintf(stderr,"memory overflow");
					}
				}
			}
		}
	}
}
/*
void mergesortrecursion(int single,int max,char ibool)
{
	int usedup1,usedup2;int remainingright;
	for (int ilv1=0;ilv1<max;ilv1+=2*single)
	{
		usedup1=0;usedup2=0;
		int done=0;
		remainingright=max-ilv1-single;
		if (remainingright<0)
		{
			while (usedup1<single+remainingright)
			{
				sortarr2[ibool^1][ilv1+done]=sortarr2[ibool][ilv1+usedup1];
				usedup1++;
				done++;
			}
			goto ifertig;
		}
		if (remainingright>single) remainingright=single;
		iback:
		if (usedup1>=single) 
		{
			goto finishright;
		}
		if (usedup2>=remainingright)
		{
			goto finishleft;
		}
		
		if (atoms[sortarr2[ibool][ilv1+usedup1]].pos.a[2].C>atoms[sortarr2[ibool][ilv1+single+usedup2]].pos.a[2].C)
		{
			sortarr2[ibool ^ 1][ilv1+done]=sortarr2[ibool][ilv1+usedup1];
			usedup1++;
		}
		else
		{
			sortarr2[ibool ^ 1][ilv1+done]=sortarr2[ibool][ilv1+single+usedup2];
			usedup2++;
		}
		done++;
		goto iback;
		finishleft:
		while (usedup1<single)
		{
			sortarr2[ibool^1][ilv1+done]=sortarr2[ibool][ilv1+usedup1];
			usedup1++;
			done++;
		}
		goto ifertig;
		finishright:
		while (usedup2<remainingright)
		{
			sortarr2[ibool^1][ilv1+done]=sortarr2[ibool][ilv1+single+usedup2];
			usedup2++;
			done++;
		}
		ifertig:
		;
	}
}

void mergesort()
{
	int ilv1;
	for (ilv1=0;ilv1<atom_count;ilv1++)
	{
		sortarr2[0][ilv1]=ilv1;
	}
	int t=1;int n=0;
	int subt;
	while (t<atom_count)
	{
		t*=2;
		n++;
	}
	subt=1;
	for (ilv1=0;ilv1<n;ilv1++)
	{
		mergesortrecursion(subt,atom_count, (ilv1 & 1));
		subt*=2;
	}
	for (int ilv2=0;ilv2<atom_count;ilv2++)
	{
		sortarr[ilv2]=sortarr2[(ilv1 & 1)][ilv2];
	}
	sortarr_count=atom_count;
}
*/