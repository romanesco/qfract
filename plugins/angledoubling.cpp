/*
 * Angle doubling map (1-t+t|z|^2)(z/|z|)^2+c (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Angle doubling Map (parameter space)";
const char* CHILD= "angledoublingjulia.so";
//

const double XL = -2.5;
const double YT = 2.0;
const double XR = 1.5;
const double YB = -2.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N = 1;
double VALUE[N]={1};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "λ" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=a;
    double R = (norm(a) > 10) ? 2*norm(a) : 20;
    double t=param.Value(0);
    complex<double> tmp;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
      double n=norm(x);
      if (n > 0) {
	x = (1+t*(n-1))/n * x*x + a;
      } else {
	x = a+1.0-t;
      }
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
    double t = param.Value(0);
    complex<double> tmp;

    double n=norm(x);
    if (n > 0) {
      x = (1+t*(n-1))/n * x*x + a;
    } else {
      x = a+1.0-t;
    }
    
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
