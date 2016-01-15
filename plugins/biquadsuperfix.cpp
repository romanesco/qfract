/*
 * Biquadratic family with fixed point
 * (parameter space)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Biquad family (with fp 0)";
const char* CHILD = "biquadsuperfixjulia.so";
const char* COLORMAP = "per2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=1;
double VALUE[N]={ 1 };
const Parameter PARAM(N,VALUE);
const char* PARAMDESC[N] = { "Color interior" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x, z.y);
  complex<double> x=-a*a;
  
  register int i;
  bool attracted=false;
  for ( i=0; (i<max) && (norm(x)<100); i++ ) {
    x = x*x*(x*x+2.0*a);
    if (norm(a*x)<0.0001) {
      attracted=true; break;
    }
  }
  
  if (i>=max)
    return -1;
  else if ( !attracted ) {
    if (imag(x)>0)
      return i*2;
    else 
      return i*2+40;
  } else if (param.Value(0)) { 
    if (imag(a*x)>0)
      return i*2+1;
    else
      return i*2+41;
  }
  return -1;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> a(z.x, z.y);
  complex<double> x(z.x,z.y);

  x = x*x*(x*x+2.0*a);
  return Point(real(x),imag(x));
}

//Point init(Point c, Parameter param) {return Point(-0.5, 0.0);}
Point init(Point c, Parameter param) 
{
  complex<double> a(c.x, c.y), x=-a*a;

  return Point(real(x), imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
