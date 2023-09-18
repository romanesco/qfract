/*
 * Quartic Blaschke product with superattrctive fixed point of order 3
 * z^3(z-a)/(1-conj(a)z)
 * (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define FUNCTION x = x*x*x*(x-a)/(1.0-ac*x);

#define INCOND  ( (norm(x)>0.000001) && (norm(x) < 1000000 ) )

const char* NAME = "Quartic Blaschke product w/ superattr fp";
const char* CHILD = "blaschkequarticsuperjulia.so";

const double XL = -5.0;
const double YT = 5.0;
const double XR = 5.0;
const double YB = -5.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
double VALUE[0]={};
const Parameter PARAM( 0, VALUE );

extern "C" {

int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    complex<double> ac(z.x, -z.y);
    complex<double> k=sqrt(-a);

    //if (norm(a) > 1) return -1;

    double n=norm(a);
    complex<double> b=2.0+n, x=b/ac*(1.0-sqrt(1.0-9.0*n/(b*b)))/3.0;
    
    register int i;
    if (norm(a)<1) {
      for ( i=0; (i<max) && INCOND; i++ ) {
	FUNCTION;
      }
      
      if (i<max) {
	if (imag(k*x) >=0)
	  return i*10;
	else 
	  return i*10 + 16;
      }
      return -1;
    } else {
      int maxper = 10;
      complex<double> *orbit = new complex<double>[maxper];   
      for ( i=0; (i<max) && INCOND; i++ ) {
	if (i >= max-maxper) {
	  orbit[i-max+maxper] = x;
	}
	FUNCTION;
      }
      
      if (i>=max) {
	for (int j=0; j<maxper; j++) {
	  if (norm(x-orbit[maxper-j-1]) < 0.000001) {
	    delete[] orbit;
	    return j*20+128;
	  }
	}
	delete[] orbit;
	return -1;
      } else {
	delete[] orbit;
	if (imag(k*x) >=0)
	  return i*10 + 32;
	else
	  return i*10 + 48;
      }
    }
}
Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x, z.y);
    complex<double> a(z.x, z.y);
    complex<double> ac(z.x, -z.y);

    FUNCTION

    return Point(real(x), imag(x));
}

Point init(Point c, Parameter param) {
    complex<double> a(c.x, c.y);
    complex<double> ac(c.x, -c.y);

    double n=norm(a);
    complex<double> b=2.0+n, x=b/ac*(1.0-sqrt(1.0-9.0*n/(b*b)))/3.0;
    return Point(real(x),imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD);
  return i;
}
}
