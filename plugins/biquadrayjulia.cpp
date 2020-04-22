/*
 * Biquadratic family
 * z -> (z^2 + a)^2 + b (dynamical space)
 */

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = 2*re*im + aim; \
                 re = re2-im2 + are; \
		 RE2IM2 \
		 im = 2*re*im + bim; \
                 re = re2-im2 + bre;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Biquadratic family (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "per2.map";

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

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else 
    if (im > 0) {
      return i*6;
    } else {
      return i*6+1;
    }
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
