/*
 * standard maps
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Standard Map (parameter space)";
const char* CHILD = "standardjulia.so";
const char* COLORMAP = "per2.map";

const double XL = 0.0;
const double YT = 1.0;
const double XR = 1.0;
const double YB = 0.0;

const int MAXITER = 100;
const int MAXORBIT = 50;
const Parameter PARAM( 0, NULL );

const complex<double> I(0,1);

const int PER=10;

extern "C" {
int iter(Point z, Parameter param, int max)
{
    register double b=z.y, nor;
    complex<double> a=exp(2*M_PI*I*z.x);
    complex<double> x=(-1.0-sqrt((complex<double>) 1.0-4*b*b))/(2*b);

    register int i, j, k=0;
    complex<double> y[PER];

    for ( i=0; (i<max) && ((nor = norm(x)) < 100000.0*100000 ) && (nor > 0.00001*0.00001) ; i++ )
    {
	x = a*x*exp(b*(x-1.0/x));
	for ( j=0; j<PER; j++)
	  //if ( ( (norm(x) > 1.1) || ((norm(x) < 0.9) && (norm(x)>0.0001)) ) && (norm(x-y[j]) < 0.00001) )
	  if ( (norm(x)>0.0001) && (norm(x-y[j]) < 0.00001) )
		return i*2+128;
	k = (k+1) % PER;
	y[k]=x;
    }
    
    if (i>=max)
	return -1;
    else 
	return i*6+1;
}

Point map(Point z, Point c, Parameter param)
{
    register double b=z.y;
    complex<double> a=exp(2*M_PI*I*z.x);
    complex<double> x(z.x, z.y);

    x = a*x*exp(b*(x-1.0/x));
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    register double b=c.y;
    complex<double> x=(-1.0-sqrt((complex<double>) 1.0-4*b*b))/(2*b);
    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP);
  return i;
}
}
