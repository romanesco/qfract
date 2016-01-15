/*
 * Biquadratic family with fixed point (parameter space)
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
const char* PARAMDESC[N] = { "Re(b)", "Im(b)", "Re(lambda)", "Im(lambda)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> lambda(param.Value(2),param.Value(3));
    complex<double> a, b(param.Value(0), param.Value(1));
    if ( lambda != 0.0 ) {
	b /= (lambda*sqrt(lambda));
	if ( real(b) >= 0 )
	    a = b + sqrt(b*b-1.0);
	else
	    a = b - sqrt(b*b-1.0);
	a *= sqrt(lambda);
    }
    complex<double> x(z.x, z.y), y;
    
    // first critical point
    register int i;
    for ( i=0; (i<max) && (norm(x)<10); i++ ) {
	y = x*(1.0+x);
	x = lambda*y*(1.0+a*y);
    }

    if (i>=max)
	return -1;
    else 
	return i;
}
    
Point map(Point z, Point c, Parameter param)
{
    complex<double> lambda(param.Value(2),param.Value(3));
    complex<double> a, b(param.Value(0), param.Value(1));
    if ( lambda != 0.0 ) {
	b /= (lambda*sqrt(lambda));
	if ( real(b) >= 0 )
	    a = b + sqrt(b*b-1.0);
	else
	    a = b - sqrt(b*b-1.0);
	a *= sqrt(lambda);
    }
    complex<double> x(z.x, z.y), y;
    
    y = x*(1.0+x);
    x = lambda*y*(1.0+a*y);
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
