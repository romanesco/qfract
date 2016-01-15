/*
 * Quadratic rational map with period 2 cycle 0 <-> \infty
 * z -> 1/4*c/(z^2-z)
 * (parameter space)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION x = (x*x+p)/(x*x-1.0);

#define INCOND  ( norm(x)<r )

const char* NAME = "Rational with degree 2, period 3 cycle (parameter space)";
const char* CHILD = "rat2per2-2julia.so";

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
  complex<double> p(z.x,z.y), x(0.0);
  double r=100;

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
  complex<double> p(c.x,c.y), x(z.x,z.y);

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
