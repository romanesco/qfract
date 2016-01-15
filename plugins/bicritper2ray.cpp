/*
 * Unicritical map z^d+c (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Bicritical rational map with period 2 cycle (parameter space)";
const char* CHILD= "bicritper2rayjulia.so";
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
const char* PARAMDESC[N]={ "degree", "R" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=a;
    double R=param.Value(1);
    complex<double> tmp;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ) ; i++ ) {
	tmp=x;
	for ( j=1; j<param.Value(0); j++)
	    tmp *= x;
	x = (tmp - a)/(tmp - 1.0);
    }
    
    if (i>=max)
	return -1;
    else if (imag(x)>0)
      return i*2;
    else
      return i*2+21;
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
    x = (tmp - a)/(tmp - 1.0);
    
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
