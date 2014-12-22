//This is the memory unit. It should make a list of all buffers and extend/reallocate them as needed.
//CAMBRIDGE type multilists' "dependants" information (adjust bufferlistsize dynamically accordingly)
//Selections. Make them always as large so the desired count of items fits in them.
//(Should be trivial, as long as one index is smaller than the smallest object)

//Buffers to care about:
//The careabout buffer itself...
//all current multilists
//all current content multilists
//sglgfx bezierpoints and max's etc. (!)...
//actual_node (atoms and bonds)
//z-order-list
//ALL selection lists!
//The buffers mentioned in "undo".......

//What-to-do:
//Allocate memory
//Expand when desired, change entry points
//These are, 

//in addition to the trivial ones:
//multilist buffer pointers, EVEN IN UNDOBUFFERS (better deal differently with that!)
//Onliest question: what to do with pointers in saved multilists?
//well, upon load they are fitted to the current position of the content
//OK, but how should the alloc routine realize it would not be supposed to update them any longer?
//Re-register them. Use memory_handlemove to tell that a handle has moved.
//Use that when multilists are stored AND SET THE HANDLE TO THE MULTILISTINDEX

//!!! USE the suitable time point for that, and react with correct rewind if it is the wrong one !!!
//Adjust max undo steps when told to care about that
//Zero new memory in certain cases!
//
struct memory_bufferstructure_
{
	char * baseaddress;
	char ** baseaddressaddress;
	int allocsize;
};
memory_bufferstructure_ * memory_bufferstructure;
int memory_bufferstructure_count=0;
int memory_bufferstructure_max=0;
void memory_alloc(char ** address)
{
	if (memory_bufferstructure_count>=memory_bufferstructure_max-1)
	{
		fprintf(stderr,"Programming error! Not enough memspace for all structures!\n");exit(1);
	}
	(*address)=(char*)malloc(LHENDRAW_buffersize);
	memory_bufferstructure[memory_bufferstructure_count].baseaddress=(*address);
	memory_bufferstructure[memory_bufferstructure_count].baseaddressaddress=address;
}
void memory_areamoved(char * ibegin1,char * ibegin2,intl ilength)//can be called from outside to tell that data with baseaddresses has moved inside a buffer, or from memory if a memory is moved. Better name memory areas whether they contain memory addresses or not... (NO! too complicated).
{
	char * iend=ibegin1+ilength;
	intl idelta=ibegin2-ibegin1;
	for (int ilv1=0;ilv1<memory_bufferstructure_count;ilv1++)
	{
		if ((memory_bufferstructure[ilv1].baseaddressaddress>=(char**)ibegin1) && (memory_bufferstructure[ilv1].baseaddressaddress<(char**)iend))
		{
			memory_bufferstructure[ilv1].baseaddressaddress+=idelta;
		}
	}
}
int memory_free(void * ibaseaddress)
{
	for (int ilv1=0;ilv1<memory_bufferstructure_count;ilv1++)
	{
		if (memory_bufferstructure[ilv1].baseaddress==ibaseaddress)
		{
			free(memory_bufferstructure[ilv1].baseaddress);
			*(memory_bufferstructure[ilv1].baseaddressaddress)=NULL;
			for (int ilv2=ilv1;ilv2<memory_bufferstructure_count-1;ilv2++)
			{
				memory_bufferstructure[ilv1]=memory_bufferstructure[ilv1+1];
			}
			memory_bufferstructure_count--;
			return 1;
		}
	}
	//This would be a memory-double-free, or freeing of a not mapped memory block.
	return 0;
}
int memory_realloc_down(int newsize)
{
	char * oldaddress;
	for (int ilv1=0;ilv1<memory_bufferstructure_count;ilv1++)
	{
		if (memory_bufferstructure[ilv1].allocsize>newsize)
		{
			char * newaddress=(char*)realloc(memory_bufferstructure[ilv1].baseaddress,newsize);
			if (newaddress==NULL) return 0;//TODO: handle failures
			memory_bufferstructure[ilv1].allocsize=newsize;
			if (newaddress!=memory_bufferstructure[ilv1].baseaddress)
			{
				oldaddress=memory_bufferstructure[ilv1].baseaddress;
				*(memory_bufferstructure[ilv1].baseaddressaddress)=newaddress;
				memory_bufferstructure[ilv1].baseaddress=newaddress;
				memory_areamoved(oldaddress,newaddress,LHENDRAW_buffersize);
			}
		}
	}
	LHENDRAW_buffersize=newsize;
	return 1;
}
int memory_realloc_up(int newsize)
{
	char * oldaddress;
	for (int ilv1=0;ilv1<memory_bufferstructure_count;ilv1++)
	{
		char * newaddress=(char*)malloc(newsize);
		if (newaddress==NULL) {memory_realloc_down(LHENDRAW_buffersize);return 0;}
		memory_bufferstructure[ilv1].allocsize=newsize;
		memcpy(newaddress,memory_bufferstructure[ilv1].baseaddress,LHENDRAW_buffersize);
		oldaddress=memory_bufferstructure[ilv1].baseaddress;
		*(memory_bufferstructure[ilv1].baseaddressaddress)=newaddress;
		memory_bufferstructure[ilv1].baseaddress=newaddress;
		free(oldaddress);//TODO: use contiguous ones 
		memory_areamoved(oldaddress,newaddress,LHENDRAW_buffersize);
	}
	LHENDRAW_buffersize=newsize;
	return 1;
}
int memory_realloc_x2()
{
	return memory_realloc_up(LHENDRAW_buffersize*2);
}
int memory_realloc_d2()
{
	if (LHENDRAW_buffersize<=4096)
	return 0;
	return memory_realloc_down(LHENDRAW_buffersize/2);
}
void memory_init()
{
	memory_bufferstructure_max=LHENDRAW_buffersize/sizeof(memory_bufferstructure_);
	memory_bufferstructure=(memory_bufferstructure_*)malloc(LHENDRAW_buffersize);
	memory_bufferstructure[0].baseaddress=(char*)memory_bufferstructure;
	memory_bufferstructure[0].baseaddressaddress=(char**)&memory_bufferstructure;
	memory_bufferstructure[0].allocsize=LHENDRAW_buffersize;
	memory_bufferstructure_count=1;
}
