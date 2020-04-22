/*
 * Sierpinski carpet Julia set (parameter space)
 * (az^3 - az^2 + a^3(1-a)) / (z^3 - z^2 + 4/27)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Sierpinski carpet Julia set by Nuria Fagella (parameter space)";
const char* CHILD= "carpet_nuria2julia.so";
//

const double XL = -0.7;
const double YT = 1.0;
const double XR = 1.3;
const double YB = -1.0;

const int MAXITER = 100;
const int MAXORBIT = 100;
const int N = 1;
double VALUE[N]={ 1 };
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "eps" };

complex<double> f(complex<double> z, complex<double> a = 1.0)
{
  complex<double> t = z*z*(z-1.0);
  return a * (t + a*a*(1.0-a)) / (t + 4.0/27.0);
}

complex<double> df(complex<double> z, complex<double> a = 1.0)
{
  complex<double> b = 3.*a+1., c = 3.*a-2.,
    x = 3.*z+1., y = 3.*z-2.;
  return 27.0*a*z*b*c*c/(x*x*y*y*y);
}


extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=0.0;

    x = f(x,a);
    double logdf = 0;
    register int i;
    for ( i=0; i<max; i++ ) {
      logdf += log(norm(df(x,a)));
      x = f(x,a);
    }
    logdf /= max;
    
    double eps = param.Value(0);
    if (logdf < -eps)
      return -1;
    if (logdf > eps)
      return 127;
    return 1;
    // return (logdf > 0) ? 1 : -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);

    x = f(x,a);
    
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
