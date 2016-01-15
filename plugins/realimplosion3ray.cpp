/*
 * For real parabolic implosion of cubic polynomials
 * z -> -z^3 + a z^2 + z + b 
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define CRIT1    re=(a+sqrt(a*a+3))/3; im=0;

#define CRIT2    re=(a-sqrt(a*a+3))/3; im=0;

#define FUNCTION tmp1 = (-re+a)*re+im*im; \
                 tmp2 = (-2*re+a)*im; \
		 tmp3 = (tmp1+1)*re-tmp2*im + b; \
                 im = (tmp1+1)*im+tmp2*re; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Cubic (real, with parabolic implosion)";
const char* CHILD = "realimplosion3rayjulia.so";

const double XL = 1.8;
const double YT = 0.05;
const double XR = 1.9;
const double YB = -0.05;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double a=z.x;
  register double b=z.y;

  register double re,im;
  register double re2, im2;

  // first critical point
  CRIT1;

  RE2IM2

  register double tmp1, tmp2, tmp3;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  // second critical point
  CRIT2;

  int j;
  for ( j=0; (j<max) && INCOND; j++ ) {
    FUNCTION
    RE2IM2
  }

  if ((i>=max) && (j>=max))
    return -1;
  else 
    return ((i<j) ? j*2: i*2+1);
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double a=c.x;
  register double b=c.y;
  register double tmp1, tmp2, tmp3;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
  register double re,im,a;
  a=param.Value(0);
  CRIT1;
  return Point(re,im);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
