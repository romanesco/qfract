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
const char* CHILD= "ratTuning-perturbjulia.so";
//

const double XL = -0.1;
const double YT = 0.1;
const double XR = 0.1;
const double YB = -0.1;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={256, 0, 3, 8};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Re(a)", "Im(a)", "p", "q" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> C(z.x, z.y);
  complex<double> a(param.Value(0), param.Value(1));
  int p = param.Value(2), q = param.Value(3);
  /*
  complex<double> x = a*a, b = ((double) q-p)*x - ((double) p+q);
  b = b/(a*(double) p);
  if (real(a) >= 0) {
    x = (-b + sqrt(b*b - 4.0))/2.0; // x^2 + bx + 1 = 0
  } else {
    x = (-b - sqrt(b*b - 4.0))/2.0; // x^2 + bx + 1 = 0
  }
  x = pow(x,1./q);
  */
  
  complex<double> x = 0.;
  
  double R=10000; 
  complex<double> tmp, tmp2;
  
  register int i,j;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    tmp = x;
    for (j=1; j<p; j++) {
      tmp *= x;
    }
    tmp2 = x;
    for (j=1; j<q; j++) {
      tmp2 *= x;
    }
    x = tmp*(tmp2-a)/(1.0-a*tmp2)+C;
  }
  
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
  complex<double> tmp, tmp2;
  int p = param.Value(2), q = param.Value(3);
  
  register int j;
  tmp = x;
  for (j=1; j<p; j++) {
    tmp *= x;
  }
  tmp2 = x;
  for (j=1; j<q; j++) {
    tmp2 *= x;
  }
  x = tmp*(tmp2-a)/(1.0-a*tmp2)+C;
  
  return Point(real(x),imag(x));
}
  
Point init(Point c, Parameter param) 
{
  return Point(0,0);
  /*
  complex<double> C(c.x, c.y);
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
  */
}
  
PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
