/*
 * Real quadratic rational map with x → 0 (cp) → ∞ → y (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Real quadratic rational map (phase space)";
const char* CHILD = "";
//const char* COLORMAP = "per2-2.map";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={0.0, 0.0, 0.01, 10};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "x", "y", "unused", "sample#"};

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double a = param.Value(0), b = param.Value(1);
  double r = (1-a-b)/(1-a);
  complex<double> x(z.x, z.y);

  register int i;
  for ( i=0; (i<max); i++ ) {
    x = (x-a)*(x-r)/(x*x);
  }

  double logdfn = 0;
  
  for ( i=0; (i<param.Value(3)); i++ ) {
    logdfn += log(abs( ((r+a)*x - 2*a*r)/(x*x) ));
    x = (x-a)*(x-r)/(x*x);
  }

  return (int) (logdfn*5) + 256;
}

Point map(Point z, Point c, Parameter param)
{
  double a = param.Value(0), b = param.Value(1);
  double r = (1-a-b)/(1-a);
  complex<double> x(z.x,z.y);

  x = (x-a)*(x-r)/(x*x);
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
