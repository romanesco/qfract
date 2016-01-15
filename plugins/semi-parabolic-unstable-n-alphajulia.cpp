/*
 * semi-parabolic implosion F(x,y)=((1+a)x-ay+x^2+e^2,x+e^2)
 * e = pi/(n+alpha)
 * unstable slice for period 2 saddle
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

const double XL = -0.0001;
const double YT = 0.0001;
const double XR = 0.0001;
const double YB = -0.0001;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N = 5;
double VALUE[N] = {40, 0.0, 0.0, 0.3, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "n", "Re(alpha)", "Im(alpha)", "Re(a)", "Im(a)" };

//const complex<double> I(0,1);
static complex<double> qx, qy, vx, vy;

extern "C" {
void paraminit(Parameter param)
{
  double n(param.Value(0));
  complex<double> alpha(param.Value(1),param.Value(2));
  complex<double> e=M_PI/(n+alpha),
    a(param.Value(3),param.Value(4)), f=e*e/(1.0-a);

  // (qx,qy): period 2 saddle.
  complex<double> sq = sqrt(-(a+1.0)*(a+1.0)+f*(a-1.0)),
    sq2 = sqrt( (5.0*(a+1.0)*(a+1.0)+4.0*f*(1.0-a)) *
		(a*(5.0*a+6.0)+5.0+4.0*f*(1.0-a)) );
  qx = -a - 1.0 + sq;
  qy = -2.0*a - qx + f - 2.0;
  
  vx = -1.0 - a + 2.0*sq;
  vy = 4.0*a + 2.5 + 2.5*a*a+ 2.0*f*(1.0-a) + 0.5*sq2
    - 5.0 - (9.0 + 5.0*a)*a + 4.0*f*(a-1.0);

}

int iter(Point z, Parameter param, int max)
{
  double n(param.Value(0));
  complex<double> alpha(param.Value(1),param.Value(2));
  complex<double> e=M_PI/(n+alpha),
    a(param.Value(3),param.Value(4)), x(z.x,z.y), y, tmp;
  
  y = qy + x*vy;
  x = qx + x*vx;
  
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
