/*
 * Cubic polynomials with fixed critical value
 * f(z) = a(z^3-z^2) + 1
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

#define FUNCTION x = a*x*x*(x-1.0)+1.0;

const char* NAME = "Cubic polyn w/ fixed cv (parameter space)";
const char* CHILD= "cubicfixcvjulia.so";
const char* COLORMAP = "default.map";

const double XL = -2.5;
const double YT = 6;
const double XR = 9.5;
const double YB = -6;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 0;
//double VALUE[N]={3,100};
const Parameter PARAM;
const char* PARAMDESC[N];

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x, z.y);
  complex<double> x = 2.0/3.0;
  double R = 100.0/sqrt(abs(a));
  
    
  register int i;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    FUNCTION;
  }
  
  if (i>=max) {
    return -1;
  }
  
  return i;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> a(c.x, c.y);
  complex<double> x(z.x,z.y);
  
  FUNCTION;
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(2.0/3.0,0);
}
  
PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
