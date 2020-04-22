/*
 * Mandelbrot-like set for Henon family (x,y) -> (y+x^2+t, x)
 * with initial point (a,b)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Mandelbrot-like set for Henon family";
const char* CHILD= "";
const char* COLORMAP = "per2.map";
//const char* COLORMAP = "dim2.map";

const double XL = -1.0;
const double YT = 1.0;
const double XR = 1.0;
const double YB = -1.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N = 4; // number of parameters
double VALUE[N]={0,0,0,0}; // default parameter values
const Parameter PARAM( N, VALUE );
// names for parameters
const char* PARAMDESC[N]={ "Re(a)", "Im(a)", "Re(b)", "Im(b)" }; 

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(param.Value(0), param.Value(1)),
    b(param.Value(2), param.Value(3)), x(a), y(b);
  complex<double> t(z.x, z.y);
  complex<double> tmp;
  double R = 100;
  
  int i,j;
  // forward iteration
  for ( i=0; (i<max) && (norm(x) < R ); i++ ) {
    tmp = y + x*x + t;
    y = x;
    x = tmp;
  }
  if (i < max) {
    return i*2;
  }
  
  // backward iteration
  x = a; y = b;
  for ( j=0; (j<max) && (norm(x) < R ); j++ ) {
    tmp = x-y*y-t;
    x = y;
    y = tmp;
  }
  if (j<max) {
    return j*2+1;
  } 
  return -1;

  /*
  if ((i>=max) && (j>=max))
    return -1;
  else if (i>=max)
    return (j%15)+1;
  else if (j>=max)
    return ((i%15)+1)*16;
  else
    return ((i%15)+1)*16+(j%15)+1;
  */
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    complex<double> x(z.x, z.y);
    complex<double> tmp;

    register int j;
    tmp=x;
    for ( j=1; j<param.Value(0); j++)
	tmp *= x;
    x = tmp + a;
    
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
