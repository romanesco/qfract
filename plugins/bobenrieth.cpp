/*
 * Bobenrieth family 1+c/z^2
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Bobenrieth Map (parameter space)";
const char* CHILD= "bobenriethjulia.so";
//

const double XL = -4.5;
const double YT = 4.5;
const double XR = 4.5;
const double YB = -4.5;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={500, -10, 500, -10};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "scale", "min", "scale for J", "min for J" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> x=1;

    int i;
    double logdf2 = 0;
    for (i=0; i<max; i++) {
      logdf2 += log(norm(2.0*a/(x*x*x)));
      x = 1.0 + a/(x*x);
    }
    
    int l = logdf2/i * param.Value(0) + param.Value(1);
    //if (l<0) l = 0;

    return l;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);

    x = 1.0 + a/(x*x);
    
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
