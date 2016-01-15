/*
 * Exponential map exp(a*(z^2-p^2)+log(p)) (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "exp(a*(z^2-p^2)+log(p)) (parameter space)";
const char* CHILD= "exp_crit_asymp_valuejulia.so";
//

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={1.0, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Re(fp)", "Im(fp)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  //complex<long double> p(c.x, c.y), a=1.0/(2.0*p*p);
  complex<long double> a(z.x, z.y), p(param.Value(0), param.Value(1));
  complex<long double> x(0.0);

  register int i,j;
  //for ( i=0; (i<max) && (!isinf(real(x))) && (!isinf(imag(x))); i++ ) {
  for ( i=0; (i<max) && (real(x) < 1e+4000) && (!isinf(imag(x))); i++ ) {
    x = exp(a*(x*x-p*p)+log(p));
  }
  
  if (i>=max)
    return -1;
  else
    return i;
}
  
Point map(Point z, Point c, Parameter param)
{
  //complex<long double> p(c.x, c.y), a=1.0l/(2.0l*p*p);
  complex<long double> a(c.x, c.y), p(param.Value(0), param.Value(1));
  complex<long double> x(z.x, z.y);
  
  x = exp(a*(x*x-p*p)+log(p));
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    return c;
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
