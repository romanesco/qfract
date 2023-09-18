/*
 * Quartic Blaschke product with superattractive fixed point of order 3
 * z^3(z+a)/(1+conj(a)z)
 * (phase space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*x*x*(x-l)/(1.0-lc*x);

#define INCOND  ( (norm(x)>eps) && (norm(x) < 1/eps) )

const char* NAME = "Quartic Blaschke product w/ superattr fp";
const char* CHILD = "";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

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
    complex<double> l(param.Value(0),param.Value(1));
    complex<double> lc(param.Value(0),-param.Value(1));
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
    complex<double> x(z.x, z.y);

    //if (norm(x) > 1) return -1;
    bool inD = (norm(x) < 1);
      
    complex<double> l(param.Value(0),param.Value(1));
    complex<double> lc(param.Value(0),-param.Value(1));
    complex<double> k=sqrt(-l);

    register int i;
    for ( i=0; (i<max) && INCOND; i++ ) {
	FUNCTION
    }

    if (i<max) {
      if (norm(x)<1) {
	return i*6 + inD*32 + (imag(x*k)>0)*16;
      } else {
	return i*6 + inD*32 + 8 + (imag(x*k)>0)*16;
      }
      /*
	//if (imag(x/cv) >=0)
	if (imag(-x*l) >=0)
	    return i*10;
	else 
	    return i*10+65;
      */
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
