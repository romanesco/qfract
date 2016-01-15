/*
 * Mandelbrot set
 */

#include "../plugin.h"
using namespace QFract;

#include<complex>
using namespace std;

#define FUNCTION y = (y+2.0*(b-x))*y; \
  x *= x;

#define INCOND  ( norm(y)<10000 )

const char* NAME = "Jonsson's example (fiber space)";
const char* CHILD = "";
const char* COLORMAP = "default.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 1.0;
const double CIM = 0.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N=5;
double VALUE[N]={1.0, 0.0, 1.0, 0.0, 0.0};
const Parameter PARAM(N, VALUE);
const char * PARAMDESC[N] = { "Re(z)", "Im(z)", "Re(a)", "Im(a)", "unused" };

typedef complex<double> Complex;

static Complex x;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  Complex b(param.Value(2), param.Value(3));
  Complex x(param.Value(0),param.Value(1));
  Complex y(z.x,z.y);

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  if (i<max)
    {
      if (imag(y)>0)
	return i*2;
      else
	return i*2+41;
    }
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  Complex b(param.Value(2), param.Value(3));
  Complex y(z.x,z.y);

  FUNCTION

    return Point(real(y),imag(y));
}

Point init(Point c, Parameter param) 
{ 
  x = Complex(param.Value(0),param.Value(1)); 
  return c;
}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
