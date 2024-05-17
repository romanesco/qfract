/*
 * Cubic polynomial with fixed critical value z^3 - 3a^2z + 2a^3-2a
 * (Critical value v is equal to the cocritical point -2a)
 p */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Model4 (parameter space)";
const char* CHILD= "JordanBoundaryModel4julia.so";
//

const double XL = -3;
const double YT = 3;
const double XR = 3;
const double YB = -3;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 0;
double VALUE[N]={};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={};

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=a;
    
    register int i;
    for ( i=0; (i<max) && (norm(x) < 10000 ); i++ ) {
      x = x*x*(-a/2.0 + x/3.0);
      x = x*x;
    }
    
    if (i>=max)
	return -1;
    else 
      return i + (imag(x)>0)*32;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);

    x = x*x*(-a/2.0 + x/3.0);
    x = x*x;

    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(-c.x, -c.y);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
