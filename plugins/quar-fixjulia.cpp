/*
 * quartic family with fixed point at 0
 * z -> -z^4 + a*z^3 + b*z^2 + lambda*z
 * with critical point c (b = -(1/2)*(3*a*c^2-4*c^3+lambda)/c)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Quartic Family";
const char* CHILD = "";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N=7;
double VALUE[N]={1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "Re(c)","Im(c)", "Re(lambda)", "Im(lambda)", "epsilon" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> lambda(param.Value(4),param.Value(5)),
      a(param.Value(0), param.Value(1)),
      b, 
      c(param.Value(2), param.Value(3)),
      p(z.x, z.y);

    b = -((3.0*a - 4.0*c)*c*c + lambda)/(2.0*c);

    double epsilon = param.Value(6), R=1000;

    register int i;

    for (i=0; (i<max) && (norm(p)<R); i++) {
      p = (((-p+a)*p + b)*p + lambda)*p+epsilon;
    }

    if (i>=max)
	return -1;
    else
	return i;
	
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> lambda(param.Value(4),param.Value(5)),
      a(param.Value(0), param.Value(1)),
      b,
      d(param.Value(2), param.Value(3));

    b = -((3.0*a - 4.0*d)*d*d + lambda)/(2.0*d);

    double epsilon = param.Value(6);

    complex<double> p(z.x, z.y);
    
    p = (((-p+a)*p + b)*p + lambda)*p + epsilon;

    return Point(real(p),imag(p));
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
