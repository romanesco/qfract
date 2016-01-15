/*
 * quadratic maps az(1+z)
 * (log parameter space)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Double Mandelbrot set (in log coordinate)";
const char* CHILD = "quadfixexpjulia.so";

const double XL = -0.5;
const double YT = 0.1;
const double XR = 0.5;
const double YB = -0.9;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM;

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=-0.5;
    
    if ( z.y >= 0 ) return -1;

    a = exp(2.0*M_PI*a*sqrt((complex<double>) -1.0));

    register int i;
    for ( i=0; (i<max) && (norm(x)<10); i++ ) {
	x = a*x*(1.0+x);
    }

    if (i<max)
	return i;
    else
	return -1;
}
    
Point map(Point z, Point c, Parameter param)
{
    complex<double> a(z.x, z.y);
    complex<double> x=-0.5;
    
    a = exp(2.0*M_PI*a*sqrt((complex<double>) -1.0));
    x = a*x*(1.0+x);
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return Point(-c.x/2.0, -c.y/2.0);}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
