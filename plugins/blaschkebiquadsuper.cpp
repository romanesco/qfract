/*
 * Biquadratic Blaschke product with superattrctive fixed point
 * z^2(z^2-a)(1-conj(a)z^2)
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*x*(x*x-l)/(1.0-lc*x*x);

#define INCOND  (norm(x)>0.000001)

const char* NAME = "Biquadratic Blaschke product w/ superattr fp";
const char* CHILD = "blaschkebiquadsuperjulia.so";

const double XL = -1.0;
const double YT = 1.0;
const double XR = 1.0;
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
    complex<double> l(z.x, z.y);
    complex<double> lc(z.x, -z.y);

    if (norm(l) > 1) return -1;

    double n=norm(l);
    complex<double> b=3.0+n, x=(1-sqrt(1-n))/lc;

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
    complex<double> l(z.x, z.y);
    complex<double> lc(z.x, -z.y);

    FUNCTION

    return Point(real(x), imag(x));
}

Point init(Point c, Parameter param) {
    complex<double> l(c.x, c.y);
    complex<double> lc(c.x, -c.y);

    double n=norm(l);
    complex<double> b=3.0+n, x=(1-sqrt(1-n))/lc;
    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD);
  return i;
}
}
