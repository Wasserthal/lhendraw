
#define intl long long
#ifndef stringlength
#define stringlength 512
#endif
#define bufferlength 256
#define bufferlistsize 4096
#define multilistlistlength 512
#define _u32 unsigned int
#define _i32 signed int
#define _u16 unsigned short
#define _i16 signed short
#define _u8 unsigned char
#define _i8 signed char
inline intl sqr(intl input)
{
	return input*input;
}
char spaciatic(char input)
{
	if (input==' ')
	{
		return 1;
	}
	if (input==10)
	{
		return 1;
	}
	if (input==13)
	{
		return 1;
	}
	return 0;
}
char sentenumeric(char input)
{
	if ((input>='a') && (input<='z'))
	{
		return 1;
	}
	if ((input>='A') && (input<='Z'))
	{
		return 1;
	}
	if ((input>='0') && (input<='9'))
	{
		return 1;
	}
	if (input=='_')
	{
		return 1;
	}
	return 0;
}
