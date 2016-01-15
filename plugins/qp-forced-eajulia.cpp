/*
 * quasi-periodically forced system (by Chawanya)
 */

#include "../plugin.h"
using namespace QFract;

#include<complex>
using namespace std;

#define FUNCTION x = a - x*x + eps * cos(2*M_PI*y); \
  y = y+omega;

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
double VALUE[N]={0.815, 0.0, 0.42, 0.0, 0.0, (sqrt(5)-1)/2 };
const Parameter PARAM(N, VALUE);
const char * PARAMDESC[N] = { "re(a)", "im(a)", "re(eps)", "im(eps)", 
			      "theta", "omega" };

typedef complex<double> Complex;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  Complex eps(param.Value(2),param.Value(3)), a(param.Value(0),param.Value(1));
  double omega(param.Value(5));

  Complex x(z.x, z.y);
  double y=param.Value(4);
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
  }

  if (i<max) {
    return i;
  } else
    return -1;
}
  
static double y;

Point map(Point z, Point c, Parameter param)
{
  Complex eps(param.Value(2),param.Value(3)), a(param.Value(0),param.Value(1));
  double omega(param.Value(5));
  Complex x(z.x, z.y);

  FUNCTION

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{ 
  y=param.Value(4);
  return c;
}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
