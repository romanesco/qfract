/*
 * Cubic polynomial with fixed point
 * z -> z^3 - a z^2 + b z(parameter space, b fixed)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     

#define FUNCTION tmp1 = (re-are)*re-(im-aim)*im; \
                 tmp2 = (re-are)*im+(im-aim)*re; \
		 tmp3 = (tmp1+bre)*re - (tmp2+bim)*im; \
		 im = (tmp1+bre)*im + (tmp2+bim)*re; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<10 )

// critical points are (a +- c)/3 where c=sqrt(a^2-3b)
#define CRITINIT   tmp1=(are*are-aim*aim)-3*bre; \
                   tmp2=2*are*aim-3*bim; \
                   tmp3=((tmp3 = 0.5*(sqrt(tmp1*tmp1+tmp2*tmp2)+tmp1)) \
			 >= 0.0 ) ? tmp3 : -tmp3; \
                   cim =sqrt(((tmp1 = tmp3 - tmp1) >= 0.0 ) ? tmp1 : -tmp1); \
                   cim =(tmp2 >= 0.0) ? cim : -cim; \
                   cre = sqrt(tmp3);

#define CRIT1      re=(are + cre)/3.0; im=(aim + cim)/3.0;

#define CRIT2      re=(are - cre)/3.0; im=(aim - cim)/3.0;

const char* NAME = "Cubic (with fix pt 0)";
const char* CHILD = "cubicfixjulia.so";
const char* COLORMAP = "default3.map";

const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {1.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(b)", "Im(b)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double are=z.x;
  register double aim=z.y;
  register double bre=param.Value(0);
  register double bim=param.Value(1);

  INIT

  register double tmp1, tmp2, tmp3;
  register double cre,cim, re, im;

  CRITINIT
  //first critical point
  CRIT1

  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  //second critical point
  CRIT2
  RE2IM2
  register int j;
  for ( j=0; (j<max) && INCOND; j++ ) {
    FUNCTION
    RE2IM2
  }

  if ((i>=max) && (j>=max))
    return -1;
  else 
    return ((i<j) ? i*2: j*2+1);
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double tmp1, tmp2, tmp3;
  register double are=c.x;
  register double aim=c.y;
  register double bre=param.Value(0);
  register double bim=param.Value(1);

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
  register double are=c.x;
  register double aim=c.y;
  register double bre=param.Value(0);
  register double bim=param.Value(1);
  register double tmp1,tmp2,tmp3,cre,cim,re,im;
  CRITINIT
  CRIT1
  return Point(re,im);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
