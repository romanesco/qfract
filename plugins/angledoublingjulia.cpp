/*
 * Unicritical map z^d+c (Julia set with external rays)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Angle doubling map (Julia set with rays)";
const char* CHILD = "";
//const char* COLORMAP = "per2-2.map";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 3;
double VALUE[N]={0.0, 0.0, 1};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(c)", "Im(c)", "λ" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x(z.x,z.y);
    double t(param.Value(2));
    double R = (norm(a) > 10) ? 2*norm(a) : 20;
 
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
      return i + (imag(x)>0)*21;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x(z.x,z.y);
    double t(param.Value(2));

    double n=norm(x);
    if (n > 0) {
      x = (1+t*(n-1))/n * x*x + a;
    } else {
      x = a+1.0-t;
    }
    
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
