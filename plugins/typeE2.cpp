/*
 * type E2: z -> cz(1+z/q)^q
 * with q=2 (dynamical space)
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
const char* CHILD = "typeE2julia.so";

const double q  = 2;

const double XL = -2.5;
const double YT = 6.0;
const double XR = 9.5;
const double YB = -6.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re  = -q/(q+1);
  register double im  = 0;
  register double cre = z.x;
  register double cim = z.y;
  register double re2 = re*re;
  register double im2 = im*im;

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
  register double re  = z.x;
  register double im  = z.y;
  register double cre = c.x;
  register double cim = c.y;

  register double tre, tim, tre2, tim2;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return Point(-q/(q+1),0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
