/*
 * Cubic polynomial with preperiodic critical point 
 * (z-a)^2(z+2a)+v
 * of preperiod 2, period 1
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Cubic Preper2(1) (parameter space)";
const char* CHILD= "cubicpreper21julia.so";
//

const double XL = -M_PI;
const double YT = M_PI;
const double XR = M_PI;
const double YB = -M_PI;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 0;
double VALUE[N]={};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={};

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> t = exp(a), s=1.0/t;
    complex<double> u = (t+s)/2.0;
    t = (t-s)/2.0;
    s = (1.0-u)/(t*u);
    a = (s-t)/3.0;
    complex<double> v = (2.0*t+s)/3.0;
    complex<double> x=-a;
    
    register int i;
    for ( i=0; (i<max) && (norm(x) < 100 ); i++ ) {
      x = (x-a)*(x-a)*(x+2.0*a)+v;
    }
    
    if (i>=max)
	return -1;
    else
	return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);

    complex<double> t = exp(a), s=1.0/t;
    complex<double> u = (t+s)/2.0;
    t = (t-s)/2.0;
    s = (1.0-u)/(t*u);
    a = (s-t)/3.0;
    complex<double> v = (2.0*t+s)/3.0;

    x = (x-a)*(x-a)*(x+2.0*a)+v;

    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    complex<double> a(c.x, c.y);
    complex<double> t = exp(a), s=1.0/t;
    complex<double> u = (t+s)/2.0;
    t = (t-s)/2.0;
    s = (1.0-u)/(t*u);
    a = (s-t)/3.0;
    return Point(-real(a), -imag(a));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
