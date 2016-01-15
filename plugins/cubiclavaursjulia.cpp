/*
 * Cubic parabolic map with Lavaurs map
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

const char* NAME = "Cubic parabolic map with Lavaurs map (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2;
const double YT = 2;
const double XR = 2;
const double YB = -2;

const int MAXITER = 1000;
const int MAXORBIT = 1000;
const int N=3;
double VALUE[N] = {0, 2, 0.01};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "R" };

const complex<double> iPi(0,M_PI);

extern "C" {

complex<double> f(complex<double> z, complex<double> a,
		  double r, complex<double> phase) 
{
    complex<double> A=1.0/(a*a);
    complex<double> w = z*a;
    if ( norm(w+r) < r*r )
    {
	z = -1.0/w;
	z -= (1.0-A)*log(z)+0.5*(1.0+A)*(2.0*A-1.0)/z;
	z = z - 2.0/r + phase;
	z += (1.0-A)*log(-z) +
	    ( ((A-2.0)*A+1.0)*log(-z) + iPi + ((A+0.5)*A-0.5) )/z;
	z = -1.0/z;
	z /= a;
    } else {
	z=z*(1.0+z*(a+z));
    }
    return z;
}

int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x,z.y), a(param.Value(0),param.Value(1));
    complex<double> A=1.0/(a*a);
    register int i;
    for ( i=0; (i<max) && (norm(x)<16); i++ ) {
	x=f(x, a, param.Value(2), -(1.0-A)*iPi);
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
    complex<double> a(param.Value(0),param.Value(1)), A=1.0/(a*a);
    x=f(x, a, param.Value(2),-(1.0-A)*iPi);

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
