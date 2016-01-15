/*
 * Cubic antipodal preserving map (phase space)
 * z -> z^2(z-q)/(1+conj(q)z)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION x = x*x*(x-a)/(1.0+conj(a)*x);

#define INCOND ( ( norm(x)<1/eps ) && (norm(x)>eps) )

const char* NAME = "Cubic antipode preserving map (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "per3.map";

const double XL = -2;
const double YT = 2;
const double XR = 2;
const double YB = 2;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N] = {0.0, 1.0, 10, 0.0001};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };
const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(param.Value(0),param.Value(1)), x(z.x,z.y);
  double eps=param.Value(3);
  const int maxper=param.Value(2);
  complex<double> orbit[maxper];

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    if (i >= max-maxper) {
      orbit[max-1-i] = x;
    }
    FUNCTION
  }

  if (i<max)
    if (norm(x) < eps)
      return 3*i + 51*(imag(-x/a)>0);
    else
      return 3*i+1 + 51*(imag(a*x)>0);
  else
    // attracting cycle detection
    for (int j=0; j<maxper-1; j++) {
      if (norm(x-orbit[j]) < eps) 
	return j*9+2;
    }
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> a(param.Value(0),param.Value(1)), x(z.x,z.y);

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
