/*
 * tricorn
 * z -> a*conj(z)^2+(1-a)*conj(z) (parameter space)
 * (fixing 0 and 1)
 */

#include "../plugin.h"
using namespace QFract;

#include <complex>
typedef std::complex<double> Complex;

#define FUNCTION x=(a*conj(x)+1.0-a)*conj(x);

#define INCOND  ( norm(x)<10000 )

const char* NAME = "Tricorn (parameter space)";
const char* CHILD = "tricorn-01fixjulia.so";

const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  Complex a(z.x,z.y), x;

  x = conj( (-1.0+a)/(2.0*a) );

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
  Complex a(c.x, c.y), x(z.x, z.y);

  FUNCTION

    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {
  Complex a(c.x, c.y), x;

  x = conj( (-1.0+a)/(2.0*a) );
  
  return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
