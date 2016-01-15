/*
 * Real cublc family
 * z -> z^3 - 3 a^2 z + b (real parameter space with stretching rays)
 */

#include "../plugin.h"
using namespace QFract;
#include <math.h>

#define RE2      re2=re*re;

#define INIT    register double A=a*a;

#define FUNCTION tmp1 = re2 - 3*A; \
		 re   = tmp1*re + b;

#define INCOND  ( re2<100000 )

const char* NAME = "Real cubic polynomials (parameter space with ray)";
const char* CHILD = "cubicrealjulia.so";

const double XL = 0.0;
const double YT = 1.3;
const double XR = 1.3;
const double YB = 0.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double a=z.x;
  register double b=z.y;

  INIT

  // first critical point
  register double re=a;

  register double re2=re*re;
  register double tmp1;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2
  }

  register double tmp=re*re;

  // second critical point
  re=-a;
  RE2;

  int j;
  for ( j=0; (j<max) && INCOND; j++ ) {
    FUNCTION
    RE2
  }

  if ((i>=max) || (j>=max))
    return -1;
  else {
    long double t1=log(log(tmp));
    long double t2=log(log(re*re));
    long double t3=log(3)*(j-i);
    return (int) ( (t2-t3-t1)*262144);
  }
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double a=c.x;
  register double b=c.y;
  register double re2=re*re;
  register double tmp1;

  INIT

  FUNCTION

  return Point(re,0);
}

Point init(Point c, Parameter param)
{
  return Point(c.x,0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
