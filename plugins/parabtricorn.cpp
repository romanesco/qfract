/*
 * Parabolic tricorn e^{2\pi i α}conj(z+1/z)+A (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Parabolic tricorn (parameter space)";
const char* CHILD= "parabtricornjulia.so";
//

const double XL = -2.5;
const double YT = 2.5;
const double XR = 2.5;
const double YB = -2.5;

const int MAXITER = 1000;
const int MAXORBIT = 100;
const int N = 3;
double VALUE[N]={0, 10, 0.0001 };
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "alpha", "max period", "eps" };

const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double eps = param.Value(2);
  const int maxper = param.Value(1);
  complex<double> *orbit = new complex<double>[maxper];
  
  complex<double> A(z.x, z.y);
  complex<double> x=-1;
  double alpha=param.Value(0);
  complex<double> lambda=exp(2*M_PI*I*alpha);
  
  complex<double> T = conj(A)*lambda +A;
  if (norm(T)<0.01) T = T/abs(T)*0.1;

  register int i;
  for ( i=0; (i<max) && (real(x/T) < 100 ); i++ ) {
    if ( i>= max-maxper ) {
      orbit[i-max+maxper] = x;
    }
    x = lambda*conj(x+1.0/x)+A;
  }
  
  if (i>=max) {
    for (int j=0; j<maxper; j++) {
      if (norm(x-orbit[maxper-j-1]) < eps) {
	delete[] orbit;
	return j*20+128;
      }
    }
    delete[] orbit;
    return -1;
  } else {
    delete[] orbit;
    return i;
  }
}
  
Point map(Point z, Point c, Parameter param)
{
    complex<double> A(c.x, c.y);
    complex<double> x(z.x, z.y);
    double alpha=param.Value(0);
    complex<double> lambda=exp(2*M_PI*I*alpha);
    
    x = lambda*conj(x+1.0/x)+A;
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(-1,0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
