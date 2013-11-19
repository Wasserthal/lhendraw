
double discriminant2(double a,double b,double c, double d, double e)
{
	return 256*a*a*a*e*e*e-192*a*a*b*d*e*e-128*a*a*c*c*e*e+144*a*a*c*d*d*e-27*a*a*d*d*d*d+144*a*b*b*c*e*e-6*a*b*b*d*d*e-80*a*b*c*c*d*e+18*a*b*c*d*d*d+16*a*c*c*c*c*e-4*a*c*c*c*d*d-27*b*b*b*b*e*e+18*b*b*b*c*d*e-4*b*b*b*d*d*d-4*b*b*c*c*c*e+b*b*c*c*d*d;
}
double delta0(double a,double b,double c, double d, double e)
{
	return c*c-3*b*d+12*a*e;
}
double delta1(double a,double b,double c, double d, double e)
{
	return 2*c*c*c-9*b*c*d+27*b*b*e+27*a*d*d-72*a*c*e;
}
double discriminant1(double idelta0,double idelta1)
{
	return idelta1*idelta1-4*idelta0*idelta0*idelta0;
}
double Q(double idiscriminant1,double idelta0,double idelta1)
{
	double wert1=(idelta1+sqrt(idiscriminant1))/2.0;
	double idiscriminant3=wert1;
	printf("dis3:%f\n",idiscriminant3);
	if (fabs(idelta0)<1e-50)
	{
		idiscriminant3=2*idelta1;
	}
	//ONE ALTERNATIVE
	if (fabs(idiscriminant3)<1e-50) return 0;//what if both zero!
	return pow(fabs(idiscriminant3),1.0/3.0)*((idiscriminant3>0)?1:-1);
	//OTHER ALTERNATIVE
	///there is no choice of root
}
double S(double a,double p,double iQ, double idelta0)
{
	float wert1=0.5*sqrt(-(2.0/3.0)*p+(iQ+(idelta0/iQ))/(3.0*a));
	return wert1;//Is this sufficient?
}
double S_trigonometric(double a,double ilittle_p,double idelta0,double idelta1)
{
	return 0.5*sqrt(-(2.0/3.0)*ilittle_p+(2/(3.0*a))*sqrt(idelta0)*cos(acos(idelta1/(2*sqrt(idelta0*idelta0*idelta0)))/3.0));
}
double little_p(double a,double b,double c)
{
	return (8*a*c-3*b*b)/(8*a*a);
}
double little_q(double a,double b,double c,double d)
{
	return (b*b*b-4*a*b*c+8*a*a*d)/(8*a*a*a);
}
int calculatecrossing(double a, double b, double iS, double ilittle_p, double ilittle_q,double *backval1,double *backval2,double *backval3,double *backval4)
{
	double discriminant;
	discriminant=-4*iS*iS-2*ilittle_p+(ilittle_q/iS);
	if (discriminant>=0)
	{
		*backval1=-b/(4*a)-iS+0.5*sqrt(discriminant);
		*backval2=-b/(4*a)-iS-0.5*sqrt(discriminant);
	}
	else
	{
		*backval1=nanf("");
		*backval2=nanf("");
	}
	discriminant=-4*iS*iS-2*ilittle_p-(ilittle_q/iS);
	if (discriminant>=0)
	{
		*backval3=-b/(4*a)+iS+0.5*sqrt(discriminant);
		*backval4=-b/(4*a)+iS-0.5*sqrt(discriminant);
	}
	else
	{
		*backval3=nan("");
		*backval4=nan("");
	}
}
double eqquart(double a,double b,double c,double d,double e,double x)
{
	return a*x*x*x*x+b*x*x*x+c*x*x+d*x+e;
}
int QUARTIC_quartic(double a,double b,double c,double d,double e,double * backval1,double * backval2,double * backval3,double * backval4)
{
	if (fabs(a)<1e-50)
	{
		//TODO: cubic needed!
	}
	double idelta0=delta0(a,b,c,d,e);
	double idelta1=delta1(a,b,c,d,e);
	double ilittle_p=little_p(a,b,c);
	double iS;
	double idiscriminant1=discriminant1(idelta0,idelta1);
	char alternate=0;
	double iQ;
	alternating:
	if (idiscriminant1>1e-50)
	{
		iQ=Q(idiscriminant1,idelta0,idelta1);
		if (abs(iQ)<1e-50)
		{
			*backval1=nan("");//can't happen
			*backval2=nan("");
			*backval3=nan("");
			*backval4=nan("");
			return -1;
		}
		iS=S(a,ilittle_p,iQ,idelta0);
	}
	else
	{
		if (fabs(idelta0)>1e-50)
		{
			iS=S_trigonometric(a,ilittle_p,idelta0,idelta1);
		}
		else
		{
			iS=0.5*sqrt(-(2.0/3.0)*ilittle_p);
		}
	}
	double ilittle_q=little_q(a,b,c,d);
	printf("dis2:%f\n",-27*discriminant2(a,b,c,d,e));
	printf("[%f,%f,%f,%f,%f,%f]\n",idelta0, idelta1, iQ, ilittle_p, iS, ilittle_q);
	calculatecrossing(a,b,iS,ilittle_p,ilittle_q,backval1,backval2,backval3,backval4);
}
