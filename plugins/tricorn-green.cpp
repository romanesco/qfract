/*
 * tricorn (bifurcation locus)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Tricorn (with Green's function algorithm)";
const char* CHILD = "tricorn-greenjulia.so";

const double XL = -2.5;
const double YT = 2.0;
const double XR = 1.5;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={300,100};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "R", "metric minimum" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
     complex<double> a(z.x, z.y);
    complex<double> x=a;
    double R=param.Value(0);
    complex<double> tmp, dc=1, dcbar=0;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
      /*
      dc = 1.0+2.0*conj(x*dcbar);
      dcbar = 2.0*conj(x*dc);
      x = conj(x*x)+a;
      */
      tmp = x*x + conj(a);
      dc = 1.0 + 4.0*x*tmp*dc;
      dcbar = 2.0*tmp + 4.0*x*tmp*dcbar;
      x = tmp*tmp + a;
    }
    
    if (i>=max)
	return 10;

    double g, dg, metric;
    g=log(norm(x))/(2*pow(4.0,i));
    dg = sqrt(norm(dc)-norm(dcbar))/(abs(x)*pow(4.0,i));
    metric =dg/sinh(g);
    if (metric < param.Value(1))
	return 0;
    else
	return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);
    complex<double> tmp;

    register int j;
    x = conj(x*x) + a;
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    return c;
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
