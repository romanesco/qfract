/*
 * McMullen maps
 * z -> z^n + c/z^m
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

const char* NAME = "Anti-McMullen maps (parameter space)";
const char* CHILD = "antimcmullenjulia.so";

const double XL = -0.5;
const double YT = 0.5;
const double XR = 0.5;
const double YB = -0.5;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N] = {3, 3};
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
  complex<double> c(z.x,z.y), x;
  int n=param.Value(0), m=param.Value(1);

  x = pow( ((double) n)/m*c, 1/((double) n+m) );

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
  complex<double> x(z.x,z.y);
  complex<double> d(c.x, c.y);
  int n=param.Value(0), m=param.Value(1);
  
  x = f(x,d,n,m);

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{
  complex<double> x(c.x, c.y), y;
  int n=param.Value(0), m=param.Value(1);
  
  x = pow( ((double) n)*x/((double) m), 1/((double) n+m) );
  
  return Point(real(x),imag(x));
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
