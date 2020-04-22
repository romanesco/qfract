/*
 * Cubic polynomials with fixed critical value
 * f(z) = a(z^3-z^2) + 1
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

#define FUNCTION x = a*x*x*(x-1.0)+1.0;

#define UNNORMALIZE x = x/sqrt(a);
#define NORMALIZE x = sqrt(a)*x;

const char* NAME = "Cubic polyn w/ fix 3 cv (phase space)";
const char* CHILD= "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 3;
double VALUE[N]={-1,0, 1};
const Parameter PARAM( N,  VALUE );
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "Normalize?" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(param.Value(0), param.Value(1));
  complex<double> x(z.x, z.y);
  double R = 100.0/sqrt(abs(a));
  
  if (param.Value(2)) {
    UNNORMALIZE;
  }

  register int i;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    FUNCTION;
  }
  
  if (i>=max) {
    return -1;
  }
  
  return i;
}
  
Point map(Point z, Point c, Parameter param)
{
  complex<double> a(param.Value(0), param.Value(1));
  complex<double> x(z.x,z.y);
  
  if (param.Value(2)) {
    UNNORMALIZE;
  }

  FUNCTION;
  
  if (param.Value(2)) {
    NORMALIZE;
  }

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
