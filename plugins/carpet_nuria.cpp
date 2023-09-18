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
const char* CHILD= "carpet_nuriajulia.so";
//

const double XL = -1.2;
const double YT = 1.2;
const double XR = 1.2;
const double YB = -1.2;

const int MAXITER = 100;
const int MAXORBIT = 100;
const int N = 2;
double VALUE[N]={10, 0.0001};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "max detection period", "eps" };

complex<double> f(complex<double> z, complex<double> a = 1.0)
{
  complex<double> t = z*z*(z-1.0);
  return a * (t + a*a*(1.0-a)) / (t + 4.0/27.0);
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=a;
    double eps = param.Value(1);
    const int maxper = param.Value(0);
    complex<double> * orbit = new complex<double>[maxper];

    register int i;
    for ( i=0; (i<max); i++ ) {
      if (i >= max - maxper) {
	orbit[i-max+maxper] = x;
      }
      x = f(x,a);
    }
    
    for (int j=0; j<maxper; j++) {
      if (norm(x-orbit[maxper-j-1]) < eps) {
	delete[] orbit;
	return j*20+128;
      }
    }
    delete[] orbit;
    return -1;
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
