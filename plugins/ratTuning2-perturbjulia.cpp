/*
 * Finding tunable rational map (phase space)
 * 
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Finding tunable rational map (phase space)";
const char* CHILD = "";
//const char* COLORMAP = "per2-2.map";
const char* COLORMAP = "default2.map";

const double XL = 0.9;
const double YT = 0.1;
const double XR = 1.1;
const double YB = -0.1;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={0.0, 0.0, -1.5, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(C)", "Im(C)", "Re(a)", "Im(a)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> C(param.Value(0), param.Value(1));
  complex<double> a(param.Value(2), param.Value(3));
  complex<double> x(z.x,z.y);
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
  
  if (i>=max)
    return -1;
  else 
    return i;
}
  
Point map(Point z, Point c, Parameter param)
{
  complex<double> C(param.Value(0), param.Value(1));
  complex<double> a(param.Value(1), param.Value(2));
  complex<double> x(z.x,z.y);
  complex<double> tmp, tmp2, b, p, q;
  
  tmp = x * x * x;
  tmp2 = tmp * tmp;
  b = 9.*a - 15.;
  p = 1. + tmp2*(b + tmp2 * (b + tmp2) );
  q = tmp * ( 1. + tmp2 * (a + tmp2));
  
  x = C * p / q;
  
  return Point(real(x),imag(x));
}
  
Point init(Point c, Parameter param) {return c;}
  
PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
