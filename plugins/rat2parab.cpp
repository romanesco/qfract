/*
 * Quadratic rational map with parabolic fixed point
 * z -> [ (c-i)(z^2-1) - 4iz ]/[ (c+i)(z^2-1) - 4iz ]
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;

#include <Complex>
using namespace std;
typedef complex<double> Complex;
const Complex I(0,1);

#define FUNCTION w = x*x-1.0; \
                 y = 4.0*I*x; \
		 x = ( (A-I)*w - y )/( (A+I)*w - y );

#define INCOND  ( norm( (x-1.0)*I*A-epsilon ) > epsilon*epsilon )

const char* NAME = "Rational with degree 2, period 2 cycle (parameter space)";
const char* CHILD = "rat2parabjulia.so";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 10000;
const int MAXORBIT = 10;
const Parameter PARAM(0);
const char* COLORMAP = "per2.map";

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> A(z.x, z.y), x, y, w;
  //double epsilon=0.1;
  double epsilon = 0.1*norm(A);
  epsilon = (epsilon > 0.1) ? 0.1 : epsilon;

  x=I;
  register int i, j;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  x=-I;
  for ( j=0; (j<max) && INCOND; j++ ) {
    FUNCTION
  }

  if ( (i<max) && (j<max) ) {
    return (i<j) ? i*4 : j*4+1;
  } else {
    return -1;
  }
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> A(c.x, c.y), x(z.x, z.y), y, w;
  double epsilon=0.01;

  FUNCTION;

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{
  return Point(0,1);
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
