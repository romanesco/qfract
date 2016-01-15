/*
 * Cauliflower z^2+1/4
 */

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = 2*re*im; \
                 re = re2-im2 + 0.25;

#define INCOND  ( ( (re2+im2)<100 ) && (l > 0) )

const char* NAME = "Quadratic Map";
const char* CHILD = "";
const char* COLORMAP = "per2.map";

const double XL = -1.2;
const double YT = 1.2;
const double XR = 1.2;
const double YB = -1.2;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={100,2};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "# of critical orbits", "r1" };

static double r, r1;

extern "C" {
void paraminit( Parameter param )
{
    int i;
    r=0;

    for ( i=0; (i<param.Value(0)); i++) {
	r = r*r+0.25;
    }
    r = (0.5-r)/2;
    r1 = 1/param.Value(1)/2;
}

int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;
  register double l=re2+im2-2.0*(0.5-r)*re+0.25-r;

  register int i;
  int k=0;

  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
    l=re2+im2-2.0*(0.5-r)*re+0.25-r;
  }

  if (i<max) {
      if (l > 0)
	  return -1;
      if ( (re2+im2-re+2*r1*im+0.25 < 0) || (re2+im2-re-2*r1*im+0.25 < 0) )
	  k = 90;
      if ( im < 0 )
	  return (i%2)*50+k+1;
      return (i%2)*50+k;
  }
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;

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
