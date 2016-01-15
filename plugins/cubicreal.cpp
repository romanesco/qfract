/*
 * Real cubic family
 * z -> z^3 - 3 a^2 z + b (real parameter space with A=a^2 and B=b^2)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT1    register double are,aim; \
                 if (A>=0) { \
		   are=sqrt(A); aim=0; \
                 } else { \
		   are=0; aim=sqrt(-A); \
                 }

#define INIT2    register double bre, bim; \
                 if (B>=0) { \
		   bre=sqrt(B); bim=0; \
                 } else { \
		   bre=0; bim=sqrt(-B); \
                 }

#define FUNCTION tmp1 = re2-im2 - 3*A; \
                 tmp2 = 2*re*im; \
		 tmp3 = tmp1*re - tmp2*im + bre; \
		 im = tmp1*im + tmp2*re + bim; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Real cubic polynomial (parameter space)";
const char* CHILD = "cubicrealjulia.so";
const char* COLORMAP = "per2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double A=z.x;
  register double B=z.y;

  INIT1
  INIT2

  // first critical point
  register double re=are;
  register double im=aim;

  register double re2=re*re;
  register double im2=im*im;
  register double tmp1, tmp2, tmp3;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  // second critical point
  re=-are; im=-aim;
  RE2IM2;

  int j;
  for ( j=0; (j<max) && INCOND; j++ ) {
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
  register double A=c.x;
  register double B=c.y;
  register double re2=re*re;
  register double im2=im*im;
  register double tmp1, tmp2, tmp3;

  INIT2
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
  if (c.y >= 0)
    return Point(sqrt(c.x),0);
  else
    return Point(0,sqrt(c.x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
