/*
 * type E2: z -> cz(1+z/q)^q
 * with q=2 (parameter space)
 */

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION tre = 1 + re/q; \
                 tim = im/q; \
		 tre2 = tre*tre; \
		 tim2 = tim*tim; \
		 tim = 2*tre*tim; \
		 tre = tre2 - tim2; \
		 tre2 = tre * re - tim * im; \
		 tim2 = tre * im + tim * re; \
		 re = tre2 * cre - tim2 * cim; \
		 im = tre2 * cim + tim2 * cre;

#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Polynomial of type E2";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double q  = 2;

const double XL = -4.0;
const double YT = 3.0;
const double XR = 2.0;
const double YB = -3.0;
const double CRE = 2.0;
const double CIM = 0.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={2.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double cre=param.Value(0);
  register double cim=param.Value(1);
  register double re2=re*re;
  register double im2=im*im;

  register double tre, tim, tre2, tim2;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
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
  register double cre=param.Value(0);
  register double cim=param.Value(1);

  register double tre, tim, tre2, tim2;

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
