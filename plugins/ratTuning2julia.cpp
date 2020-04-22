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

const double XL = -15.0;
const double YT = 15.0;
const double XR = 15.0;
const double YB = -15.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={-1.5, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(a)", "Im(a)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x(z.x,z.y);
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
    
    if (i>=max)
      return -1;
    else if (norm(x) >= R) 
      return i*2;
    else if (norm(x-1.) <= 1/(R*R))
      return i*2 + 21;
    else
      return i*2 + 63;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x(z.x,z.y);
    complex<double> tmp, tmp2, b, p, q;

    tmp = x * x * x;
    tmp2 = tmp * tmp;
    b = 9.*a - 15.;
    p = 1. + tmp2*(b + tmp2 * (b + tmp2) );
    q = tmp * ( 1. + tmp2 * (a + tmp2));
    
    x = (a+2.)/(18.*a-28.) * p / q;
    
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
