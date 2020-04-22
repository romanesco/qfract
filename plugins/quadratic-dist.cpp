/*
 * Quadratic family z^2+c (colored w.r.t. the distance)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Quadratic family (parameter space colored w.r.t. the distance)";
const char* CHILD = "quadratic-distjulia.so";
const char* COLORMAP = "darkblue.map";

const double XL = -2.5;
const double YT = 2.0;
const double XR = 1.5;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N]={300,1};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "R", "Normalization const" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x, z.y);
  complex<double> x=a;
  double R=param.Value(0);
  complex<double> tmp, dx=1;
  
  register int i;
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    dx = 2.0*dx*x+1.0; // dx = d/dc ((f^n)(c))
    /*
    // dx = d/dc ((f^n)(c)) / 2^n
    dx = dx*x;
    if (i < 1000) {
      dx += 1.0/pow(2,i+1);
    }
    */
    x = x*x + a;
  }
  
  if (i>=max) {
    return -1;
  }

  double logg, g, dg, metric;
  /*
  g=log(norm(x))/pow(2,i+1);
  dg = abs(dx)/abs(x);
  metric =dg/sinh(g);
  */
  
  // approximate sinh(g) by g (don't care when g large)
  g = log(norm(x));
  dg = abs(dx)/abs(x);
  metric = dg/g;
  
  if (isnan(g))
    return 1;
  
  return (int) sqrt(metric)/param.Value(1);
}
  
Point map(Point z, Point c, Parameter param)
{
  complex<double> a(c.x, c.y);
  complex<double> x(z.x, z.y);
  complex<double> tmp;
  
  register int j;
  x = x*x + a;
  
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
