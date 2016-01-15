/*
 * tricorn (Julia set)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Tricorn (Julia set with Green's function algorithm)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N=4;
double VALUE[N]={0.0, 0.0, 100, 100};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Re(c)", "Im(c)", "R", "metric" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    register complex<double> a(param.Value(0), param.Value(1));
    register complex<double> x(z.x,z.y);
    double R=param.Value(2);
    complex<double> tmp, dx=1;

    register int i;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
      tmp = x*x + conj(a);
      // dx = f^n(x)/4^n
      dx *= x*tmp;
      x = tmp*tmp + a;
    }
    
    if (i>=max)
	return 10;

    double g, dg, metric;
    g=log(norm(x))/(2*pow(4.0,i));
    dg = abs(dx)/abs(x);
    metric =dg/sinh(g);
    if (metric < param.Value(3))
	return 0;
    else
	return -1;
}

Point map(Point z, Point c, Parameter param)
{
    register complex<double> a(param.Value(0), param.Value(1));
    register complex<double> x(z.x,z.y);
    complex<double> tmp;

    x = conj(x*x) + a;
    
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
