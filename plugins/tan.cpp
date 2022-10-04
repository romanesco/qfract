/*
 * Tangent map c*tan(z) (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Tangent Map (parameter space)";
const char* CHILD= "tanjulia.so";
//

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 0;
double VALUE[N]={};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<long double> a(z.x, z.y);
    complex<long double> x(-z.y, z.x);

    register int i,j;
    //for ( i=0; (i<max) && (!isinf(real(x))) && (!isinf(imag(x))); i++ ) {
    for ( i=0; (i<max) && (norm(x)<100); i++ ) {
      x = a * tan(x);
    }
    
    if (i>=max)
	return -1;
    return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<long double> a(c.x, c.y);
    complex<long double> x(z.x, z.y);

    register int j;
    x = a * tan(x);
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(-c.y, c.x);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
