/*
 * quartic map with fixed point and superattractive fixed point
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Quartic Map (0: fp, 1: safp, Julia set)";
const char* CHILD = "";
#define COLORMAP "default2.map"

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N=4;
double VALUE[N]={1.0, 0.0, (sqrt(5)-1)/2, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(b)", "Im(b)", 
			     "Re(log(a))/(2*pi*i)",
			     "Im(log(a))/(2*pi*i)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> p(z.x, z.y);
    complex<double> b(-param.Value(3), param.Value(2));
    complex<double> a=exp(2*M_PI*b);
    b=complex<double>(param.Value(0), param.Value(1));
    double R=100;
    register int i;
    for ( i=0; (i<max) && (norm(p) < R ); i++ ) {
	p = (p-1.0)*(p-1.0)*((b*p+(a-2.0))*p-1.0)+1.0;
    }
    
    if (i<max)
	return i*6;
    else
	return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> p(z.x, z.y);
    complex<double> b(-param.Value(3), param.Value(2));
    complex<double> a=exp(2*M_PI*b);
    b=complex<double>(param.Value(0), param.Value(1));
    
    p = (p-1.0)*(p-1.0)*((b*p+(a-2.0))*p-1.0)+1.0;
	
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
