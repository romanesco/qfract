/*
 * Cubic polynomials with periodid critical value of period 3
 * f(z) = - (z-a) ((t^2+t+1)z - t)^2 / (t^2 a)
 * with a = t^2+1 ( cp1 = t/(a+t) -> 0 -> 1 -> a -> 0 )
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

#define INIT t2 = t*t; \
  a = t2+1.0;	       \
  den = a*t2; \
  b = a+t;

#define CRITINIT num = 2.0*a*b + t;

#define FUNCTION x = -(x-a)*(b*x-t)*(b*x-t)/den;

const char* NAME = "Cubic polyn w/ per 3 cv (parameter space)";
const char* CHILD= "cubicper3cvjulia.so";
const char* COLORMAP = "default.map";

const double XL = -2.1;
const double YT = 2.1;
const double XR = 2.1;
const double YB = -2.1;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 1;
double VALUE[N]={1};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Normalize J?" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> t(z.x, z.y);
  complex<double> x, a, b, t2, num, den;
  double R;
  
  INIT;
  
  R = abs(b);
  R = abs(den)/(R*R)*10;
  R = R*R;
  
  
  // critical point
  CRITINIT;
  x = num/(3.0*b);
  
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
  complex<double> t(c.x, c.y);
  complex<double> x(z.x,z.y), a, b, t2, den;
  INIT;
  
  FUNCTION;
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  complex<double> t(c.x, c.y);
  complex<double> x, a, b, t2, num, den;
  INIT;
  
  CRITINIT;
  x = num/den;
  
  return Point(real(x),imag(x));
}
  
PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
