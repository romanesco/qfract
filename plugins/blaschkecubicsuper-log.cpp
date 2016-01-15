/*
 * Cubic Blaschke product with superattrctive fixed point
 * z^2(z-a)(1-conj(a)z), a=exp(2*pi*c)
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*x*(x-l)/(1.0-lc*x);

#define INCOND  (norm(x)>0.000001)

const char* NAME = "Cubic Blaschke product w/ superattr fp";
const char* CHILD = "blaschkecubicsuper-logjulia.so";

const double XL = -2.0;
const double YT = 1.0;
const double XR = 0.0;
const double YB = -1.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 15;
const int MAXORBIT = 10;
double VALUE[0]={};
const Parameter PARAM( 0, VALUE );

extern "C" {

int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y), l=exp(2*M_PI*a);
    complex<double> lc=conj(l);

    if (z.x > 0) return -1;

    double n=norm(l);
    complex<double> b=3.0+n, x=b/lc*(1.0-sqrt(1.0-16.0*n/(b*b)))/4.0;
    
    register int i;
    for ( i=0; (i<max) && INCOND; i++ ) {
	FUNCTION
    }

    if (i<max) {
	if (imag(-x) >=0)
	    return i*10;
	else 
	    return i*10+65;
    }
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x, z.y);
    x=exp(2*M_PI*x);
    complex<double> a(c.x, c.y),l=exp(2*M_PI*a);
    complex<double> lc=conj(l);

    FUNCTION

    x=log(x)/(2*M_PI);
    return Point(real(x), imag(x));
}

Point init(Point c, Parameter param) {
    complex<double> a(c.x, c.y),l=exp(2*M_PI*a);
    complex<double> lc=conj(l);

    double n=norm(l);
    complex<double> b=3.0+n, x=b/lc*(1.0-sqrt(1.0-16.0*n/(b*b)))/4.0;
    x=log(x)/(2*M_PI);
    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD);
  return i;
}
}
