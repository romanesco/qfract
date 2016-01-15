/*
 * tricorn
 * z -> lambda conj(z + z^2) (parameter space)
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

#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Tricorn (with fix pt 0, parameter space)";
const char* CHILD = "tricorn-fixjulia.so";

const double XL = -3.5;
const double YT = 4.0;
const double XR = 4.5;
const double YB = -4.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {10, 0.0001};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "max detection period", "eps" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double eps = param.Value(1);
  const int maxper=param.Value(0);
  Complex orbit[maxper];

  Complex a(z.x,z.y);
  register double are=real(a), aim=imag(a);
  register double re=-are/2, im=aim/2, tmp;
  register double re2=re*re, im2=im*im;

  if ( norm(a) < 1.0 ) {
    return 108;
  } 
  double R=1-sqrt(6.0);

  // for circle approximating period two hyperbolic components
  int inCircle=0;
#if 0
  if ( norm(a) < R*R ) {
    inCircle=10;
  }
#endif

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    if (i >= max-maxper) {
      orbit[i-max+maxper] = Complex(re,im);
    }
    FUNCTION;
    RE2IM2;
  }

  if (i>=max) {
    Complex x(re,im);
    for (int j=0; j<maxper; j++) {
      if (norm(x-orbit[maxper-j-1]) < eps) {
	return j*20+128+inCircle;
      }
    }
    return -1+inCircle;
  } else {
    return i*2+inCircle;
  }
}
  
Point map(Point z, Point c, Parameter param)
{
  register double re=z.x, im=z.y, tmp;
  Complex a(c.x,c.y);
  register double are=real(a), aim=imag(a);
  register double re2=re*re, im2=im*im;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return Point(-0.5,0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
