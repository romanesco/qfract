/*
 * Finding tunable rational map (parameter space)
 * 
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Finding tunable rational map (parameter space)";
const char* CHILD= "ratTuning2julia.so";
//

const double XL = -5.0;
const double YT = 5.0;
const double XR = 5.0;
const double YB = -5.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N = 0;
double VALUE[N]={};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> v = 3.*a-8.;
    complex<double> w = (v+sqrt(v*v-4.))/2.;
    complex<double> x = pow(w,1./6.);
    
    double R=10000; 
    complex<double> tmp, tmp2, p, q, b;

    register int i,j;
    for ( i=0;
	  (i<max)
	    && (norm(x) < R )
	    && (norm(x-1.) > 1/(R*R))
	    && (norm(x+1.) > 1/(R*R));
	  i++ ) {
      tmp = x * x * x;
      tmp2 = tmp * tmp;
      b = 9.*a - 15.;
      p = 1. + tmp2*(b + tmp2 * (b + tmp2) );
      q = tmp * ( 1. + tmp2 * (a + tmp2));

      x = (a+2.)/(18.*a-28.) * p / q;
    }
    
    complex<double> x1=x; // keep the final point

    w = (v-sqrt(v*v-4.))/2.;
    x = pow(w,1./6.);
    for ( j=0;
	  (j<max)
	    && (norm(x) < R )
	    && (norm(x-1.) > 1/(R*R))
	    && (norm(x+1.) > 1/(R*R));
	  j++ ) {
      tmp = x * x * x;
      tmp2 = tmp * tmp;
      b = 9.*a - 15.;
      p = 1. + tmp2*(b + tmp2 * (b + tmp2) );
      q = tmp * ( 1. + tmp2 * (a + tmp2));

      x = (a+2.)/(18.*a-28.) * p / q;
    }

    x = (i>j) ? x1 : x;
    i = (i>j) ? i : j;

    if (i>=max)
	return -1;
    else if (norm(x) >= R) 
      return i*2;
    else if (norm(x-1.) <= 1/(R*R))
      return i*2+21;
    else
      return i*2+63;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);
    complex<double> tmp, tmp2, b, p, q;
    
    tmp = x * x * x;
    tmp2 = tmp * tmp;
    b = 9.*a - 15.;
    p = 1. + tmp2*(b + tmp2 * (b + tmp2) );
    q = tmp * ( 1. + tmp2 * (a + tmp2));
    
    x = (a+2.)/(18.*a-28.) * p / q;
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    complex<double> a(c.x, c.y);
    complex<double> v = 3.*a-8.;
    complex<double> w = (v+sqrt(v*v-4.))/2.;
    complex<double> x = pow(w,1./6.);

    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
