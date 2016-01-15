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

const char* NAME = "Quasi-periodically forced system (1D base space)";
const char* CHILD = "qp-forced_thetajulia.so";
const char* COLORMAP = "default.map";

const double XL = 0.0;
const double YT = 0.5;
const double XR = 1.0;
const double YB = -0.5;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N=4;
double VALUE[N]={ 0, 0.815, 0.42, (sqrt(5)-1)/2 };
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "0=real, 1=cpx", "a", "eps", "omega" };

typedef complex<double> Complex;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  if (!param.Value(0))
      return -1;

  double a(param.Value(1)), eps(param.Value(2)), omega(param.Value(3));

  Complex x(0), y;
  
  y=Complex(z.x, z.y*param.Value(0));

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
  double a(param.Value(1)), eps(param.Value(2)), omega(param.Value(3));
  Complex x(z.x, z.y);

  
  FUNCTION;
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{ 
  if (param.Value(0)) {
  y=Complex(c.x, c.y*param.Value(0));
  } else {
    y=c.x;
  }
  return Point(0,0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
