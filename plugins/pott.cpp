/*
 * Rational map coming from Pott model
 * U_{mn\lambda}(z) = t_{m\lambda} (t_{n\lambda}(z),
 * where t_{n\lambda}(z) = ( (z+lambda-1)/(z-1) )^n.
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Rational map coming from Pott model (parameter space)";
const char* CHILD= "pottjulia.so";
//const char* COLORMAP = "per2.map";
const char* COLORMAP = "default.map";

#define FUNCTION x = uintpow( (x+a-1.0)/(x-1.0), n); \
  x = uintpow((x+a-1.0)/(x-1.0), m);

// for x close to 1
#define FUNCTION2 x = uintpow( (x-1.0)/(x+a-1.0), n);	\
  x = uintpow( (1.0+(a-1.0)*x)/(1.0-x), m);

const double XL = -2.0;
const double YT = 3.0;
const double XR = 4.0;
const double YB = -3.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={2,2};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "n", "m" };

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
  unsigned int n=param.Value(0), m=param.Value(1);
  complex<double> a(z.x, z.y);
  complex<double> x(0);
  double R=param.Value(1);
  complex<double> tmp;
  
  bool attractedtoinfinity=false;
  register int i,j;
  // first critical value=0
  for (i=0; i<max; i++) {
    double nm=norm(x);
    if ( (nm > 10000) || (norm(x-1.0) < 0.0001) ) {
      if (nm > 10000)
	attractedtoinfinity = true;
      break;
    }
    FUNCTION;
  }

  //second critical value
  x=uintpow(1.0-a, m);
  for (j=0; j<max; j++) {
    if ( (norm(x) > 10000) || (norm(x-1.0) < 0.0001) )
      break;
    FUNCTION;
  }
  
  if ( (i>=max) && (j>= max) ) 
    return -1;
  else
    return ((i<max) + (j<max)*2 +
	    attractedtoinfinity*4 + (norm(x)>10000)*8) *16;
  /*
  else if (i < j) 
    return i*4+attractedtoinfinity*22;
  else
    return j*4+1+(norm(x)>10000)*22;
  */
}
  
 Point map(Point z, Point c, Parameter param)
{
  unsigned int n=param.Value(0), m=param.Value(1);
  complex<double> a(c.x, c.y);
  complex<double> x(z.x, z.y);
  
  if (norm(x-1.0) >  0.01) {
    FUNCTION;
  } else {
    FUNCTION2;
  }

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(0,0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
