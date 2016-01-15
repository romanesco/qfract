/*
 * semi-parabolic implosion F(x,y)=((1+a)x-ay+x^2+e^2,x+e^2)
 * e = pi/(n+alpha)
 * tangent line at the origin (parabolic direction for e=0)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION tmp = x+e*e/(1.0-a);		\
  x = (1.0+a)*x-a*y+x*x+e*e/(1.0-a);		\
  y = tmp;

#define INCOND ( norm(x)+norm(y)<r )

const char* NAME = "Semi-parabolic implosion (tangent line)";
const char* CHILD = "tanleijulia.so";
const char* COLORMAP = "default.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N = 5;
double VALUE[N] = {40, 0.0, 0.0, 0.3, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "n", "Re(alpha)", "Im(alpha)", "Re(a)", "Im(a)" };

//const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double n(param.Value(0));
  complex<double> alpha(param.Value(1),param.Value(2));
  complex<double> e=M_PI/(n+alpha),
    a(param.Value(3),param.Value(4)), x(z.x,z.y), y=x, tmp;

  const double r=10000;

  register int i,j;
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
  double n(param.Value(0));
  complex<double> alpha(param.Value(1),param.Value(2));
  complex<double> e=M_PI/(n+alpha),
    a(param.Value(3),param.Value(4)), x(z.x,z.y), y=0.0, tmp;

  FUNCTION

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
