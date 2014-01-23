typedef struct telescopehead
{
	int length;//Can be max 2147483647 bytes long. Thats enough
	_u64 owner;//number of the owner. ordinal numbers can be reset upon resorting, so 32 bit is enough. However, I dont WANT to resort, since I need unique ID's anyway.
} telescopehead;
bienum telescopeelementtype
{
	{{"None"},0},//better do not use ... 
	{{"Empty"},1},//empty, tells the sackdown to discard this element.
	{{"Annotation"},2},
	{{"Textchunk"},3},
	{{"PolygonPoints"},4},
	{{"Bezierpoints"},5},//Contains a bezierpoint list. However, there are only as many elements valid as specified in <count>
	{{"Vectorpoints"},6},//not defined yet. define with care to ensure uttermost compatibility
	{{"EnhancedID"},7},//contains an id with purposes.
	{{"ContentList"},8},//contains a id list
}
bienum TELESCOPE_IDpurposes
{
	{{"None"},0},//better do not use ...
	{{"Owner"},1},//A group it belongs to. Any object should only have one, or the user should prepare for frazzlement.
	{{"LinePartner"},2},//the >>other bracket<< etc.
	{{"FurtherGroup"},3},//For selection groups or something.
	{{"TextField"},4},//Textfield
}
typedef struct telescopeelement
{
	_u16 type; //see telescopeelementtype
	_u16 length; //starting from end of this structure
}
typedef struct telescope
{
	int length;
	int owner;
	telescopechain a[2147483647];
};//no primary instance, or we will trash our memory on startup-time!!!
