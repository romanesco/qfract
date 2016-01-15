/*
 * Quadratic rational map with period 2 cycle 0 <-> \infty
 * z -> 1/4*c/(z^2-z)
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION1 tmp1 = re-1; \
                  tmp2 = (tmp1+re)*im; \
                  tmp3 = tmp1*re-im2; \
		  tmp1 = tmp2*tmp2+tmp3*tmp3; \

#define FUNCTION2 im = (tmp3*cim-tmp2*cre)/tmp1; \
		  re = (tmp3*cre+tmp2*cim)/tmp1;

#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Rational with degree 2, period 2 cycle (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -1.5;
const double YT = 2.0;
const double XR = 2.5;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {0.0, 1.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "4Re(c)", "4Im(c)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double cre=(param.Value(0))/4 ;
  register double cim=(param.Value(1))/4;
  register double tmp1;
  register double tmp2;
  register double tmp3;

  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION1
    if (tmp1 == 0) { i++;break; }
    FUNCTION2
    RE2IM2
  }

  if (i<max)
    return i;
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double cre=(param.Value(0))/4;
  register double cim=(param.Value(1))/4;
  //register double re2=re*re;
  register double im2=im*im;

  register double tmp1;
  register double tmp2;
  register double tmp3;


  FUNCTION1
  if (tmp1 == 0) return Point(100000, 100000);
  FUNCTION2
    
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
