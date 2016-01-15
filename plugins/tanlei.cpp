/*
 * Tan Lei et al.'s map (parameter space)
 * z -> az^2(z-4)/(1-4z), a = exp(ic)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION x = a*x*x*(x-4.0)/(1.0-4.0*x);

#define INCOND ( ( norm(x)<r ) && (norm(x)>eps) )

const char* NAME = "Tan Lei's map (parameter space)";
const char* CHILD = "tanleijulia.so";
const char* COLORMAP = "per2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);
const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x,z.y), x=(19+sqrt(105))/16;
  a=exp(I*a);
  double r=10000, eps=0.0001;

  register int i,j;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  complex<double> y=x;
  x=(19-sqrt(105))/16;
  for ( j=0; (j<max) && INCOND; j++ ) {
    FUNCTION
  }

  if ( (i<max) || (j < max) )
    if ( i < j )
    //if (norm(x) < eps)
      return 2*i;
    else
      return 2*j+1;
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> a(c.x,c.y), x(z.x,z.y);
  a=exp(I*a);

  FUNCTION
    
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{
  return Point( (19+sqrt(105))/16, 0);
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
