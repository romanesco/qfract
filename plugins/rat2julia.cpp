/*
 * Quadratic rational map (z^2+a)/(z^2+b) (phase space)
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

const int MAXITER = 25;
const int MAXORBIT = 10;
const int N = 5;
double VALUE[N]={1, 0, -1, 0, 10};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "Re(b)", "Im(b)", "sample#"};

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(param.Value(0),param.Value(1)),
    b(param.Value(2), param.Value(3));
  complex<double> x(z.x, z.y);

  register int i;
  for ( i=0; (i<max); i++ ) {
    x *=x;
    x = (x+a)/(x+b);
  }

  double logdfn = 0;
  
  for ( i=0; (i<param.Value(4)); i++ ) {
    complex<double> x2 = x*x, den = x2+b;
    logdfn += log(abs(2.0*x*(a-b)/(den*den)));
    x = (x2+a)/(x2+b);
  }

  return (int) (logdfn*5) + 256;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> a(param.Value(0),param.Value(1)),
    b(param.Value(2), param.Value(3));
  complex<double> x(z.x,z.y);
  x *= x;
  x = (x+a)/(x+b);
  
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
