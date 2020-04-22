/*
 * Tout function (asked by Kaoru Sano)
 * z(z+a)/(1+conj(a)z)
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*x*(a*x-1.0)/(x-a);

#define INCOND ((norm(x)<1/eps) &&  (norm(x)>eps))

const char* NAME = "Tout function (phase space)";
const char* CHILD = "";

const double XL = -1.0;
const double YT = 1.0;
const double XR = 1.0;
const double YB = -1.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
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
  eps = 0.000001;
  return;
  /*
  complex<double> a(param.Value(0),param.Value(1));
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
  */
}

int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x, z.y);

    complex<double> a(param.Value(0),param.Value(1)), a2=a*a;
    
    register int i;
    for ( i=0; (i<max) && INCOND; i++ ) {
	FUNCTION
    }

    if (i<max) {
	if (norm(x) >1)
	    return i*10;
	else 
	    return i*10+65;
    }
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x, z.y);
    complex<double> a(param.Value(0),param.Value(1));

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
