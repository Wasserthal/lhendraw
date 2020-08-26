//Some definitions you always need
#define intl long long
#ifndef stringlength
#define stringlength 512
#endif
#define bufferlength 512
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
#ifdef BITMODE32
#define _iXX signed int
#define _uXX unsigned int
#endif
#ifdef BITMODE64
#define _iXX signed long long
#define _uXX unsigned long long
#endif
float Pi=3.141592654;
#define _small int
#define minfloat -1.0e20
#define maxfloat 1.0e20
typedef struct _ARGB
{
	_u8 A;
	_u8 R;
	_u8 G;
	_u8 B;
}_ARGB;
typedef union _RGB
{
	_u32 W;
	_u8 a[4];
	_ARGB c;
}_RGB;
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
#ifndef _GLIBCXX_CMATH
inline float fmax(float in1,float in2)
{
	if (in1>in2) return in1; else return in2;
}
inline float fmin(float in1,float in2)
{
	if (in1<in2) return in1; else return in2;
}
#endif
