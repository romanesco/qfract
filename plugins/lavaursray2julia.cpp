/*
 * quadratic map with lavaurs map and external rays
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

const char* NAME = "Quadratic Map (Julia set with Lavaurs map & rays)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -1.75;
const double YT = 1.25;
const double XR = 0.75;
const double YB = -1.25;

const int MAXITER = 1000;
const int MAXORBIT = 1000;
const int N = 3;
double VALUE[N] = {0, 2, 0.01};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "epsilon" };

extern "C" {

complex<double> f(complex<double> z, double r, complex<double> a)
{
    if ( norm(z+r) < r*r )
    {
	z = -1.0/z;
	z -= log(z);
	z = z - 2.0/r
	    + a;
	z += log(-z);
	z = -1.0/z;
    } else {
	z=z*(1.0+z);
    }
    return z;
}

int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x,z.y);
    register int i;
    for ( i=0; (i<max) && (norm(x)<16); i++ ) {
	x=f(x,param.Value(2),complex<double>(param.Value(0),param.Value(1)));
    }
    
    if (i<max) {
	if ( imag(x) < 0 )
	    return i*3+11;
	return i*3;
    }
    else
	return -1;
}
    
Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x,z.y);
    x=f(x,param.Value(2),complex<double>(param.Value(0),param.Value(1)));

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
