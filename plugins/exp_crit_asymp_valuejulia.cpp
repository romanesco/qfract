/*
 * Exponential map exp(a*(z^2-p^2)+log(p)) (Julia set)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "exp(a*(z^2-p^2)+log(p)) (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default.map";

const double XL = -5.0;
const double YT = 5.0;
const double XR = 5.0;
const double YB = -5.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={0.5, 0.0, 1.0, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "Re(fp)", "Im(fp)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<long double> a(param.Value(0), param.Value(1)),
    p(param.Value(2), param.Value(3));
  complex<long double> x(z.x,z.y);

  register int i;
  //for ( i=0; (i<max) && (!isinf(real(x))) && (!isinf(imag(x))); i++ ) {
  for ( i=0; (i<max) && (real(x) < 1e+4000) && (!isinf(imag(x))); i++ ) {
    x = exp(a*(x*x-p*p)+log(p));
  }
  
  if (i>=max)
    return -1;
  return i;
}
  
Point map(Point z, Point c, Parameter param)
{
  complex<long double> a(param.Value(0), param.Value(1)),
    p(param.Value(2), param.Value(3));
  complex<long double> x(z.x,z.y);
  
  x = exp(a*(x*x-p*p)+log(p));
  
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
