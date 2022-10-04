/*
 * Tangent map sin(z)+c (Julia set)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Tangent map (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default.map";

const double XL = -10.0;
const double YT = 10.0;
const double XR = 10.0;
const double YB = -10.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={1.0, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<long double> a(param.Value(0), param.Value(1));
    complex<long double> x(z.x,z.y);

    register int i;
    //for ( i=0; (i<max) && (!isinf(real(x))) && (!isinf(imag(x))); i++ ) {
    for ( i=0; (i<max) && (norm(x)<100); i++ ) {
      x = a * tan(x);
    }
    
    if (i>=max)
 	return -1;
    if (imag(x) > 0) {
      return i;
    } else {
      return i+33;
    }
}

Point map(Point z, Point c, Parameter param)
{
    complex<long double> a(param.Value(0), param.Value(1));
    complex<long double> x(z.x,z.y);

    register int j;
    x = a * tan(x);
    
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
