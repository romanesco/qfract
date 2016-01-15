/*
 * quadratic map with period 4 external rays
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <cmath>

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = 2*re*im + cim; \
                 re = re2-im2 + cre;

#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Quaedratic Map (Julia set with period 4 rays)";
const char* CHILD = "";
const char* COLORMAP = "per2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {.282271390766913879696817539321,
                          .530060617578525299487869047520};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };

const double slope1=tan(2*M_PI*1/15);
const double slope2=tan(2*M_PI*2/15);
const double slope3=tan(2*M_PI*4/15);
const double slope4=tan(2*M_PI*8/15);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double cre=param.Value(0);
  register double cim=param.Value(1);
  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i<max) {
      if ( ((slope1*re < im) && (slope2*re > im)) ||
	   ((slope3*re > im) && (slope4*re < im)) )
      return i*5;
    return i*5+1;
  }
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double cre=param.Value(0);
  register double cim=param.Value(1);
  register double re2=re*re;
  register double im2=im*im;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
