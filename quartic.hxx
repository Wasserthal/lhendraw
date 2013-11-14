
float discriminant(float a,float b,float c, float d, float e)
{
	return 256*a*a*a*e*e*e-192*a*a*b*d*e*e-128*a*a*c*c*e*e+144*a*a*c*d*d*e-27*a*a*d*d*d*d+144*a*b*b*c*e*e-6*a*b*b*d*d*e-80*a*b*c*c*d*e+18*a*b*c*d*d*d+16*a*c*c*c*c*e-4*a*c*c*c*d*d-27*b*b*b*b*e*e+18*b*b*b*c*d*e-4*b*b*b*d*d*d-4*b*b*c*c*c*e+b*b*c*c*d*d;
}
float delta0(float a,float b,float c, float d, float e)
{
	return c*c-3*b*d+12*a*e;
}
float delta1(float a,float b,float c, float d, float e)
{
	return 2*c*c*c-9*b*c*d+27*b*b*e+27*a*d*d-72*a*c*e;
}
float Q(float delta1,float delta0)
{
	powf((delta1+sqrt(delta1*delta1-4*delta0*delta0*delta0))/2,1.0/3.0);
}
float S(float a,float p,float Q, float delta0)
{
	return 0.5*sqrt(-2*3*p+(Q+(delta0/Q))/(3*a));
}
float little_p(float a,float b,float c)
{
	return (8*a*c-3*b*b)/(8*a*a);
}
float little_q(float a,float b,float c,float d)
{
	return (b*b*b-4*a*b*c+8*a*a*d)/(8*a*a*a);
}
int calculatecrossing(float a, float b, float S, float little_p, float little_q,float &backval1,float &backval2)
{
	float discriminant;
	discriminant=-4*S*S-2*little_p+(little_q/S);
	int mode=1;
	if (discriminant<0)
	{
		discriminant=-4*S*S-2*little_p-(little_q/S);
		mode=2;
	}
	*backval1=-b/(4*a)+((mode==1)?(-S):(S))+0.5*sqrt(discriminant);
	*backval2=-b/(4*a)+((mode==1)?(-S):(S))-0.5*sqrt(discriminant);
}
