/*
 * quadratic polynomials normalized s.t. fixed points are 0 and infinity
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Quadratic Map (infty -> 1, fixed pts=0, infty)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N]={0.1, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(log(a))/(2*pi*i)",
			     "Im(log(a))/(2*pi*i)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> p(z.x, z.y);
    complex<double> b(-param.Value(1), param.Value(0));
    complex<double> a=exp(2*M_PI*b);
    double R=norm(1.0-a)/100;
    register int i;
    for ( i=0; (i<max) && (norm(p-1.0) >R ); i++ ) {
	p = p*(p-2.0+a)/(p*a-1.0);
    }
    
    if (i<max)
	return i;
    else
	return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> p(z.x, z.y);
    complex<double> b(-param.Value(1), param.Value(0));
    complex<double> a=exp(2*M_PI*b);

    p = p*(p-2.0+a)/(p*a-1.0);

    return Point(real(p),imag(p));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
