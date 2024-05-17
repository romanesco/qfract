/*
 * Cubic polynomial with preperiodic critical point (Julia set)
 * (z-a)^2(z+2a)+v
 * of preperiod 2, period 1
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Cubic Preper2(1) (Julia set)";
const char* CHILD = "";
//const char* COLORMAP = "per2-2.map";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={0.0, 0.0, 3, 100};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(c)", "Im(c)", "degree", "R" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> t = exp(a), s=1.0/t;
    complex<double> u = (t+s)/2.0;
    t = (t-s)/2.0;
    s = (1.0-u)/(t*u);
    a = (s-t)/3.0;
    complex<double> v = (2.0*t+s)/3.0;
    complex<double> x(z.x,z.y);

    register int i;
    for ( i=0; (i<max) && (norm(x) < 100 ); i++ ) {
      x = (x-a)*(x-a)*(x+2.0*a)+v;
    }
    
    if (i>=max)
	return -1;
    return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> t = exp(a), s=1.0/t;
    complex<double> u = (t+s)/2.0;
    t = (t-s)/2.0;
    s = (1.0-u)/(t*u);
    a = (s-t)/3.0;
    complex<double> v = (2.0*t+s)/3.0;
    complex<double> x(z.x,z.y);

    x = (x-a)*(x-a)*(x+2.0*a)+v;
    
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
