/*
 * Unicritical map z^d+c (drawing Julia sets)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Unicritical map (Julia set with Green's function algorithm)";
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
const int N=5;
double VALUE[N]={0.0, 0.0, 2, 100, 100};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Re(c)", "Im(c)", "degree", "R", "metric" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    register complex<double> a(param.Value(0), param.Value(1));
    register complex<double> x(z.x,z.y);
    double R=param.Value(3);
    complex<double> tmp, dx=1;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
	tmp=x;
	for ( j=1; j<param.Value(2)-1; j++)
	    tmp *= x;
	dx *= tmp;
	x = x*tmp + a;
    }
    
    if (i>=max) {
	return -2;
    }

    double g, dg, metric;
    g=log(norm(x))/(2*pow(param.Value(2),i));
    dg = abs(dx)/abs(x);
    metric =dg/sinh(g);
    if (metric < param.Value(4))
	return i*2;
    else
	return -1;
}

Point map(Point z, Point c, Parameter param)
{
    register complex<double> a(param.Value(0), param.Value(1));
    register complex<double> x(z.x,z.y);
    complex<double> tmp;

    register int j;
    tmp=x;
    for ( j=1; j<param.Value(2); j++)
	tmp *= x;
    x = tmp + a;
    
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
