/*
 * Tan Lei et al.'s map (phase space)
 * z -> az^2(z-4)/(1-4z), a = exp(ic)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION x = a*x*x*(x-4.0)/(1.0-4.0*x);

#define INCOND ( ( norm(x)<r ) && (norm(x)>eps) )

const char* NAME = "Tan Lei's map (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "per2-2.map";

const double XL = -3;
const double YT = 5;
const double XR = 7;
const double YB = 5;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {0.0, 1.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };
const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(param.Value(0),param.Value(1)), x(z.x,z.y);
  a=exp(I*a);
  double r=10000, eps=0.0001;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  if (i<max)
    if (norm(x) < eps)
      return 2*i;
    else
      return 2*i+1;
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> a(param.Value(0),param.Value(1)), x(z.x,z.y);
  a=exp(I*a);

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
