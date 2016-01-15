/*
 * quartic map with parabolic fixed point of multiplier -1
 * z -> -z - az^3 + z^4
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "-z - az^3 + bz^4 (cp=1, parameter space)";
const char* CHILD = "quar-fix-p3julia.so";

const double XL = -0.2;
const double YT = 0.4;
const double XR = 0.6;
const double YB = -0.4;
const complex<double> cp(1.0);
const complex<double> I(0.0,1.0);

const int MAXITER = 500;
const int MAXORBIT = 10;
//double VALUE[]={};
const Parameter PARAM( 0, NULL );

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y), b=(1.0+3.0*a)/4.0;
    complex<double> x=cp;
    complex<double> tmp;

    register int i;
    for ( i=0; (i<max) && (norm(x) < 100 ) ; i++ ) {
	x=-x*(1.0 + x*x*(a - b*x));
    }
    
    if (i>=max)
	return -1;
    else
	return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y), b=(1.0+3.0*a)/4.0;
    complex<double> x(z.x, z.y);

    x=-x*(1.0 + x*x*(a - b*x));
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    return Point(1.0,0.0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD);
  return i;
}
}
