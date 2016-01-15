/*
 * tricorn
 * z -> lambda conj(z + z^2) (dynamical space)
 */

#include <complex>
typedef std::complex<double> Complex;

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION tmp =  are*re+aim*im + re2-im2;\
                 im  = -are*im+aim*re - 2*re*im;\
	         re  = tmp;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Tricorn (with fix pt 0, Julia set)";
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
const int N = 2;
double VALUE[N] = {-1.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(lambda)", "Im(lambda)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x, im=z.y, tmp;
  Complex a(param.Value(0),param.Value(1));
  register double are=real(a), aim=imag(a);
  register double re2=re*re, im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else 
    return i*2;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x, im=z.y, tmp;
  Complex a(param.Value(0),param.Value(1));
  register double are=real(a), aim=imag(a);
  register double re2=re*re, im2=im*im;

  FUNCTION

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
