/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (dynamical space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
typedef std::complex<double> dcomplex;

#define FUNCTION x = conj(x)*conj(x)+c;
#define INCOND  ( norm(x)<10 )

const char* NAME = "Tricorn with parabolic fixed point (Julia-Lavaurs set, parameter space)";
const char* CHILD = "tricornjulialavaurs.so";
const char* COLORMAP = "default.map";

const double XL = 0;
const double YT = 0.5;
const double XR = 1.0;
const double YB = -0.5;

const int MAXITER = 10000;
const int MAXORBIT = 10000;
const int N = 1;
double VALUE[N] = {0.005};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "eps" };

const dcomplex I(0,1);

dcomplex ptoc(dcomplex p)
{
  return p-conj(p*p);
}

dcomplex parabfix(double t)
{
  return exp(2*M_PI*I*t)/2.0;
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
  double t=z.y/6;
  dcomplex p=parabfix(t);
  dcomplex c=ptoc(p);
  dcomplex x(c);
  double phase=z.x;
  double eps=param.Value(0);
  // a2=(2p)^2+\bar{2p} is the second coefficient for the Taylor expansion
  // of f^2 at p.
  dcomplex a2 = 4.0*p*p + 2.0*conj(p);

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
    if (norm(a2*(x-p)+eps)<eps*eps) {
      x=lavaurs(p, a2, x, phase);
    }
  }

  if (i>=max)
    return -1;
  else 
    return i*2;
}

Point map(Point z, Point c0, Parameter param)
{
  dcomplex x(z.x,z.y);
  double t=c0.y/6;
  dcomplex p=parabfix(t);
  dcomplex c=ptoc(p);
  double phase=c0.x;
  double eps=param.Value(0);
  // a2=(2p)^2+\bar{2p} is the second coefficient for the Taylor expansion
  // of f^2 at p.
  dcomplex a2 = 4.0*p*p + 2.0*conj(p);
  
  FUNCTION;
  if (norm(a2*(x-p)+eps)<eps*eps) {
    x=lavaurs(p, a2, x, phase);
  }
  
  return Point(real(x),imag(x));
}

Point init(Point c0, Parameter param)
{
  double t=c0.y/6;
  dcomplex p=parabfix(t);
  dcomplex c=ptoc(p);
  return Point(real(c),imag(c));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
