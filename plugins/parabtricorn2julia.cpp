/*
 * Parabolic tricorn e^{2\pi i α}conj(z+1/z)+A (Julia set)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Parabolic tricorn (Julia set)";
const char* CHILD = "";
//const char* COLORMAP = "per2-2.map";
const char* COLORMAP = "default2.map";

const double XL = -5.0;
const double YT = 5.0;
const double XR = 5.0;
const double YB = -5.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 500;
const int MAXORBIT = 1000;
const int N = 6;
double VALUE[N]={0, 1, 0, 0, 0, 0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "alpha", "Re(A)", "Im(A)", "", "", "invert" };

const complex<double> I(0,1);

static double R=200;

extern "C" {
void paraminit(Parameter param)
{
  complex<double> A(param.Value(1), param.Value(2));
  double alpha=param.Value(0);
  complex<double> lambda=exp(2*M_PI*I*alpha);
  A = A*exp(M_PI*I*alpha);
  
  complex<double> T = conj(A)*lambda +A;
  if (norm(T)<0.01) T = T/abs(T)*0.1;
  
  complex<double> x=1.0;
  
  for ( int i=0; i<200; i++ ) {
    x = lambda*conj(x+1.0/x)+A;
  }
  R = real(x/T);

  if (R<10) R=10;
}
  
int iter(Point z, Parameter param, int max)
{
  complex<double> A(param.Value(1), param.Value(2));
  double alpha=param.Value(0);
  complex<double> lambda=exp(2*M_PI*I*alpha);
  A = A*exp(M_PI*I*alpha);
  
  complex<double> T = conj(A)*lambda +A;
  if (norm(T)<0.01) T = T/abs(T)*0.1;
  
  complex<double> x(z.x,z.y);
  if (param.Value(5)>0) {
    x = 1.0/x;
  }
  
  register int i;
  for ( i=0; (i<max) && (real(x/T) < R ); i++ ) {
    x = lambda*conj(x+1.0/x)+A;
  }
  
  if (i>=max)
    return -1;
  return i + ((imag(x/T)-param.Value(2)/2>0) ? 65 : 0);
}
  
Point map(Point z, Point c, Parameter param)
{
  complex<double> A(param.Value(1), param.Value(2));
  double alpha=param.Value(0);
  complex<double> lambda=exp(2*M_PI*I*alpha);
  complex<double> x(z.x,z.y);
  A = A*exp(M_PI*I*alpha);
  
  if (param.Value(5)>0) {
    x = 1.0/x;
  }
  x = lambda*conj(x+1.0/x)+A;

  if (param.Value(5)>0) {
    x = 1.0/x;
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
