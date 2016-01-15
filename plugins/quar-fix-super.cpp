/*
 * quartic map with superattractive fixed point and a fixed point
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Quartic Map (0: fp, 1: safp, parameter space)";
const char* CHILD = "quar-fix-superjulia.so";

const double XL = -10.0;
const double YT = 10.0;
const double XR = 10.0;
const double YB = -10.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N]={(sqrt(5)-1)/2, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(log(a))/(2*pi*i)",
			     "Im(log(a))/(2*pi*i)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> b(-param.Value(1), param.Value(0));
    complex<double> a=exp(2*M_PI*b);
    b=complex<double>(z.x, z.y);
    double R=10000;
    complex<double> p, tmp1, tmp2, tmp3;

    register int i,j;
    tmp1 = 3.0*(a-2.0)-2.0*b;
    tmp2 = sqrt(tmp1*tmp1 + 16.0*b*a);
    tmp3 = 8.0*b;
    // cp1
    p = (-tmp1+tmp2)/tmp3;

    for ( i=0; (i<max) && (norm(p) < R ); i++ ) {
	p = (p-1.0)*(p-1.0)*((b*p+(a-2.0))*p-1.0)+1.0;
    }
    
    // cp2
    p = (-tmp1-tmp2)/tmp3;

    for ( j=0; (j<max) && (norm(p) < R ); j++ ) {
	p = (p-1.0)*(p-1.0)*((b*p+(a-2.0))*p-1.0)+1.0;
    }

    if ( (i>=max) && (j>=max) )
	return -1;
    else if (i < j)
	return j*8+1;
    else
	return i*8;
	
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> b(-param.Value(1), param.Value(0));
    complex<double> a=exp(2*M_PI*b);
    b = complex<double>(c.x, c.y);
    complex<double> p(z.x, z.y);
    
	p = (p-1.0)*(p-1.0)*((b*p+(a-2.0))*p-1.0)+1.0;
	
    return Point(real(p),imag(p));
}

Point init(Point c, Parameter param) 
{
    complex<double> b(-param.Value(1), param.Value(0));
    complex<double> a=exp(2*M_PI*b);
    b = complex<double>(c.x, c.y);

    complex<double> p, tmp1,tmp2,tmp3;

    tmp1 = 3.0*(a-2.0)-2.0*b;
    tmp2 = sqrt(tmp1*tmp1 + 16.0*b*a);
    tmp3 = 8.0*b;
    // cp1
    p = (-tmp1+tmp2)/tmp3;

    return Point(real(p), imag(p));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
