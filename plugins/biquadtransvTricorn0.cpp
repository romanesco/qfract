/*
 * Biquadratic family (transverse to the tricorn family)
 * z -> (z^2 + a)^2 + b with a = c+d, b = conj(c)-conj(d)
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

const char* NAME = "Biquadratic family (parallel to the tricorn)";
const char* CHILD = "biquadtransvTricorn.so";
const char* COLORMAP = "dim2-2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {0.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(d)", "Im(d)" };

extern "C" {int iter(Point z, Parameter param, int max)
{
  register double re=0.0;
  register double im=0.0;
  register double dre=param.Value(0);
  register double dim=param.Value(1);
  register double cre = z.x;
  register double cim = z.y;
  register double are =  z.x+dre;
  register double aim =  z.y+dim;
  register double bre =  z.x+2*dre;
  register double bim = -z.y+2*dim;
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

  if ( (dre == 0) && (dim == 0) ) {
    double cre2=cre*cre, cim2 = cim*cim;
    double normc = cre2+cim2, rec3 = cre2*cre-3*cre*cim2;
    double D = 256*( normc*normc + 2*rec3) + 288*normc - 27;
    if (D < 0) return 255;
  }
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
  register double dre=param.Value(0);
  register double dim=param.Value(1);
  register double are =  z.x+dre;
  register double aim =  z.y+dim;
  register double bre =  z.x+2*dre;
  register double bim = -z.y+2*dim;
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
