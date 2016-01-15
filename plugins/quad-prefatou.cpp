/*
 * quadratic maps in the pre-Fatou coordinate
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Quadratic Julia set in the pre-Fatou coordinate";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -10.0;
const double YT = 10.0;
const double XR = 10.0;
const double YB = -10.0;
const double CRE = 0.1;
const double CIM = 0.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N]={0.1, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(log(a))/(2*pi*i)",
			     "Im(log(a))/(2*pi*i)" };

const complex<double> I(0,1);

extern "C" {
    
complex<double> prefatou(complex<double> alpha, complex<double> w)
{
    if (alpha != 0.0) {
	complex<double> lambda = exp(2.0*M_PI*I*alpha),
	    sigma = 1.0/lambda-1.0;
	return (sigma/(1.0-exp(-2.0*M_PI*I*alpha*w)));
    } else 
	return 1.0/w;
}

int iter(Point z, Parameter param, int max)
{
    complex<double> w(z.x, z.y), x;
    complex<double> a(param.Value(0),param.Value(1)),
	lambda = exp(2.0*M_PI*I*a);
    if (norm(w)<0.0001) return 0;
    x = prefatou(a,w);

    register int i;
    for ( i=0; (i<max) && (norm(x)<10); i++ ) {
	x = lambda*x*(1.0+x);
    }

    if (i<max)
	return i;
    else
	return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> w(z.x, z.y), x;
    complex<double> a(param.Value(0),param.Value(1)),
	lambda = exp(2.0*M_PI*I*a),
	sigma = 1.0/lambda-1.0;
    x = prefatou(a,w);
    x = lambda*x*(1.0+x);
    
    w = -log(1.0-sigma/x)/(2.0*M_PI*I*a);
    if (norm(w) < 0.0001) return Point(0.0,0.0);
    return Point(real(w),imag(w));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
