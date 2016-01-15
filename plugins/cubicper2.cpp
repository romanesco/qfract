/*
 * Cubic polynomials Per_2(lambda)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

#define INIT   a = t + 2.0 + l/t;		\
  b = -( 2.0*t + 3.0 + l/t);			\
  R=100/sqrt(norm(a));
#define INIT2  p = -b/(3.0*a);			\
  q = sqrt(-3.0*a*t + b*b)/(3.0*a);

#define FUNCTION x = ((a*x - b)*x + t)*x + 1.0;

const char* NAME = "Cubic polyn Per_2(lambda) (parameter space)";
const char* CHILD= "cubicper2-2.so";
const char* COLORMAP = "dim2-2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 0;
//double VALUE[N]={3,100};
const Parameter PARAM;
const char* PARAMDESC[N];

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> l(z.x, z.y);
    complex<double> x, a, b, t(0.5);
    double R;

    INIT;
    
    complex<double> p, q;
    INIT2;
    
    // first critical point
    x = p+q;
    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
      FUNCTION;
    }

    // second critical point
    x = p-q;
    for ( j=0; (j<max) && (norm(x) < R ); j++ ) {
      FUNCTION;
    }
    
    if ( (i>=max) && (j >= max) ) {
      return -1;
    } else if (i >= max) {
      return (j%15)+1;
    } else if (j >= max) {
      return ((i%15)+1)*16;
    } else {
      return ((i%15)+1)*16+(j%15)+1;
    }
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> l(c.x, c.y);
    complex<double> x(z.x,z.y), a, b, t(0.5);
    double R;
    INIT;
    
    FUNCTION;
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    complex<double> l(c.x, c.y);
    complex<double> x, a, b, t(1.0);
    double R;
    INIT;
    
    complex<double> p, q;
    INIT2;
    
    // first critical point
    x = p+q;

    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
