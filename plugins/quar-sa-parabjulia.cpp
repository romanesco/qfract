/*
 * Quartic map with superattractive and parabolic fixed points
 * (phase space)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Quartic map with superattr fp & parab fp (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.5;
const double YT = 3.0;
const double XR = 3.5;
const double YB = -3.0;
//const double CRE = 0.0;
//const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N]={-1.0, 0.0};
const Parameter PARAM(N,VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };

extern "C" {
// f(z) = z + z(z-1)^2(az-1) = z^2((a+2)+(-1-2*a)*z+z^2*a)
// a = (1/2)*(-4+3*c)/(-3*c+1+2*c^2)
int iter(Point z, Parameter param, int max)
{
    complex<double> c(param.Value(0),param.Value(1)), x(z.x, z.y), c2, a;
    register int i;
    
    a = (-4.0+3.0*c)/(1.0+(-3.0+2.0*c)*c)/2.0;
    for ( i=0; (i<max) && (norm(x)<100); i++ ) {
	x = x*x*((a+2.0)+((-1.0-2.0*a)+a*x)*x);
    }
    
    if (i>=max)
	return -1;
    else 
	return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> c1(param.Value(0), param.Value(1)), x(z.x,z.y), a;
    
    a = (-4.0+3.0*c1)/(1.0+(-3.0+2.0*c1)*c1)/2.0;
    x = x*x*((a+2.0)+((-1.0-2.0*a)+a*x)*x);

    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
