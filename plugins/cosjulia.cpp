/*
 * Cosine map sin(z)+c (Julia set)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Cosine map (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default.map";

const double XL = -5.0;
const double YT = 5.0;
const double XR = 5.0;
const double YB = -5.0;
const double CRE = 1.0;
const double CIM = 0.0;

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
    for ( i=0; (i<max) && (imag(x)<1e+10) && (imag(x)>-1e+10); i++ ) {
      x = a * cos(x);
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
    x = a * cos(x);
    
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
