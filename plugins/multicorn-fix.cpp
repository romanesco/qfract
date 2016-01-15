/*
 * Multicorn parametrized by a fixed point
 * i.e., c=a-conj(a)^d for conj(z)^d+c
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Multicorn parametrized by a fixed point";
const char* CHILD= "multicorn-fixjulia.so";
//

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={3,100, 10, 0.0001};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "degree", "R", "max detection period", "eps" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x, z.y), b=1.0;
  const int deg=param.Value(0), maxper=param.Value(2);
  const double eps=param.Value(3);
  for (int i=0; i<deg; i++) {
    b*=a;
  }
  b = a-conj(b);

  complex<double> x=0.0;
  complex<double> orbit[maxper];

  double R=param.Value(1);
  complex<double> tmp;
  
  if (norm(a) < pow(1.0/deg, 2.0/(deg-1))) {
    return 108;
  }

  register int i,j;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    if (i >= max-maxper) {
      orbit[i-max+maxper] = x;
    }
    tmp=x;
    for ( j=1; j<deg; j++)
      tmp *= x;
    x = conj(tmp) + b;
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
  complex<double> a(z.x, z.y), b=1.0;
  int deg=param.Value(0);
  for (int i=0; i<deg; i++) {
    b*=a;
  }
  b = a-conj(b);

  complex<double> x(z.x, z.y);
  complex<double> tmp;
  
  register int j;
  tmp=x;
  for ( j=1; j<deg; j++)
    tmp *= x;
  x = conj(tmp) + b;
  
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
