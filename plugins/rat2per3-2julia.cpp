/*
 * Quadratic rational map with period 3 cycle
 * z -> 1/(c*z^2-(c+1)*z+1)
 * (0 -> 1 -> \infty -> 0, phase space)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION x = (x*x+p)/(x*x+q);

#define INCOND  ( norm(x)<r )

const char* NAME = "Rational with degree 2, period 3 cycle (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -10.0;
const double YT = 10.0;
const double XR = 10.0;
const double YB = -10.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {0.0, 1.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(param.Value(0),param.Value(1)),
    p=(a*a-1.0)*a-1.0, q=-a*a, x(z.x,z.y);
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
  complex<double> a(param.Value(0),param.Value(1)),
    p=(a*a-1.0)*a-1.0, q=-a*a, x(z.x,z.y);

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
