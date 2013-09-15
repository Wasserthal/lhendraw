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
if back effects should arise, the objects must be linked back to ordered list.This is impossible if Objects are reordered without being changed.
Either we need backlink lists(overkill), or NO back effects may arise. OK, no backeffects!!!
The ordered list will have to be gappy, too, for that reason!
Gaps will disappear on mergesort.
If we make a mergesort everytime an object is deleted, we need no gaps. But that would produce lags.
the sortlist doesnt get more items than any other lists altogether. 
As long the objects are only appended on the end, we can do a list sort which would go with n.(no exclamation mark here, please!)
*/
struct multi_objref
{
	int listnr;//-1 means empty
	int nr;
	int next;//-1 means final
	int last;//-1 means first
	int Z;//Or better let that out? either link ratio or actual order will be relevant!
}multi_objref;
multi_objref objectZorderlist;
/*
how is the depth order list created for the first time?
The objects are dumbly listed. Then we do a merge sort.

*/
//After each merge sort, the Items are renumbered.
//After a list sort, the Items are only renumbered if some were deleted.
//Problem: Z-Order is changed of Objects that are technically unchanged.
//Consequence: Z-Order must not be inside of the objects, but in the order list.
//No back effects!
//If objects should be inserted at a determined POSITION, The ordering list is ordered FIRST, 
multi_objref initlist()
{
 	for (int ilv1=0;ilv1<xml_set_register_count;ilv1++)
	{
		CDXMLREAD_functype thisfunc;
		int propertypos;
		basic_xml_element_set * thisset=((((basic_xml_element_set**)xml_set_register)[ilv1]));
		propertypos=thisset->getproperties("Z",&thisfunc);
		basicmultilist * thismultilist=((*thisset).hismultilist);
		if (propertypos!=-1)
		{
			for (int ilv1=0;ilv1<(*hismultilist).filllevel;ilv1++)
			{
				float tlthisZ=(float*)((((char*)((*hismultilist).bufferlist)))+ilv1*((*hismultilist).itemsize)+propertypos);
			}
		}
	}
}
multi_objref sortlist()
{
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
