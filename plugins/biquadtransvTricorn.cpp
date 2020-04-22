/*
 * Biquadratic family (transverse to the tricorn family)
 * z -> (z^2 + a)^2 + b with a = c+d, b = conj(c)+d
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

const char* NAME = "Biquadratic family (transverse to the tricorn)";
const char* CHILD = "biquadtransvTricornjulia.so";
const char* COLORMAP = "dim2-2.map";

const double XL = -0.0;
const double YT = 0.1;
const double XR = 0.2;
const double YB = -0.1;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {0.25, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };

extern "C" {int iter(Point z, Parameter param, int max)
{
  register double re=0.0;
  register double im=0.0;
  register double cre=param.Value(0);
  register double cim=param.Value(1);
  register double are =  cre+z.x;
  register double aim =  cim+z.y;
  register double bre =  cre+2*z.x;
  register double bim = -cim+2*z.y;
  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  int j=i;

  re=bre; im=bim;
  re2=re*re; im2=im*im;

  for ( i=0; (i<max) && INCOND; i++ ) {
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
  register double cre=param.Value(0);
  register double cim=param.Value(1);
  register double are =  cre+c.x;
  register double aim =  cim+c.y;
  register double bre =  cre+2*c.x;
  register double bim = -cim+2*c.y;
  register double re2=re*re;
  register double im2=im*im;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return Point(0,0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
