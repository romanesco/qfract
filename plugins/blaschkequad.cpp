/*
 * Quadratic Blaschke product
 * z(z+a)/(1+conj(a)z)
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*(x+a)/(1.0+ac*x);

#define INCOND  (norm(x)>0.001)

const char* NAME = "Quadratic Blaschke product";
const char* CHILD = "";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 3;
double VALUE[N]={0.9, 0.0, 0.01};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "slope" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x, z.y);

    if (norm(x) > 1) return 0;

    complex<double> a(param.Value(0),param.Value(1));
    complex<double> ac(param.Value(0),-param.Value(1));
    
    register int i;
    for ( i=0; (i<max) && INCOND; i++ ) {
	FUNCTION
    }

  if (i<max)
      if ( (real(x)*param.Value(2) < imag(x)) &&
	   (-real(x)*param.Value(2)) > imag(x) )
	  return i;
  return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x, z.y);
    complex<double> a(param.Value(0),param.Value(1));
    complex<double> ac(param.Value(0),-param.Value(1));

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
