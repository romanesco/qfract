/*
 * Real biquadratic family
 * z -> (z^2 + a)^2 + b (real parameter space with stretching rays)
 */

#include "../plugin.h"
using namespace QFract;
#include <cmath>

#define RE2      re2=re*re;

#define FUNCTION re = re2 + a; \
                 re = re*re + b;

#define INCOND  ( re2<100000 )

const char* NAME = "Swallow (real biquadratic maps)";
const char* CHILD = "biquadrealjulia.so";

const double XL = 0.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = 0.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register long double re=0.0;
  register long double a=z.x;
  register long double b=z.y;
  register long double re2=re*re;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2
  }

  long double tmp=re*re;

  int j=i;

  re=z.y;
  re2=re*re;

  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2
  }

  if ((i>=max) || (j>=max))
    return -1;
  else {
    long double t1=log(log(tmp));
    long double t2=log(log(re*re));
    long double t3=log(4)*(j-i);
    return (int) ( (t2+t3-t1)*262144 );
  }
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double a=c.x;
  register double b=c.y;
  register double re2=re*re;

  FUNCTION

  return Point(re,0);
}

Point init(Point c, Parameter param) {return Point(c.y,0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
