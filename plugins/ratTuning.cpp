/*
 * tunable rational map (parameter space)
 * z^p(z^q-a)/(1-az^p)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Finding tunable rational map z^p(z^q-a)/(1-az^q) (parameter space)";
const char* CHILD= "ratTuningjulia.so";
//

const double XL = -25.0;
const double YT = 25.0;
const double XR = 25.0;
const double YB = -25.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={3, 10};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "p", "q" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    int p = param.Value(0), q = param.Value(1);
    complex<double> x = a*a, b = ((double) q-p)*x - ((double) p+q);
    b = b/(a*(double) p);
    if (real(a) >= 0) {
      x = (-b + sqrt(b*b - 4.0))/2.0; // x^2 + bx + 1 = 0
    } else {
      x = (-b - sqrt(b*b - 4.0))/2.0; // x^2 + bx + 1 = 0
    }
    x = pow(x,1./q);
    double R=10000; 
    complex<double> tmp, tmp2;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ) && (norm(x) > 1/R); i++ ) {
      tmp = x;
      for (j=1; j<p; j++) {
	tmp *= x;
      }
      tmp2 = x;
      for (j=1; j<q; j++) {
	tmp2 *= x;
      }
      x = tmp*(tmp2-a)/(1.0-a*tmp2);
    }
    
    if (i>=max)
	return -1;

    else if (norm(x) >= R) 
	return i*2;
    else
      return i*2+21;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);
    complex<double> tmp, tmp2;
    int p = param.Value(0), q = param.Value(1);
    
    register int j;
    tmp = x;
    for (j=1; j<p; j++) {
      tmp *= x;
    }
    tmp2 = x;
    for (j=1; j<q; j++) {
      tmp2 *= x;
    }
    x = tmp*(tmp2-a)/(1.0-a*tmp2);
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    complex<double> a(c.x, c.y);
    int p = param.Value(0), q = param.Value(1);
    complex<double> x = a*a, b = ((double) q-p)*x - ((double) p+q);
    b = b/(a*(double) p);
    if (real(a) >= 0) {
      x = (-b + sqrt(b*b - 4.0))/2.0; // x^2 + bx + 1 = 0
    } else {
      x = (-b - sqrt(b*b - 4.0))/2.0; // x^2 + bx + 1 = 0
    }
    x = pow(x,1./q);
    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
