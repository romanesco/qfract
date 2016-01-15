/*
 * quadratic map with lavaurs map and external rays
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

const char* NAME = "Quadratic Map (parameter space with Lavaurs map & rays)";
const char* CHILD = "lavaursrayjulia.so";

const double XL = -0.75;
const double YT = 3.5;
const double XR = 0.75;
const double YB = 2.0;

const int MAXITER = 1000;
const int MAXORBIT = 1000;
const int N = 1;
double VALUE[N] = {0.01};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "epsilon" };

extern "C" {

complex<double> f(complex<double> z, double r, complex<double> a)
{
    if ( norm(z+r) < r*r )
    {
	z = -1.0/z;
	z = z - 2.0/r
	    + a;
	z = -1.0/z;
    } else {
	z=z*(1.0+z);
    }
    return z;
}

int iter(Point z, Parameter param, int max)
{
    complex<double> c(z.x,z.y), x(-0.5,0);
    register int i;
    for ( i=0; (i<max) && (norm(x)<16); i++ ) {
	x=f(x, param.Value(0), c);
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
    x=f(x,param.Value(0),complex<double>(c.x,c.y));

    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return Point(-0.5,0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
