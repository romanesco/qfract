/*
 * Unicritical map z^d+c (parameter space with external rays)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Unicritical Map (parameter space with rays)";
const char* CHILD = "unicrit-rayjulia.so";
const char* COLORMAP = "per2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={3,100};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "degree", "R" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=a;
    double R=param.Value(1);
    complex<double> tmp;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
	tmp=x;
	for ( j=1; j<param.Value(0); j++)
	    tmp *= x;
	x = tmp + a;
    }
    
    if (i>=max)
	return -1;
    double t=arg(-x)/2/M_PI+0.5; // t = arg(x)/(2*pi)
    return ((i+t)*param.Value(0));
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
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
