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

const char* NAME = "Quasi-periodically forced system (parameter space)";
const char* CHILD = "qp-forcedjulia.so";
const char* COLORMAP = "default.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N]={ 0, (sqrt(5)-1)/2 };
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "theta", "omega" };

typedef complex<double> Complex;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double a(z.x), eps(z.y), omega(param.Value(1));

  Complex x(0);
  double y=param.Value(0);
  
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
  double a(c.x), eps(c.y), omega(param.Value(1));
  Complex x(z.x, z.y);

  
  FUNCTION;
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{ 
  y=param.Value(0);
  return Point(0,0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
