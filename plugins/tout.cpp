/*
 * Tout function (asked by Kaoru Sano)
 * (az^3-z^2)/(z-a)
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*x*(a*x-1.0)/(x-a);

#define INCOND ((norm(x)<100000) &&  (norm(x)>0.000001))

const char* NAME = "Tout function (parameter space)";
const char* CHILD = "toutjulia.so";

const double XL = -1.2;
const double YT = 1.2;
const double XR = 1.2;
const double YB = -1.2;

const int MAXITER = 500;
const int MAXORBIT = 10;
double VALUE[0]={};
const Parameter PARAM( 0, VALUE );

extern "C" {

int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x, z.y), a2=a*a;
  complex<double> x=0.25*(3.0*a2 + 1.0 + sqrt(9.0*a2*a2 - 10.0*a2+1.0))/a;
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
  }
  
  if (i<max) {
    if (norm(x)>1)
      return i*10;
    else 
      return i*10+65;
  }
  return -1;
}
  
Point map(Point z, Point c, Parameter param)
{
  
  complex<double> a(c.x, c.y);
  complex<double> x(z.x, z.y);
  
  FUNCTION;
    
  return Point(real(x), imag(x));
}

Point init(Point c, Parameter param) {
  complex<double> a(c.x, c.y), a2=a*a;
  complex<double> x=0.25*(3.0*a2 + 1.0 + sqrt(9.0*a2*a2 - 10.0*a2+1.0))/a;

    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD);
  return i;
}
}
