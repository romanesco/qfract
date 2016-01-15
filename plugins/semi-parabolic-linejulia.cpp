/*
 * semi-parabolic implosion F(x,y)=((1+a)x-ay+x^2+e^2,x+e^2)
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
const int N = 4;
double VALUE[N] = {0.05, 0.0, 0.3, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(eps)", "Im(eps)", "Re(a)", "Im(a)" };

//const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> e(param.Value(0),param.Value(1)),
    a(param.Value(2),param.Value(3)), x(z.x,z.y), y=x, tmp;

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
  complex<double> e(param.Value(0),param.Value(1)),
    a(param.Value(2),param.Value(3)), x(z.x,z.y), y=0.0, tmp;

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
