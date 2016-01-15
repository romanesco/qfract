/*
 * Cubic parabolic map with Lavaurs map
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

const char* NAME = "Cubic Lavaurs Map";
const char* CHILD = "cubiclavaursjulia.so";

const double XL = -3;
const double YT = 3;
const double XR = 3;
const double YB = -3;

const int MAXITER = 1000;
const int MAXORBIT = 1000;
const int N = 1;
double VALUE[N] = {0.01};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "epsilon" };

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
	z += (1.0-A)*log(-z)+
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
    register complex<double> a(z.x,z.y), A=1.0/(a*a), x;
    register int i;

    double sgn = -real(a)/abs(real(a));
    x = (-a+sgn*sqrt(a*a-3.0))/3.0;
    for ( i=0; (i<max) && (norm(x)<16); i++ ) {
	x=f(x, a, param.Value(0), -(1.0-A)*iPi);
    }

    if (i<max) {
	if ( imag(x) < 0 )
	    return i*3+11;
	return i*3;
    }

    x = (-a-sgn*sqrt(a*a-3.0))/3.0;
    for ( i=0; (i<max) && (norm(x)<16); i++ ) {
	x=f(x, a, param.Value(0), -(1.0-A)*iPi);
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
    register complex<double> x(z.x,z.y), a(c.x,c.y), A=1.0/(a*a);
    x=f(x, a, param.Value(0), -(1.0-A)*iPi);

    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    register complex<double> a(c.x,c.y), A=1.0/(a*a), x;
    double sgn = -real(a)/abs(real(a));

    x = (-a+sgn*sqrt(a*a-3.0))/3.0;
    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
