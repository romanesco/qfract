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

const char* NAME = "Rational with degree 2, period 2 cycle (Julia set)";
const char* CHILD = "";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {1.0, 0.0};
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "Re(A)", "Im(A)" };
const char* COLORMAP = "default2.map";

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> A(param.Value(0), param.Value(1)), x(z.x, z.y), y, w;
  //double epsilon=0.1;
  double epsilon = 0.1*norm(A);
  epsilon = (epsilon > 0.1) ? 0.1 : epsilon;

  register int i, j;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  if (i<max) {
    return i*2;
  } else {
    return -1;
  }
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> A(param.Value(0), param.Value(1)), x(z.x, z.y), y, w;
  double epsilon=0.5;

  FUNCTION;

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{
  return c;
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
