/*
 * Sierpinski carpet Julia set (phase space)
 * (az^3 - az^2 + a^3(1-a)) / (z^3 - z^2 + 4/27)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Sierpinski carpet Julia set by Nuria Fagella (phase space)";
const char* CHILD= "";
//

const double XL = -1.0;
const double YT = 1.0;
const double XR = 1.0;
const double YB = -1.0;

const int MAXITER = 100;
const int MAXORBIT = 100;
const int N = 4;
double VALUE[N]={1, 0, 10, 0.0001};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "re(a)", "im(a)", "max detection period", "eps" };

complex<double> f(complex<double> z, complex<double> a = 1.0)
{
  complex<double> t = z*z*(z-1.0);
  return a * (t + a*a*(1.0-a)) / (t + 4.0/27.0);
}

complex<double> co;



extern "C" {
void paraminit(Parameter param)
{
  complex<double> a(param.Value(0), param.Value(1)), x=a;
    for (int i=0; i<1000; i++) {
      x = f(x,a);
    }
    co=x;
}

int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x, z.y);
    complex<double> a(param.Value(0), param.Value(1));
    double eps = param.Value(3);
    const int maxper = param.Value(2);
    
    register int i;
    for ( i=0; (i<max) && (norm(x-co) >= eps); i++ ) {
      x = f(x,a);
    }
    
    if (i >= max) {
      return -1;
    } else {
      return i + 20 * (i % maxper);
    }
      
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(param.Value(0), param.Value(1));
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
