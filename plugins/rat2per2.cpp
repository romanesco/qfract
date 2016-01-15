/*
 * Quadratic rational map with period 2 cycle 0 <-> \infty
 * z -> 1/4*c/(z^2-z)
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     re = 0.5; \
                 im = 0; \
		 cre /= 4; \
		 cim /= 4;

#define FUNCTION1 tmp1 = re-1; \
                  tmp2 = (tmp1+re)*im; \
                  tmp3 = tmp1*re-im2; \
		  tmp1 = tmp2*tmp2+tmp3*tmp3; \

#define FUNCTION2 im = (tmp3*cim-tmp2*cre)/tmp1; \
		  re = (tmp3*cre+tmp2*cim)/tmp1;


#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Rational with degree 2, period 2 cycle (parameter space)";
const char* CHILD = "rat2per2julia.so";

const double XL = -2.0;
const double YT = 1.5;
const double XR = 1.0;
const double YB = -1.5;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double cre=re;
  register double cim=im;
  register double tmp1;
  register double tmp2;
  register double tmp3;

  if ((cre == 0) && (cim == 0)) return 0;
  INIT

  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION1
    if (tmp1 == 0) { i++; break;}
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
  register double cre=c.x/4;
  register double cim=c.y/4;

  register double tmp1;
  register double tmp2;
  register double tmp3;

  //register double re2=re*re;
  register double im2=im*im;

  FUNCTION1
  if (tmp1 == 0) return Point(100000, 100000);
  FUNCTION2
    
  return Point(re,im);
}

Point init(Point c, Parameter param)
{
  register double re;
  register double im;
  register double cre=c.x;
  register double cim=c.y;
  //register double tmp1;

  if ((cre == 0) && (cim == 0)) return Point(100000, 100000);
  INIT

  return Point(re,im);
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
