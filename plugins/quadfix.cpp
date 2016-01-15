/*
 * Double Mandelbrot set
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Double Mandelbrot set";
const char* CHILD = "quadfixjulia.so";

const double XL = -2.0;
const double YT = 3.0;
const double XR = 4.0;
const double YB = -3.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const Parameter PARAM;

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
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
    complex<double> x=-0.5;
    
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
