struct fileformat_
{
	char ending[8];
	char description[64];
	int fcolor,bgcolor;
	char saved,opened;
};
fileformat_ fileformat[]={
{"","from File ending",0xFFFFFF,0,1,1},
{".cdx","CambridgeSoft binary",0xFFFFFF,0,1,1},
{".cdxml","CambridgeSoft xml (recommended)",0xFFFFFF,0,1,1},
{".png","png (uncompressed)",0xFFFF00,0,1,0},
{".bmp","Bitmap 24 Bit+Alpha",0xFFFF00,0,1,0},
{".svg","scalable vector graphics",0xFFFF00,0,1,0},
{".cdml","bkchem (highly experimental, incomplete)",0xFF0000,0,0,1},
};
int fileformat_count=sizeof(fileformat)/sizeof(fileformat_);
