/*
 * Cubic polynomials
 * z -> z^3 - 3 a^2 z + 2 a^3 - a (dynamical space with a fixed)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     register double Are, Aim; \
		 Are=3*(are*are-aim*aim);\
		 Aim=6*are*aim;

#define FUNCTION tmp1 = re2-im2 - Are; \
                 tmp2 = 2*re*im - Aim; \
		 tmp3 = tmp1*re - tmp2*im + bre; \
		 im = tmp1*im + tmp2*re + bim; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Cubic (a fixed)";
const char* CHILD = "cubic-a-ray.so";
const char* COLORMAP = "dim2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {0.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)" };

const double slope=-sqrt(3);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double are=param.Value(0);
  register double aim=param.Value(1);
  register double bre=z.x;
  register double bim=z.y;

  INIT

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
  else if (i>=max)
    return (j%15)+1;
  else if (j>=max)
    return ((i%15)+1)*16;
  else 
    return ((i%15)+1)*16+(j%15)+1;
  // return ((i<j) ? j*2: i*2+1);
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;
  register double are=param.Value(0);
  register double aim=param.Value(1);
  register double bre=c.x;
  register double bim=c.y;
  register double tmp1, tmp2, tmp3;

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
    return c;
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
