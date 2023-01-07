/*
 * Deg 4 rational map with 2 fixed crit pts and per 2 crit cycle
 * (by Navarro)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Navarro Map (parameter space)";
const char* CHILD= "navarrojulia.so";
//

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 0;
double VALUE[N]={};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={};

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> t(z.x, z.y);

    complex<double> a = (t+1.0)/(1.0-t);
    
    complex<double> b = (-2.0+a*(1.0 + a * (1.0+a) ) ) /
      (a* (1.0+ a * (1.0 + a*(1.0-2.0*a) ) ) );

    complex<double> x = (1.0-3.0*b)/ ( b*(b-3.0) );

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
  complex<double> t(c.x, c.y);
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
  complex<double> t(c.x, c.y);
  
  complex<double> a = (t+1.0)/(1.0-t);
  
  complex<double> b = (-2.0+a*(1.0 + a * (1.0+a) ) ) /
    (a* (1.0+ a * (1.0 + a*(1.0-2.0*a) ) ) );

  t = (1.0-3.0*b)/ ( b*(b-3.0) );

  return Point(real(t), imag(t));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
