/*
 * Bobenrieth family 1+c/z^2
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Bobenrieth Map (phase space)";
const char* CHILD= "";
//

const double XL = -10;
const double YT = 10;
const double XR = 10;
const double YB = -10;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 6;
double VALUE[N]={-4, 0, 500, -10, 500, -10};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Re(c)", "Im(c)", "unused", "unused", "scale", "min"};

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x(z.x,z.y);

    int i;
    double logdf2 = 0;
    for (i=0; i<max; i++) {
      double df2 = norm(2.0*a/(x*x*x));
      x = 1.0 + a/(x*x);
      logdf2 += log(df2);
    }
    
    int l = logdf2/i * param.Value(4) + param.Value(5);
    //if (l<0) l = 0;

    return l;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> a(param.Value(0), param.Value(1));
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
