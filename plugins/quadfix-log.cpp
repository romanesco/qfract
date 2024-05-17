/*
 * Double Mandelbrot set (log coordinate)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Double Mandelbrot set (log coordinate)";
const char* CHILD = "quadfix-logjulia.so";

const double XL = -1.0;
const double YT = 0.1;
const double XR = 1.0;
const double YB = -1.9;

const int MAXITER = 500;
const int MAXORBIT = 10;
const Parameter PARAM;

const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    a = exp(2*M_PI*I*a);
    complex<double> x=-0.5;
    
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
    a = exp(2*M_PI*I*a);
    complex<double> x(z.x,z.y);
    
    x = a*x*(1.0+x);
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {
  return Point(-0.5, 0);
}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
