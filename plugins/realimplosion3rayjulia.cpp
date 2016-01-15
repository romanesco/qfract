/*
 * For real parabolic implosion of cubic polynomials
 * z -> - z^3 +a z^2 + z + b, a,b: real
 * (dynamical space with external rays)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION tmp1 = (-re+a)*re+im*im; \
                 tmp2 = (-2*re+a)*im; \
		 tmp3 = (tmp1+1)*re-tmp2*im; \
                 im = (tmp1+1)*im+tmp2*re; \
		 re = tmp3 + b;

#define INCOND  ( (re2+im2)<1000 )

const char* NAME = "Cubic (real, with parambolic implosion, Julia set)";
const char* CHILD = "";
const char* COLORMAP = "per3-3.map";

const double XL = -1.2;
const double YT = 2.0;
const double XR = 2.3;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {1.85, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "a", "b" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double a=param.Value(0);
  register double b=param.Value(1);
  register double re2=re*re;
  register double im2=im*im;
  register double tmp1,tmp2,tmp3;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else 
    if ( re>0 )
      return i*3;
    else
      return i*3+1;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double a=param.Value(0);
  register double b=param.Value(1);
  register double tmp1,tmp2,tmp3;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
