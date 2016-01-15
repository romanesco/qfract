/*
 * Cubic Newton maps (phase space)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Cubic Newton map (Julia set)";
const char* CHILD = "cubicnewtonjulia.so";
const char* COLORMAP = "per3.map";

const double XL = -1.5;
const double YT = 2.0;
const double XR = 2.5;
const double YB = -2.0;
//const double CRE = 0.0;
//const double CIM = 1.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N=3;
double VALUE[N]={0.0,0.0, 0.00001};
const Parameter PARAM(N,VALUE);
const char* PARAMDESC[N] = { "Re(d)", "Im(d)", "epsilon" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> d(param.Value(0),param.Value(1)), x(z.x, z.y), t;
    double eps = param.Value(2);

    register int i;
    if (d == 0.0) {
	for ( i=0; (i<max) && (norm(x)>eps) && (norm(x-1.0)>eps); i++ ) {
	    x = x*(2.0*x-1.0)/(3.0*x-2.0);
	}
    } else if (d == 1.0) {
	for ( i=0; (i<max) && (norm(x)>eps) && (norm(x-1.0)>eps); i++ ) {
	    x = 2.0*x*x/(3.0*x-1.0);
	}
    } else {
	for ( i=0; (i<max) && (norm(x)>eps) && (norm(x-1.0)>eps) &&
		  (norm(x-d)>eps); i++ ) {
	    t = x*x*(2.0*x-d-1.0);
	    x = (3.0*x-2.0*(d+1.0))*x+d;
	    x = t/x;
	}
    }
    
    if (i>=max)
	return -1;
    else if (norm(x)<=eps)
	return i*3;
    else if (norm(x-1.0)<=eps)
	return i*3+1;
    else
	return i*3+2;
}
    
Point map(Point z, Point c, Parameter param)
{
    complex<double> d(param.Value(0), param.Value(1)), x(z.x,z.y), t;

    if (d == 0.0) {
	x = x*(2.0*x-1.0)/(3.0*x-2.0);
    } else if (d == 1.0) {
	x = 2.0*x*x/(3.0*x-1.0);
    } else {
	t = x*x*(2.0*x-d-1.0);
	x = (3.0*x-2.0*(d+1.0))*x+d;
	x = t/x;
    }

    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
