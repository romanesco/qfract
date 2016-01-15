/*
 * Cubic Blaschke product with superattrctive fixed point
 * z^2(z-a)(1-conj(a)z), a=exp(2*pi*c)
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*x*(x-l)/(1.0-lc*x);

#define INCOND  (norm(x)>eps)

const char* NAME = "Cubic Blaschke product w/ superattr fp";
const char* CHILD = "";

const double XL = -2.0;
const double YT = 1.0;
const double XR = 0.0;
const double YB = -1.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 15;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={0.0, 0.9};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(log(a))/(2*pi)", "Im(log(a))/(2*pi)" };

static double eps;
static complex<double> cv;

extern "C" {
void paraminit(Parameter param)
{
    complex<double> a(param.Value(0),param.Value(1)), l=exp(2*M_PI*a);
    complex<double> lc=conj(l);
    cout << l << endl << lc << endl;
    double n=norm(l);
    if (n>=1) {
	eps = 0.000001;
	return;
    }
    complex<double> b=3.0+n, x=b/lc*(1.0-sqrt(1.0-16.0*n/(b*b)))/4.0;
    cout << "cp: " << x << endl;

    while(norm(x)>0.000001)
    {
	FUNCTION
    }
    cv=x;
    eps = norm(cv);
}

int iter(Point z, Parameter param, int max)
{
    complex<double> y(z.x, z.y), x=exp(2*M_PI*y);

    if (norm(x) > 1) return -1;

    complex<double> a(param.Value(0),param.Value(1)), l=exp(2*M_PI*a);
    complex<double> lc=conj(l);
    
    register int i;
    for ( i=0; (i<max) && INCOND; i++ ) {
	FUNCTION
    }

    if (i<max) {
	//if (imag(x/cv) >=0)
	if (imag(-x*l) >=0)
	    return i*10;
	else 
	    return i*10+65;
    }
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x, z.y);
    x=exp(2*M_PI*x);
    complex<double> a(param.Value(0),param.Value(1)), l=exp(2*M_PI*a);
    complex<double> lc=conj(l);

    FUNCTION

    x=log(x)/(2*M_PI);
    return Point(real(x), imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
