/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (dynamical space with external rays)
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

const char* NAME = "Tricorn (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 5;
double VALUE[N] = {-1.0, 0.0, 100, 0,0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)", "escape radius", "c,z-rotation", "z-rotation" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double t=-param.Value(3), t1 = -param.Value(4);
  register double s=sin(t), c=cos(t), s1 = sin(t+t1), c1=cos(t+t1);
  register double re=c1*z.x-s1*z.y;
  register double im=s1*z.x+c1*z.y;
  register double are=c*param.Value(0)-s*param.Value(1);
  register double aim=s*param.Value(0)+c*param.Value(1);
  register double re2=re*re;
  register double im2=im*im;
  double R2=param.Value(2);
  R2 *= R2;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else if (im >= 0) 
    return i*8;
  else
    return i*8+21;
}

Point map(Point z, Point d, Parameter param)
{
  register double t=-param.Value(3), t1 = -param.Value(4);
  register double s=sin(t), c=cos(t), s1 = sin(t+t1), c1=cos(t+t1);
  register double re=c1*z.x-s1*z.y;
  register double im=s1*z.x+c1*z.y;
  register double are=c*param.Value(0)-s*param.Value(1);
  register double aim=s*param.Value(0)+c*param.Value(1);
  register double re2=re*re;
  register double im2=im*im;

  FUNCTION

  are = c1*re+s1*im;
  aim = -s1*re+c1*im;
  return Point(are,aim);
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
