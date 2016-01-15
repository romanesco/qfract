/*
 * Cubic polynomial s.t. a -> -a
 * z -> z^3 - 3 a^2 z + 2 a^3 - a (dynamical space)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     register double a2re, a2im, bre, bim; \
		 a2re=are*are-aim*aim; a2im=2*are*aim; \
		 bre=2*(a2re*are-a2im*aim)-are;\
		 bim=2*(a2re*aim+a2im*are)-aim;

#define FUNCTION tmp1 = re2-im2 - 3*a2re; \
                 tmp2 = 2*re*im - 3*a2im; \
		 tmp3 = tmp1*re - tmp2*im + bre; \
		 im = tmp1*im + tmp2*re + bim; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Cubic (real)";
const char* CHILD = "cubicato-ajulia.so";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double are=z.x;
  register double aim=z.y;

  INIT

  // first critical point
  register double re=are;
  register double im=aim;

  register double re2=re*re;
  register double im2=im*im;
  register double tmp1, tmp2, tmp3;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else 
    return i;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;
  register double tmp1, tmp2, tmp3;
  register double are=c.x;
  register double aim=c.y;
  

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
    return c;
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
