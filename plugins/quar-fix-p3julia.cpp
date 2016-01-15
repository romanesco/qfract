/*
 * quartic map with parabolic fixed point of multiplier -1
 * z -> -z - az^3 + z^4
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "-z - az^3 + bz^4 (cp=1, Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const complex<double> I(0.0,1.0);

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N]={1.0,0.0};
const Parameter PARAM( N, VALUE );
const char *PARAMDESC[N] = { "Re(a)", "Im(a)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> b=(1.0+3.0*a)/4.0, x(z.x,z.y);

    register int i;
    for ( i=0; (i<max) && (norm(x) < 100 ) && (norm(x-0.1*I)>0.01); i++ ) {
	x=-x*(1.0 + x*x*(a - b*x));
    }
    
    if (i>=max)
	return -1;
    else if (norm(x-0.1*I) <=0.01)
	return i+10;
    else
	return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> b=(1.0+3.0*a)/4.0, x(z.x,z.y);

    x=-x*(1.0 + x*x*(a - b*x));
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
