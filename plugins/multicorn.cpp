/*
 * Multicorn conj(z)^d+c (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Multicorn";
const char* CHILD= "Multicornjulia.so";
//

const double XL = -1.5;
const double YT = 1.5;
const double XR = 1.5;
const double YB = -1.5;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={3,100, 10, 0.0001};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "degree", "R", "max detection period", "eps" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x, z.y);
  complex<double> x=a;
  const int deg=param.Value(0), maxper=param.Value(2);
  const double R=param.Value(1), eps=param.Value(3);
  
  complex<double> tmp;
  complex<double> orbit[maxper];
  
  register int i,j;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    if (i >= max-maxper) {
      orbit[i-max+maxper] = x;
    }
    tmp=x;
    for ( j=1; j<deg; j++)
      tmp *= x;
    x = conj(tmp) + a;
  }
  
  if (i>=max) {
    for (int k=0; k<maxper; k++) {
      if (norm(x-orbit[maxper-k-1]) < eps) {
	return k*20+128;
      }
    }
    return -1;
  } else
    return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);
    complex<double> tmp;

    register int j;
    tmp=x;
    for ( j=1; j<param.Value(0); j++)
	tmp *= x;
    x = conj(tmp) + a;
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    return c;
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
