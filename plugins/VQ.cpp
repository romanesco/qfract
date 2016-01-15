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

const char* NAME = "V' for Q";
const char* CHILD = "";
const char* COLORMAP = "default.map";

const double XL = -30;
const double YT = 30;
const double XR = 30;
const double YB = -30;

const int MAXITER = 500;
const int MAXORBIT = 2500;

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
  t *= t; y = t*t;
  y *= t;
  
  t = x - 1.0;
  t *= t; t *= t;
  y /= x * t ;
  return y;
}

const double cv=27;
const double CRIT = 5+sqrt(24);
const complex<double> I(0,1);


extern "C" {

int iter(Point z, Parameter param, int max)
{
  int i=0;
  complex<double> x(z.x,z.y);
  if (param.Value(1) == 2) {
    double a = (real(x)+0.18)/1.24, b = imag(x)/1.04;
    if (a*a+b*b>1) {
      i += 32;
    }
  }
  if ( param.Value(1) ) {
    if (norm(x) < 1) { 
      return -1;
    }
    x = map(x);
  }

  double r=exp(2*M_PI*param.Value(0));
  i += (imag(x)>0) * 16;
  
  if ( abs(x) < cv/r ) {
    return i;
  } else if ( abs(x) < cv ) {
    return 64+i;
  } else if ( abs(x) < cv*r ) {
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
