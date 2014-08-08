void test_routine();

#define intl long long
#ifndef stringlength
#define stringlength 512
#endif
#define bufferlength 512
#define bufferlistsize 4096
#define multilistlistlength 512
#define _u64 unsigned long long
#define _i64 signed long long
#define _u32 unsigned int
#define _i32 signed int
#define _u16 unsigned short
#define _i16 signed short
#define _u8 unsigned char
#define _i8 signed char
#define _x8 signed char
float Pi=3.141592654;
#define _small int
#define minfloat -1.0e20
#define maxfloat 1.0e20
union ARGB
{
	_u32 A;
	char c[4];
};
inline intl sqr(intl input)
{
	return input*input;
}
inline double fsqr(double input)
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
inline intl max(intl in1,intl in2)
{
	if (in1>in2) return in1; else return in2;
}
inline intl min(intl in1,intl in2)
{
	if (in1<in2) return in1; else return in2;
}
inline float fmax(float in1,float in2)
{
	if (in1>in2) return in1; else return in2;
}
inline float fmin(float in1,float in2)
{
	if (in1<in2) return in1; else return in2;
}
