/*
 * Cosine map c*cos(z) (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Cosine Map (parameter space)";
const char* CHILD= "cosjulia.so";
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
    complex<long double> x=1;

    register int i,j;
    //for ( i=0; (i<max) && (!isinf(real(x))) && (!isinf(imag(x))); i++ ) {
    for ( i=0; (i<max) && (imag(x)<1e+10) && (imag(x)>-1e+10); i++ ) {
      x = a * cos(x);
    }
    
    if (i>=max)
	return -1;
    if (imag(x) > 0) {
      return i;
    } else {
      return i+33;
    }
}

Point map(Point z, Point c, Parameter param)
{
    complex<long double> a(c.x, c.y);
    complex<long double> x(z.x, z.y);

    register int j;
    x = a * cos(x);
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(0,0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
