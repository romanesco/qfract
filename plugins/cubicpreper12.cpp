/*
 * Cubic polynomial with fixed critical value 
 * (Critical value v is periodic of period 2)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Cubic Preper1(2) (parameter space)";
const char* CHILD= "cubicpreper12julia.so";
//

const double XL = -0.95;
const double YT = 1.1;
const double XR = 0.95;
const double YB = -0.8;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 0;
double VALUE[N]={};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={};

const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=-a;
    complex<double> b = a+I;
    complex<double> tmp;
    
    register int i;
    for ( i=0; (i<max) && (norm(x) < 100 ); i++ ) {
      tmp = x-a;
      tmp *= tmp;
      x = tmp*(x+2.0*a)+b;
    }
    
    if (i>=max)
	return -1;
    else
	return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> b = a+I;
    complex<double> x(z.x, z.y);
    complex<double> tmp;

    tmp = x-a;
    tmp *= tmp;
    x = tmp*(x+2.0*a)+b;
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(-c.x, -c.y);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
