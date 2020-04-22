/*
 * Cubic polynomials with periodid critical value of period 3
 * f(z) = - (z-a) ((t^2+t+1)z - t)^2 / (t^2 a)
 * with a = t^2+1 ( cp1 = t/(a+t) -> 0 -> 1 -> a -> 0 )
 * (normalized to monic family with rays)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const complex<double> I(0,1);

#define INIT t2 = t*t; \
  a = t2+1.0;	       \
  den = a*t2; \
  b = a+t; \

#define UNNORMALIZE x = x * sqrt(a)*t*I/b;
#define NORMALIZE x = x * b /(sqrt(a)*t*I);


#define CRITINIT num = 2.0*a*den + t;

#define FUNCTION x = -(x-a)*(b*x-t)*(b*x-t)/den;

const char* NAME = "Cubic polyn w/ per 3 cv (phase space)";
const char* CHILD= "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={-1,0};
const Parameter PARAM( N,  VALUE );
const char* PARAMDESC[N] = { "Re(t)", "Im(t)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> t(param.Value(0), param.Value(1));
  complex<double> x(z.x, z.y), a, b, t2, num, den;
  double R;
  
  INIT;

  UNNORMALIZE;
  
  R = abs(b);
  R = abs(den)/(R*R)*100;
  R = R*R;

  register int i;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    FUNCTION;
  }
  
  if (i>=max) {
    return -1;
  }

  NORMALIZE;

  if (imag(x) > 0) {
    return i*2;
  } else {
    return i*2+21;
  }
}
  
Point map(Point z, Point c, Parameter param)
{
  complex<double> t(param.Value(0), param.Value(1));
  complex<double> x(z.x,z.y), a, b, t2, den;
  INIT;
  
  UNNORMALIZE;

  FUNCTION;

  NORMALIZE;
  
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
