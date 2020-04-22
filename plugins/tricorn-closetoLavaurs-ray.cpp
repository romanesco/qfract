/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (parameter space with external rays)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
typedef std::complex<double> dcomplex;

#define FUNCTION x = conj(x)*conj(x)+c;
#define INCOND  ( norm(x)<R2 )

const char* NAME = "Tricorn (close to Lavaurs map, parameter space)";
const char* CHILD = "tricorn-closetoLavaurs-rayjulia.so";

const double XL = 1000;
const double YT = 20;
const double XR = 1040;
const double YB = -20;

const int MAXITER = 1000;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {100, 0.75};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "escape radius", "phase offset" };

const dcomplex I(0,1);

dcomplex ptoc(dcomplex p)
{
  return p-conj(p*p);
}

dcomplex parabfix(double t)
{
  return exp(2*M_PI*I*t)/2.0;
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double t=z.y/240;
  dcomplex p=parabfix(t);
  dcomplex c0=ptoc(p);
  double phase=z.x-param.Value(1)*z.y*z.y;
  dcomplex c=c0 + 1/phase;
  dcomplex x(c);
  
  double R2 = param.Value(0);
  R2 *= R2;
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
  }
  
  if (i>=max)
    return -1;
  else if (imag(x) >= 0) 
    return i*8;
  else
    return i*8+21;
}

Point map(Point z, Point cc, Parameter param)
{
  double t=cc.y/6;
  dcomplex p=parabfix(t);
  dcomplex c0=ptoc(p);
  dcomplex x(z.x,z.y);
  double phase=cc.x-param.Value(1)*cc.y*cc.y;
  dcomplex c=c0 + 1/phase;

  FUNCTION;

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return Point(0,0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
