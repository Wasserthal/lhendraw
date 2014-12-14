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
	char * baseadress;
	char ** baseadressadress;
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
	memory_bufferstructure[memory_bufferstructure_count].baseadress=(*address);
	memory_bufferstructure[memory_bufferstructure_count].baseadressadress=address;
}
void memory_init()
{
	memory_bufferstructure_max=LHENDRAW_buffersize/sizeof(memory_bufferstructure_);
	memory_bufferstructure=(memory_bufferstructure_*)malloc(LHENDRAW_buffersize);
	memory_bufferstructure[0].baseadress=(char*)memory_bufferstructure;
	memory_bufferstructure[0].baseadressadress=&(memory_bufferstructure[0].baseadress);
	memory_bufferstructure_count=1;
}
