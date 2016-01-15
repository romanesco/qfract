/*
 * Rational map coming from Pott model
 * U_{mn\lambda}(z) = t_{m\lambda} (t_{n\lambda}(z),
 * where t_{n\lambda}(z) = ( (z+lambda-1)/(z-1) )^n.
 * (Julia set)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Rational map coming from Pott model (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "per2-2.map";

#define FUNCTION x = uintpow( (x+a-1.0)/(x-1.0), n); \
  x = uintpow((x+a-1.0)/(x-1.0), m);

// for x close to 1
#define FUNCTION2 x = uintpow( (x-1.0)/(x+a-1.0), n); \
  x = uintpow( (1.0+(a-1.0)*x)/(1.0-x), m);

const double XL = -5.0;
const double YT = 5.0;
const double XR = 5.0;
const double YB = -5.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={0.0, 0.0, 2, 2};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(λ)", "Im(λ)", "n", "m" };

complex<double> uintpow(const complex<double> &z,
			const unsigned int &n)
{
  complex<double> y=z;
  for (int i=1; i<n; i++) {
    y *= z;
  }
  return y;
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
  int n=param.Value(2), m=param.Value(3);
  complex<double> a(param.Value(0), param.Value(1));
  complex<double> x(z.x,z.y);
  
  register int i;
  double nm;
  for (i=0; i<max; i++) {
    nm=norm(x);
    if ( (nm > 10000) || (norm(x-1.0) < 0.0001) ) {
      break;
    }
    FUNCTION;
  }
  
  if (i>=max)
    return -1;
  else 
    return i*2 + (nm>10000);
}

Point map(Point z, Point c, Parameter param)
{
  int n=param.Value(2), m=param.Value(3);
  complex<double> a(param.Value(0), param.Value(1));
  complex<double> x(z.x,z.y);

  if (norm(x-1.0) >  0.01) {
    FUNCTION;
  } else {
    FUNCTION2;
  }
    
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
