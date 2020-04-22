/*
 * tunable rational map (phase space)
 * z^p(z^q-a)/(1-az^q)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Finding tunable rational map z^p(z^q-a)/(1-az^q) (phase space)";
const char* CHILD = "";
//const char* COLORMAP = "per2-2.map";
const char* COLORMAP = "default2.map";

const double XL = -0.1;
const double YT = 0.1;
const double XR = 0.1;
const double YB = -0.1;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 6;
double VALUE[N]={0, 0, 256.0, 0.0, 3, 8};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(C)", "Im(C)", "Re(a)", "Im(a)", "p", "q" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> C(param.Value(0), param.Value(1));
  complex<double> a(param.Value(2), param.Value(3));
  complex<double> x(z.x,z.y);
  double R=10000;
  complex<double> tmp, tmp2;
  
  register int i,j;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    tmp = x;
    for (j=1; j<param.Value(4); j++) {
      tmp *= x;
    }
      tmp2 = x;
      for (j=1; j<param.Value(5); j++) {
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
  complex<double> C(param.Value(0), param.Value(1));
  complex<double> a(param.Value(2), param.Value(3));
  complex<double> x(z.x,z.y);
  complex<double> tmp, tmp2;
  
  register int j;
  tmp = x;
  for (j=1; j<param.Value(4); j++) {
    tmp *= x;
  }
  tmp2 = x;
  for (j=1; j<param.Value(5); j++) {
    tmp2 *= x;
  }
  x = tmp*(tmp2-a)/(1.0-a*tmp2)+C;
  
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
