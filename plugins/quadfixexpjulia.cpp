/*
 * quadratic maps az(1+z) (phase space)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Quadratic Map (Julia set with fixed point 0)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N]={0.0, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(log(a))/(2*pi*i)", 
			     "Im(log(a))/(2*pi*i)" };

extern "C" {

int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x, z.y);

    complex<double> t(-param.Value(1),param.Value(0));
    complex<double> a=exp(2.0*M_PI*t);

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
    complex<double> x(z.x, z.y);

    complex<double> t(-param.Value(1),param.Value(0));
    complex<double> a=exp(2.0*M_PI*t);

    x = a*x*(1.0+x);
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
