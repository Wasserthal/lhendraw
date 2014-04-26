//All lengths are counted from the beginning of the element.
//the data of a TELESCOPE_element starts after the actual strucure.
//That data part is of total generic type.
struct superconstellation_directory
{
	int start,end;
	char name[stringlength];
};
extern superconstellation AUTOSTRUCTURE_propertylist[];
extern superconstellation_directory AUTOSTRUCTURE_propertydirectory[];
struct TELESCOPE_element
{
	int length;
	int type;//see telescopeelementtype
	int getcontents(char * name)//TODO: remove this function. I also have to change the content list to a property list in linemode=1
	{
		return -1;
	};
	int getproperties(const char * name,CDXMLREAD_functype * delegateoutput,int * posoutput=NULL)
	{
		return -1;
	};
	const char * getName(){return 0;}
};
struct TELESCOPE//one for each object
{
	int length; //starting from end of this structure
	_u32 owner;//number of the owner. ordinal numbers can be reset upon resorting, so 32 bit is enough. However, I dont WANT to resort, since I need unique ID's anyway.-1 means dummy.
};
