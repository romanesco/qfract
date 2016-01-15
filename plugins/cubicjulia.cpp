/*
 * Cubic polynomials with fixed point
 * z -> z^3 - 3 a^2 z + b (dynamical space)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     register double Are, Aim; \
		 Are=3*(are*are-aim*aim);\
		 Aim=6*are*aim;

#define FUNCTION tmp1 = re2-im2 - Are; \
                 tmp2 = 2*re*im - Aim; \
		 tmp3 = tmp1*re - tmp2*im + bre; \
		 im = tmp1*im + tmp2*re + bim; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Cubic (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N] = {0.0, 0.0, 0.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "Re(b)", "Im(b)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double are=param.Value(0);
  register double aim=param.Value(1);
  register double bre=param.Value(2);
  register double bim=param.Value(3);
  register double re2=re*re;
  register double im2=im*im;
  register double tmp1,tmp2,tmp3;

  INIT

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else 
    return i*2;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double are=param.Value(0);
  register double aim=param.Value(1);
  register double bre=param.Value(2);
  register double bim=param.Value(3);
  register double re2=re*re;
  register double im2=im*im;
  register double tmp1,tmp2,tmp3;

  INIT

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
