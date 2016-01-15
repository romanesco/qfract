/*
 * Cubic polynomial s.t. a -> -a
 * z -> z^3 - 3 a^2 z + 2 a^3 - a (parameter space with period 3 rays)
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

#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Cubic (f(a)=-a, with period 3 rays)";
const char* CHILD = "cubicato-arayper3julia.so";
const char* COLORMAP = "per3.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

const double slope1=tan(2*M_PI*7/26);
const double slope2=tan(2*M_PI*11/26);
const double slope3=tan(2*M_PI*21/26);

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

  if (i<max) {
    if ( (slope1*re > im) && (slope2*re < im) )
      return i*4;
    if ( (slope2*re > im) && (slope3*re > im) )
      return i*4+1;
    return i*4+2;
  }
  return -1;
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
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
