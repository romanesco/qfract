/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (dynamical space with external rays)
 */

#include "../plugin.h"
using namespace QFract;

#include <complex>
typedef std::complex<double> dcomplex;

#define FUNCTION x = conj(x)*conj(x)+c;
#define INCOND  ( norm(x)<R2 )

const char* NAME = "Tricorn (close to Lavaurs map, Julia set)";
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
const int N = 4;
double VALUE[N] = {-1.0, 0.0, 100, 0.75};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)",
			     "escape radius", "phase offset" };
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
  double t=param.Value(1)/240;
  dcomplex p=parabfix(t);
  dcomplex c0=ptoc(p);
  double phase=param.Value(0)-param.Value(3)*param.Value(1)*param.Value(1);
  dcomplex c=c0 + 1/phase;
  dcomplex x(z.x,z.y);
  
  double R2 = param.Value(2);
  R2 *= R2;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
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
  double t=param.Value(1)/240;
  dcomplex p=parabfix(t);
  dcomplex c0=ptoc(p);
  dcomplex x(z.x,z.y);
  double phase=param.Value(0)-param.Value(3)*param.Value(1)*param.Value(1);
  dcomplex c=c0 + 1/phase;

  FUNCTION;

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
