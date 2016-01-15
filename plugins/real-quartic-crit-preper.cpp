/*
 * Real quartic polynomial with preperiodic critical point
 * (parameter space)
 */

#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

#include "../plugin.h"
using namespace QFract;

const char* NAME = "Real quartic polynomial with preperiodic critical point";
const char* CHILD = "real-quartic-crit-preperjulia.so";
const char* COLORMAP = "per2.map";

const double XL = 5.0;
const double YT = 5.0;
const double XR = -5.0;
const double YB = -5.0;

const int MAXITER = 100;
const int MAXORBIT = 10;

// number of parameters
const int N = 0;
// default parameter values
double VALUE[N] = {};
const Parameter PARAM( N, VALUE );
// descriptions
const char* PARAMDESC[N] = { };

const complex<double> I(0,1);

extern "C" {
// setting static parameters
void paraminit(Parameter param)
{
}

int iter(Point z, Parameter param, int max)
{
  double c=z.x, p=z.y, q, r;
  
  q = p*(3-2*c)/4 + 3*(1+2*c)/(c*c*(c+1)*(c+1));
  //r = -((3+4*c)*p - (4+6*c)*q)/(6*(1+2*c));
  r = 2/(c*c) - (c*c*p + 2*c*q)/6;

  double a4=p/4, a3=(q-c*p)/3, a2=(r-c*q)/2, a1=-c*r;

  double D=q*q-4*p*r;
  complex<double> x=(-q+sqrt((complex<double>) D))/(2*p);

  int i,j;
  for (i=0; (i<max) && (norm(x)<100); i++) {
    x = x*(a1 + x*(a2 + x*(a3 + a4*x)));
  }

  if (D<0) {
    // complex critical points
    if (i>=max) {
      return -1;
    }
    if (imag(x)>0)
      return i*8;
    else
      return i*8+21;
  } else {
    // real critical points
    x=(-q-sqrt(D))/(2*p);
    for (j=0; (j<max) && (norm(x)<100); j++) {
      x = x*(a1 + x*(a2 + x*(a3 + x*a4)));
    }
    if ((i>=max) && (j>=max)) {
      return -1;
    }
    if (i>=max)
      return i*8;
    else if (j>=max)
      return i*8+21;
  }
}

Point map(Point z, Point c0, Parameter param)
{
  complex<double> x(z.x,z.y);

  double c=c0.x, p=c0.y, q, r;
  
  q = p*(3-2*c)/4 + 3*(1+2*c)/(c*c*(c+1)*(c+1));
  //r = -((3+4*c)*p - (4+6*c)*q)/(6*(1+2*c));
  r = 2/(c*c) - (c*c*p + 2*c*q)/6;

  double a4=p/4, a3=(q-c*p)/3, a2=(r-c*q)/2, a1=-c*r;
  x = x*(a1 + x*(a2 + x*(a3 + x*a4)));

  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
  return Point(param.Value(0),0);
}

PluginInfo* getInfo()
{
    PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				   PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
