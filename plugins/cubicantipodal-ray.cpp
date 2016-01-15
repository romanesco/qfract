/*
 * Cubic antipodal preserving map (parameter space)
 * z -> z^2(z-q)/(1+conj(q)z)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

#define FUNCTION x = x*x*(x-q)/(1.0+conj(q)*x);

#define INCOND ( ( norm(x)<1/eps ) && (norm(x)>eps) )

const char* NAME = "Cubic antipode preserving map (parameter space)";
const char* CHILD = "cubicantipodal-rayjulia.so";
const char* COLORMAP = "default.map";

const double XL = -10.0;
const double YT = 10.0;
const double XR = 10.0;
const double YB = -10.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {10, 0.0001};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "max detection period", "eps" };
const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double eps=param.Value(1);
  complex<double> q(z.x,z.y), qbar(z.x,-z.y),x, fp;
  double Q=norm(q);
  const int maxper=param.Value(0);
  complex<double> orbit[maxper];
  x = ( (-3+Q+sqrt((Q+1)*(Q+9)))/4.0 )/qbar;
  
  orbit[0]=x;
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    if (i >= max-maxper) {
      orbit[max-1-i] = x;
    }
    FUNCTION;
  }

  if (i<max) {
    if (norm(x) < eps)
      return 2*i + 50*(imag(-x/q)>0);
    else
      return 2*i+1 + 50*(imag(q*x)>0);
  } else
    // attracting cycle detection
    for (int j=0; j<maxper-1; j++) {
      if (norm(x-orbit[j]) < eps) 
	return j*20+128;
    }
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> q(c.x,c.y), x(z.x,z.y);

  FUNCTION
    
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param)
{
 complex<double> a(c.x,c.y), x;
 double A=norm(a);
 x = ( (-3+A+sqrt((A+1)*(A+9)))/4.0 )/conj(a);
 return Point( real(x), imag(x));
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
