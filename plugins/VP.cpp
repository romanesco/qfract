/*
 * Fatou coordinate for exp(2*pi*i*alpha)Q(z-b)
 * (Q(z) = z(1+1/z)^6/(1-1/z)^4)
 */

#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

#include "../plugin.h"
using namespace QFract;

const char* NAME = "e^{2\\pi i alpha}Q(z-b)";
const char* CHILD = "";
const char* COLORMAP = "default.map";

const double XL = -2.2;
const double YT = 1.5;
const double XR = 0.8;
const double YB = -1.5;

const int MAXITER = 1;
const int MAXORBIT = 1;

// Parameters
// 0: eta (for the renormalization)
const int N = 2;
double VALUE[N] = {2.0, 1};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "eta", "0=range, 1=domain, 2=domain+V" };

complex<double> map( complex<double> x)
{
  complex<double> y;
  complex<double> t;
  
  t = x + 1.0;
  t *= t; 
  y = x * t ;
  return y;
}

const double cv=-4.0/27;
const double CRIT = -1.0/3;
const complex<double> I(0,1);


extern "C" {

int iter(Point z, Parameter param, int max)
{
  complex<double> x(z.x,z.y);
  int i=0;
  if (param.Value(1) == 2) {
    complex<double> y = 1.0/x;
    y = -1.0 - 2.0*y + 2.0*sqrt(y*(1.0+y));
    if (norm(y)<1) y = 1.0/y;

    double a = (real(y)+0.18)/1.24, b = imag(y)/1.04;
    if (a*a+b*b>1) {
      i += 32;
    }
  }
  if (param.Value(1)) {
    x = map(x);
  }
  double r=exp(2*M_PI*param.Value(0));
  i += (imag(x)>0) * 16;
  
  if ( abs(x) > -cv*r ) {
    return i;
  } else if ( abs(x) > -cv ) {
    return 64+i;
  } else if ( abs(x) > -cv/r ) {
    return 128+i;
  } else {
    return 192+i;
  }
}

Point map(Point z, Point, Parameter param)
{
  complex<double> x(z.x,z.y);
  x = map(x);
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
    PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				   PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
