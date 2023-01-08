/*
 * semigroup generated by z^2 and a(z-c)^2+c
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Semigroup generated by z^2 and a(z-c)^2+c";
const char* CHILD = "";
const char* COLORMAP = "per2-2.map";
//const char* COLORMAP = "default.map";

const double XL = -10.0;
const double YT = 10.0;
const double XR = 10.0;
const double YB = -10.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 5;
double VALUE[N]={5.0, 0.0, 1.0, 0.0, 4};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(c)", "Im(c)", "Re(a)", "Im(a)", "K" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> b(param.Value(0), param.Value(1)),
    a(param.Value(2), param.Value(3));
  complex<double> x(z.x,z.y), y;
  double R=param.Value(3)*param.Value(3)*norm(b);
  
  register int i,j;
  bool mapind;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    mapind=0;
    y=x*x;
    if (norm(y)>= R) {
      y = a*(x-b)*(x-b)+b;
      mapind=1;
    }
  }

  if (i>=max)
    return -1;
  return i*2+21*mapind;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> b(param.Value(0), param.Value(1)),
    a(param.Value(2), param.Value(3));
  complex<double> x(z.x,z.y), y;
  double R=param.Value(3)*param.Value(3)*norm(b);
  
  if (norm(x)>=R) return z;
  
  y=x*x;
  if (norm(y)>= R) {
    y = a*(x-b)*(x-b)+b;
  }
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
