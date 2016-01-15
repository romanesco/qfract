/*
 * Quadratic rational map with period 4 cycle
 * (parameter space)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION x = n/((x-a)*(p*x+q));

#define INCOND  ( norm(x)<r )

const char* NAME = "Rational with degree 2, period 4 cycle (parameter space)";
const char* CHILD = "rat2per4julia.so";

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
  complex<double> a(z.x,z.y), n=a*(a-1.0), p=a-2.0, q=1.0-a, 
    x= (a*(a-1.0)-1.0)/(a-2.0)/2.0;
  double r=10000;

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
  complex<double> a(c.x,c.y), n=a*(a-1.0), p=a-2.0, q=1.0-a, x(z.x,z.y);
  FUNCTION
    
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{
  complex<double> a(c.x, c.y), x= (a*(a-1.0)-1.0)/(a-2.0)/2.0;
  return Point(real(x),imag(x));
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
