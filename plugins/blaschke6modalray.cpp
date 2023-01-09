/*
 * 6-modal Blashke product ( (r2,eta2)-space)
 */

#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

#include "../plugin.h"
using namespace QFract;

// Description (displayed on window title)
const char* NAME = "6-modal Blaschke product ((r2,eta2)-space, with dynamical rays)";
// Child plugin filename (foo.so)
const char* CHILD = "blaschke6modalrayjulia.so";
// Default colormap file
const char* COLORMAP = "default.map";

// region to display (in (x,y)-coordinate)
const double XL = 1.0; // x left = minimal x value
const double YT = 1.0; // y top = maximal y value
const double XR = 2.0; // x right = maximal x value
const double YB = 0.0; // y bottom = minimal y value

// number of maximal iteration
const int MAXITER = 100;
// number of maximal iteration for forward orbit
const int MAXORBIT = 100;

// number of parameters
const int N = 8;
// default parameter values
double VALUE[N] = {1.2, 1.0/3.0, 6, 3, 2, 2, 0, 1.2};
const Parameter PARAM( N, VALUE );

// descriptions of parameters
const char* PARAMDESC[N] = { "r3", "eta3", "k", "k1", "k2", "k3", "b0", "r"};

// I=sqrt(-1)
const complex<double> I(0,1);

extern "C" {
// setting static parameters
void paraminit(Parameter param)
{
}

complex<double> power(complex<double> z, int k)
{
  if (k <0) {
    z = 1.0/z;
    k = -k;
  } else if (k==0) {
    return 1.0;
  }
  
  complex<double> w = z;
  for (int i=1; i<k; i++) {
    w *=z;
  }
  return w;
}

complex<double> f(complex<double> z, Point c, Parameter param)
{
  int k = param.Value(2);
  int k1 = param.Value(3);
  int k2 = param.Value(4);
  int k3 = param.Value(5);

  double b0 = param.Value(6);
  double r = param.Value(7);
  double r2 = c.x;
  double eta2 = c.y;
  double r3 = param.Value(0);
  double eta3 = param.Value(1);

  complex<double> lambda = exp(2*M_PI*b0*I);
  complex<double> b2 = r2 * exp(2*M_PI*eta2*I);
  complex<double> b3 = r3 * exp(2*M_PI*eta3*I);

  complex<double> w0, w1, w2, w3;
  w0 = power(z, k);
  w1 = (z - r)/(1.0-r*z);
  w1 = power(w1,k1);
  w2 = (z - b2)/(1.0-conj(b2)*z);
  w2 = power(w2,k2);
  w3 = (z - b3)/(1.0-conj(b3)*z);
  w3 = power(w3,k3);

  return lambda*w0*w1*w2*w3;
}

// do iteration for given point z
int iter(Point z, Parameter param, int max)
{
  return ((z.x > param.Value(7)) + (z.x > param.Value(0)) *2
	  + (z.y > param.Value(1))*4 )*32;

  // unused 
  complex<double> x(z.x,z.y);

  int i;
  for (i=0; (i<max) && (norm(x) > 0.01) && (norm(x) < 100); i++) {
    x = f(x, z, param);
  }
  
  if (i>=max){
    return -1;
  } else if (norm(x)<0.01) {
    return i*2;
  } else {
    return i*2 + 64;
  }

}

// for drawing forward orbit
Point map(Point z, Point c, Parameter param)
{
  return z;

  // unused
  complex<double> x(z.x,z.y);
  x = f(x, c, param);
  
  return Point(real(x),imag(x));
}

// initial point for drawing forward orbit
Point init(Point c, Parameter param) 
{
  return c;
}

// returning information of the plugin (normally no need to edit)
PluginInfo* getInfo()
{
    PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				   PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
