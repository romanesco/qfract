/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (parameter space with external rays)
 */

#include <cmath>
#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = -2*re*im + aim; \
                 re = re2-im2 + are;
/*
#define FUNCTION im = 2*re*im - aim; \
                 re = re2-im2 + are; \
		 RE2IM2 \
		 im = 2*re*im + aim; \
                 re = re2-im2 + are;
*/

#define INCOND  ( (re2+im2)<R2 )

const char* NAME = "Tricorn (parameter space)";
const char* CHILD = "tricorn-rayjulia.so";

const double XL = -2.5;
const double YT = 2.0;
const double XR = 1.5;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 1;
double VALUE[N] = {100};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "escape radius" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=0.0;
  register double im=0.0;
  register double are=z.x;
  register double aim=z.y;
  register double re2=re*re;
  register double im2=im*im;

  double R2 = param.Value(0);
  R2 *= R2;
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else if (im >= 0) 
    return log(i)*8;
  else
    return log(i)*8+21;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double are=c.x;
  register double aim=c.y;
  register double re2=re*re;
  register double im2=im*im;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return Point(0,0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
