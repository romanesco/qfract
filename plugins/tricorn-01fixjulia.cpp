/*
 * tricorn
 * z -> a*conj(z)^2+(1-a)*conj(z) (dynamical space)
 * (fixing 0 and 1)
 */

#include "../plugin.h"
using namespace QFract;

#include <complex>
typedef std::complex<double> Complex;

#define FUNCTION x=(a*conj(x)+1.0-a)*conj(x);

#define INCOND  ( norm(x)<10000 )

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
const int N = 2;
double VALUE[N] = {-1.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  Complex a(param.Value(0), param.Value(1)), x(z.x, z.y);

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  if (i>=max)
    return -1;
  else 
    return i*2;
}

Point map(Point z, Point c, Parameter param)
{
  Complex a(param.Value(0), param.Value(1)), x(z.x, z.y);
  
  FUNCTION
    
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
