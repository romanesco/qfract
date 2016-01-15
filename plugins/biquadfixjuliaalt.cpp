/*
 * Biquadratic family with fixed point (phase space)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Biquad family (with fp 0)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=4;
double VALUE[N]={ 1.0, 0.0, 1.0, 0.0 };
const Parameter PARAM(N,VALUE);
const char* PARAMDESC[N] = { "Re(t)", "Im(t)", "Re(lambda)", "Im(lambda)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> lambda(param.Value(2),param.Value(3));
    complex<double> t(param.Value(0), param.Value(1)), a, b, c;
    if (t != 0.0) {
	a = lambda/t+t*t;
	b = 2.0*t;
	c = 1.0;
    } else { // quadratic polynomial
	a = 1.0;
	b = 0.0;
	c = 0.0;
    }
    complex<double> x(z.x, z.y);
    
    // first critical point
    register int i;
    for ( i=0; (i<max) && (norm(x)<10); i++ ) {
	x = x*(lambda+x*(a+x*(b+x*c)));
    }

    if (i>=max)
	return -1;
    else 
	return i;
}
    
Point map(Point z, Point c, Parameter param)
{
    complex<double> lambda(param.Value(2),param.Value(3));
    complex<double> t(param.Value(0), param.Value(1)), a, b, d;
    if (t != 0.0) {
	a = lambda/t+t*t;
	b = 2.0*t;
	d = 1.0;
    } else { // quadratic polynomial
	a = 1.0;
	b = 0.0;
	d = 0.0;
    }
    complex<double> x(z.x, z.y);
    x = x*(lambda+x*(a+x*(b+x*d)));
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
