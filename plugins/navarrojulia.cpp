/*
 * Deg 4 rational map with 2 fixed crit pts and per 2 crit cycle
 * (by Navarro)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Navarro Map (phase space)";
const char* CHILD= "";
const char* COLORMAP = "default2.map";
//

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={ 0, 0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Re(λ)", "Im(λ)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> t(param.Value(0), param.Value(1));

  complex<double> a = (t+1.0)/(1.0-t);
  
  complex<double> b = (-2.0+a*(1.0 + a * (1.0+a) ) ) /
    (a* (1.0+ a * (1.0 + a*(1.0-2.0*a) ) ) );
  
  complex<double> x(z.x, z.y);

    int i;

    for (i=0; (i<max) && (norm(x) > 0.0001) && (norm(x) < 10000) ; i++) {
      x = a*x*x*x*(b*x+(1.0-3.0*b)/2.0) /
	( (3.0-b)/2.0 * x -1.0);
    }
    
    
    if (i>=max)
      return -1;
    else if ( norm(x) >=10000 )
	return i*3;
    else
      return i*3+96;
}

Point map(Point z, Point c, Parameter param)
{
  complex<double> t(param.Value(0), param.Value(1));
  complex<double> x(z.x, z.y);
  
  complex<double> a = (t+1.0)/(1.0-t);
  
  complex<double> b = (-2.0+a*(1.0 + a * (1.0+a) ) ) /
    (a* (1.0+ a * (1.0 + a*(1.0-2.0*a) ) ) );
  
  x = a*x*x*x*(b*x+(1.0-3.0*b)/2.0) /
    ( (3.0-b)/2.0 * x -1.0);
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return c;
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
