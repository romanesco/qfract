/*
 * Cubic polynomial with superattractive fixed point
 * z -> z^3 - a z^2 (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     

#define FUNCTION tmp1 = re2-im2; \
                 tmp2 = 2*re*im; \
		 tmp3 = tmp1*(re - are) - tmp2*(im - aim); \
		 im = tmp1*(im - aim) + tmp2*(re - are); \
		 re = tmp3;

#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Cubic with superattr fix pt (parameter space)";
const char* CHILD = "cubicsuperrayjulia.so";

const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;
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
  register double re=2*are/3;
  register double im=2*aim/3;

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
  else { 
      if (im > 0)
	  return i*4;
      else
	  return i*4+11;
  }
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
    return c.multiply(2.0/3.0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
