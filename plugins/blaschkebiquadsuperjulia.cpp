/*
 * Biquadratic Blaschke product with superattrctive fixed point
 * z^2(z^2-a)(1-conj(a)z^2)
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*x*(x*x-l)/(1.0-lc*x*x);

#define INCOND  (norm(x)>eps)

const char* NAME = "Biquadratic Blaschke product w/ superattr fp";
const char* CHILD = "";

const double XL = -1.0;
const double YT = 1.0;
const double XR = 1.0;
const double YB = -1.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 15;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={0.0, 0.9};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(a)", "Im(a)" };

static double eps;
static complex<double> cv;

extern "C" {
void paraminit(Parameter param)
{
    complex<double> l(param.Value(0),param.Value(1));
    complex<double> lc(param.Value(0),-param.Value(1));
    double n=norm(l);
    if (n>=1) {
	eps = 0.000001;
	return;
    }
    complex<double> b=3.0+n, x=(1-sqrt(1-n))/lc;
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
    complex<double> x(z.x, z.y);

    if (norm(x) > 1) return -1;

    complex<double> l(param.Value(0),param.Value(1));
    complex<double> lc(param.Value(0),-param.Value(1));
    
    
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
    complex<double> l(param.Value(0),param.Value(1));
    complex<double> lc(param.Value(0),-param.Value(1));

    FUNCTION

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
