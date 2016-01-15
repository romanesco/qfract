/*
 * Biquadratic family with fixed point
 * (phase space)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Biquad family (with fp 0)";
const char* CHILD = "";
const char* COLORMAP = "per2-2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=3;
double VALUE[N]={ 0.0, 0.0, 1 };
const Parameter PARAM(N,VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "Color interior" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> x(z.x, z.y);
  complex<double> a(param.Value(0),param.Value(1));
  
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
      return i*2+1;
    else 
      return i*2+41;
  } else if (param.Value(2)) {
    if (imag(a*x)>0)
      return i*2;
    else
      return i*2+40;
  }
  return -1;
}
    
Point map(Point z, Point c, Parameter param)
{
  complex<double> x(z.x, z.y);
  complex<double> a(param.Value(0),param.Value(1));
  x = x*x*(x*x+2.0*a);

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
