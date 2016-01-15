/*
 * quasi-periodically forced system (by Chawanya)
 */

#include "../plugin.h"
using namespace QFract;

#include<complex>
using namespace std;

#define FUNCTION x = a - x*x + eps * cos(2*M_PI*y); \
  y = y+omega; y -= (int) real(y);

#define INCOND  ( norm(x)<10000 )

const char* NAME = "Quasi-periodically forced system (fiber space)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 1.0;
const double CIM = 0.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N=6;
double VALUE[N]={0.0, 0.0, 0.0, 0.815, 0.42, (sqrt(5)-1)/2 };
const Parameter PARAM(N, VALUE);
const char * PARAMDESC[N] = { "theta", "Im(theta)", "0=real, 1=cpx", "a", "eps", "omega" };

typedef complex<double> Complex;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double a(param.Value(3)), eps(param.Value(4)), omega(param.Value(5));

  Complex x(z.x, z.y), y;

  y=Complex(param.Value(0), param.Value(1)*param.Value(2));
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
  }

  if (i<max) {
    return i;
  } else
    return -1;
}
  
static Complex y;

Point map(Point z, Point c, Parameter param)
{
  double a(param.Value(3)), eps(param.Value(4)), omega(param.Value(5));
  Complex x(z.x, z.y);

  FUNCTION

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{ 
  y=Complex(param.Value(0), param.Value(1)*param.Value(2));

  return c;
}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
