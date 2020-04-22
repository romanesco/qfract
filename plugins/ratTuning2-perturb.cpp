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
const char* CHILD= "ratTuning2-perturbjulia.so";
//

const double XL = -0.01;
const double YT = 0.001;
const double XR = -0.008;
const double YB = -0.001;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={-1.5, 0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Re(a)", "Im(a)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(param.Value(0), param.Value(1));  
  complex<double> C(z.x, z.y);

  /*
    complex<double> v = 3.*a-8.;
    complex<double> w = (v+sqrt(v*v-4.))/2.;
    complex<double> x = pow(w,1./6.);
  */
  complex<double> x = 1.;
  
  double R=10000; 
  complex<double> tmp, tmp2, p, q, b;
  
  register int i,j;
  for ( i=0;
	(i<max)
	  && (norm(x) < R );
	i++ ) {
    tmp = x * x * x;
    tmp2 = tmp * tmp;
    b = 9.*a - 15.;
    p = 1. + tmp2*(b + tmp2 * (b + tmp2) );
    q = tmp * ( 1. + tmp2 * (a + tmp2));
    
    x = C * p / q;
  }
  
  /*
  complex<double> x1=x; // keep the final point
  
  w = (v-sqrt(v*v-4.))/2.;
  x = pow(w,1./6.);
  for ( j=0;
	(j<max)
	  && (norm(x) < R );
	j++ ) {
    tmp = x * x * x;
    tmp2 = tmp * tmp;
    b = 9.*a - 15.;
    p = 1. + tmp2*(b + tmp2 * (b + tmp2) );
    q = tmp * ( 1. + tmp2 * (a + tmp2));
    
    x = C * p / q;
  }
  
  x = (i>j) ? x1 : x;
  i = (i>j) ? i : j;
  */
  
  if (i>=max)
    return -1;
  else 
    return i;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> C(c.x, c.y);
  complex<double> a(param.Value(0), param.Value(1));
  complex<double> x(z.x, z.y);
  complex<double> tmp, tmp2, b, p, q;
  
  tmp = x * x * x;
  tmp2 = tmp * tmp;
  b = 9.*a - 15.;
  p = 1. + tmp2*(b + tmp2 * (b + tmp2) );
  q = tmp * ( 1. + tmp2 * (a + tmp2));
  
  x = C * p / q;
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  complex<double> a(param.Value(0), param.Value(1));
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
