/*
 * Multicorn conj(z)^d+c (Julia set)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME  = "Multicorn (Julia set)";
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
const int N = 4;
double VALUE[N]={0.0, 0.0, 3, 100};
const Parameter PARAM( N, VALUE );

const char* PARAMDESC[N]={"Re(c)", "Im(c)", "degree", "R" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x(z.x,z.y);
    double R=param.Value(3);
    complex<double> tmp;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
	tmp=x;
	for ( j=1; j<param.Value(2); j++)
	    tmp *= x;
	x = conj(tmp) + a;
    }
    
    if (i>=max)
	return -1;
    else
	return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x(z.x,z.y);
    complex<double> tmp;

    register int j;
    tmp=x;
    for ( j=1; j<param.Value(2); j++)
	tmp *= x;
    x = conj(tmp) + a;
    
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
