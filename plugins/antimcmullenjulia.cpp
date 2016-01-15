/*
 * McMullen maps
 * z -> z^n + c/z^m
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

const char* NAME = "Anti-McMullen maps (phase space)";
const char* CHILD = "";

const double XL = -1.2;
const double YT = 1.2;
const double XR = 1.2;
const double YB = -1.2;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=4;
double VALUE[N] = {0.00000001, 0.0, 3, 3};
const Parameter PARAM(N,VALUE);

complex<double> f(complex<double> x, complex<double> c,
		  int n, int m)
{
  x = conj(x);
  int k=min(n,m);
  complex<double> y1=x;
  int j;
  for (j=1; j<k; j++) {
    y1 *= x;
  }
  complex<double> y2=y1;
  if (n > m) {
    for (; j<n; j++) {
      y1 *= x;
    } 
  } else {
    for (; j<m; j++) {
      y2 *= x;
    } 
  }
  return y1 + c / y2;
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> c(param.Value(0),param.Value(1)), x(z.x,z.y);
  int n=param.Value(2), m=param.Value(3);

  register int i;
  for ( i=0; (i<max) && (norm(x) < 100); i++ ) {
    x = f(x,c,n,m);
  }

  if (i<max)
    return i*2;
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> d(param.Value(0),param.Value(1)), x(z.x,z.y);
  int n=param.Value(2), m=param.Value(3);
  
  x = f(x,d,n,m);

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{
  return c;
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
