/*
 * Koebe map c(z+1/z) (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Koebe Map (parameter space)";
const char* CHILD= "koebejulia.so";
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
//const char* PARAMDESC[N]={ "degree", "R" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=1;
    double R=10000;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
      x = a*(x+1.0/x);
    }
    
    if (i>=max)
	return -1;
    else
	return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);

    register int j;
    x = a*(x+1.0/x);
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(1,0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD);
  return i;
}
}
