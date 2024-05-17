/*
 * Cubic polynomial with fixed critical value
 * (Critical value v is periodic of period 2)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Cubic Preper1(2) (Julia set)";
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

const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> b = a+I;
    complex<double> x(z.x,z.y);
    complex<double> tmp;
    
    register int i;
    for ( i=0; (i<max) && (norm(x) < 100 ); i++ ) {
      tmp = x-a;
      tmp *= tmp;
      x = tmp*(x+2.0*a)+b;
    }
    
    if (i>=max)
	return -1;
    return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> b = a+I;
    complex<double> x(z.x,z.y);
    complex<double> tmp;

    tmp = x-a;
    tmp *= tmp;
    x = tmp*(x+2.0*a)+b;
    
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
