/*
 * Cubic polynomials Per_2(lambda)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

#define INIT   a = t + 2.0 + l/t;		\
  b = -( 2.0*t + 3.0 + l/t);			\
  R=100/sqrt(norm(a));

#define FUNCTION x = ((a*x + b)*x + t)*x + 1.0;

const char* NAME = "Cubic polyn Per_2(lambda) (Julia set)";
const char* CHILD= "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={0, 0, 0, 0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N];

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> l(param.Value(2), param.Value(3));
  complex<double> x(z.x,z.y), a, b, t(param.Value(0), param.Value(1));
  double R;

  INIT;

  x = x/sqrt(a);

  register int i,j;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    FUNCTION;
  }
  
  if (i>=max) {
    return -1;
  } else  {
    return i;
  }
}
  
Point map(Point z, Point c, Parameter param)
{
  complex<double> l(param.Value(2), param.Value(3));
  complex<double> x(z.x,z.y), a, b, t(param.Value(0), param.Value(1));
  double R;
  INIT;

  x = x/sqrt(a);

  FUNCTION;
  
  x = x*sqrt(a);
  
  return Point(real(x),imag(x));
}
  
Point init(Point c, Parameter param) 
{
  return c;
}
  
PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
