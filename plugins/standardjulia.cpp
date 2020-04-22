/*
 * standard maps
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "standard map (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 50;
const int N = 2;
double VALUE[N]={0.0, 1.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "a", "b" };

const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
    double b=param.Value(1);
    complex<double> a=exp(2*M_PI*I*param.Value(0)), x(z.x,z.y);
    complex<double> tmp;
    double nor;

    register int i;
    for ( i=0; (i<max) && ( (nor=norm(x)) < 100000.0*10000 ) && (nor > 0.00001*0.00001); i++ )
    {
	x = a*x*exp(b*(x-1.0/x));
    }
    
    if (i>=max)
	return -1;
    else
	return i*3;
}

Point map(Point z, Point c, Parameter param)
{
    double b=param.Value(1);
    complex<double> a=exp(2*M_PI*I*param.Value(0)), x(z.x,z.y);

    x = a*x*exp(b*(x-1.0/x));
    
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
