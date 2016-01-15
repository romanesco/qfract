/*
 * Quadratic rational map with period 3 cycle
 * z -> 1/(c*z^2-(c+1)*z+1)
 * (0 -> 1 -> \infty -> 0, parameter space)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION x = (x*x+p)/(x*x+q);

#define INCOND  ( norm(x)<r )

const char* NAME = "Rational with degree 2, period 3 cycle (parameter space)";
const char* CHILD = "rat2per3-2julia.so";

const double XL = -2.0;
const double YT = 2.5;
const double XR = 3.0;
const double YB = -2.5;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x,z.y), p=(a*a-1.0)*a-1.0, q=-a*a, x(0.0);
  double r=100/norm((a-1.0)/(4.0*a));

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  if (i<max)
    return i;
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> a(c.x,c.y), p=(a*a-1.0)*a-1.0, q=-a*a, x(z.x,z.y);

  FUNCTION
    
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{
  return Point(0,0);
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
