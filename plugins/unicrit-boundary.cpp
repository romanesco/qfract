/*
 * Unicritical map z^d+c (drawing bifurcation locus)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Unicritical Map (parameter space with Green's function algorithm)";
const char* CHILD = "unicrit-boundaryjulia.so";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 3;
double VALUE[N]={2,300,100};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "degree", "R", "metric minimum" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=a;
    double R=param.Value(1);
    complex<double> tmp, dx=1;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
	tmp=x;
	for ( j=1; j<param.Value(0)-1; j++)
	    tmp *= x;
	tmp=pow(x,param.Value(0)-1);
	dx = dx*tmp+1.0/pow(param.Value(0),i+1);
	x = x*tmp + a;
    }
    
    if (i>=max) {
	return -2;
    }

    double g, dg, metric;
    g=log(norm(x))/(2*pow(param.Value(0),i));
    dg = abs(dx)/abs(x);
    metric =dg/sinh(g);
    if (metric < param.Value(2))
	return i*2;
    else
	return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);
    complex<double> tmp;

    register int j;
    tmp=x;
    for ( j=1; j<param.Value(0); j++)
	tmp *= x;
    x = tmp + a;
    
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
