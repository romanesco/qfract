/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (dynamical space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
typedef std::complex<double> dcomplex;

#define FUNCTION x = conj(x)*conj(x)+c;
/*
#define FUNCTION im = 2*re*im - aim; \
                 re = re2-im2 + are; \
		 RE2IM2 \
		 im = 2*re*im + aim; \
                 re = re2-im2 + are;
*/

#define INCOND  ( norm(x)<param.Value(5)*param.Value(5) )

const char* NAME = "Tricorn with parabolic fixed point (Julia-Lavaurs set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -104.0;
const double YT = 2.0;
const double XR = -100.0;
const double YB = -2.0;

const int MAXITER = 1000;
const int MAXORBIT = 1000;
const int N = 6;
double VALUE[N] = {0.0, 0.0, 0.005, 0, 0, 10};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "phase", "t","eps", "1=apply Lavaurs map", "rotate 1/3 × ", "R" };

const dcomplex I(0,1);

dcomplex ptoc(dcomplex p)
{
  return p-conj(p*p);
}

dcomplex parabfix(double t)
{
  return exp(2*M_PI*I*t)/2.0;
}

dcomplex fatou(dcomplex &p, dcomplex &a2, dcomplex &z)
{
  return -1.0/(a2*(z-p));
}

dcomplex fatouinv(dcomplex &p, dcomplex &a2, dcomplex &z)
{
  return -1.0/(a2*z)+p;
}


dcomplex lavaurs(dcomplex &p, dcomplex &a2, dcomplex &z, double &phase)
{

  dcomplex w=-1.0/(a2*(z-p));
  w += -((int) real(w))*2 + phase;
  return -1.0/(a2*w)+p;
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
  dcomplex x(z.x,z.y);
  double t=param.Value(1)/6;
  dcomplex p=parabfix(t);
  dcomplex c=ptoc(p);
  double phase=param.Value(0);
  double eps=param.Value(2);
  // a2=(2p)^2+\bar{2p} is the second coefficient for the Taylor expansion
  // of f^2 at p.
  dcomplex a2 = 4.0*p*p + 2.0*conj(p);

  x = fatouinv(p, a2, x);
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
    if ((param.Value(3)) && (norm(a2*(x-p)+eps)<eps*eps)) {
      x=lavaurs(p, a2, x, phase);
    }
  }

  dcomplex omega(cos(2*M_PI*param.Value(4)/3), sin(2*M_PI*param.Value(4)/3));
  
  if (i>=max)
    return -1;
  else
    if (imag(omega*x) >= 0)
    return i*8;
  else
    return i*8+21;
}

Point map(Point z, Point c0, Parameter param)
{
  dcomplex x(z.x,z.y);
  double t=param.Value(1)/6;
  dcomplex p=parabfix(t);
  dcomplex c=ptoc(p);
  double phase=param.Value(0);
  double eps=param.Value(2);
  // a2=(2p)^2+\bar{2p} is the second coefficient for the Taylor expansion
  // of f^2 at p.
  dcomplex a2 = 4.0*p*p + 2.0*conj(p);
  
  FUNCTION;
    if ((param.Value(3)) && (norm(a2*(x-p)+eps)<eps*eps)) {
    x=lavaurs(p, a2, x, phase);
  }
  
  return Point(real(x),imag(x));
}

Point init(Point c0, Parameter param) {
  double t=param.Value(1)/6;
  dcomplex p=parabfix(t);
  dcomplex c=ptoc(p);
  dcomplex x(c0.x,c0.y);
  dcomplex a2 = 4.0*p*p + 2.0*conj(p);
  x = fatouinv(p, a2, x);
  return Point(real(x),imag(x));}
  
PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
