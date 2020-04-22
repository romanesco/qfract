/*
 * Sierpinski carpet Julia set (phase space)
 * (az^3 - az^2 + a^3(1-a)) / (z^3 - z^2 + 4/27)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Sierpinski carpet Julia set by Morosawa (tiling in phase space)";
const char* CHILD= "";
//

const double XL = -1.5;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.5;

const int MAXITER = 4;
const int MAXORBIT = 100;
const int N = 0;
double VALUE[N]={};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={  };

//const complex<double> a(1,0);

complex<double> f(complex<double> z)
{
  complex<double> t = z*z*(z-1.0);
  return t / (t + 4.0/27.0);
}

complex<double> df(complex<double> z)
{
  complex<double> t = 3.0*z+1.0, s = 3.0*z-2.0;
  return 108.*z/(t*t*s*s*s);
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x, z.y);
    
    register int i;
    double logdf = 0;
    for ( i=0; i<max; i++ ) {
      logdf += log(norm(df(x)));
      x = f(x);
    }
    logdf /= max;
    
    return logdf;
    /*
    if (imag(x) >0) {
      return ((int) logdx)*2;
    } else {
      return ((int) logdx)*2+1;
    }
    */
      
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x, z.y);

    x = f(x);
    
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
