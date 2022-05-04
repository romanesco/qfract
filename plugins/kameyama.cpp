/*
 * Kameyama's example a(z^2-1)+1/(4a(z^2-1)) (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Kameyama's example (parameter space)";
const char* CHILD= "kameyamajulia.so";
//

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 1;
double VALUE[N]={100};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "R" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=0.0;
    double R=param.Value(0);

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
	x=x*x-1.0;
	x = a*x+1.0/(4.0*a*x);
    }
    
    if (i>=max)
	return -1;
    else
	return i*5;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);

    register int j;
    x=x*x-1.0;
    x = a*x+1.0/(4.0*a*x);
    
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
