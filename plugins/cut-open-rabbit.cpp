/*
 * quadratic maps az(1+z) (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#include "sc2.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = 2*re*im + imag(c);	\
                 re = re2-im2 + real(c);

#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Quadratic Map";
const char* CHILD = "quadratic2.so";
const char* COLORMAP = "default2.map";

const double XL = -5.25;
const double YT = 3.5;
const double XR = 5.25;
const double YB = -7.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
//const int N=0;
//double VALUE[]={0.0, 1.0};
//const Parameter PARAM( N, VALUE );
const Parameter PARAM;
//const char * PARAMDESC[N] = { "Re(c)", "Im(c)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> x(z.x,z.y);
  if ( norm(x) < 1 ) return 128;
  x = -x;
  x = I*(x-I)/(x+I);
  x = sc2(x);

  register double re=real(x);
  register double im=imag(x);
  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i<max)
    return i;
  else
    return -1;

  // return ((int) (norm(x)*10))*2+(imag(x)>0);
}

Point map(Point z, Point d, Parameter param)
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
				PARAM, CHILD, COLORMAP);
  return i;
}
}
