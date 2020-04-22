/*
 * parameter slice (t-plane) of
 * Henon map 
 * check f^n(p0) is bounded for fixed p0
 * f_c: (x, y) |-> (y, x + y^2 + s y + t)
 */

#include <complex>
using namespace std;

typedef complex<double> Complex;
struct C2 {
  Complex x,y;
  C2() {}
  C2(const C2 &w) { x=w.x; y=w.y; }
  C2(const Complex &a, const Complex &b) {x=a; y=b;}
};

#include "../plugin.h"
using namespace QFract;

const char* NAME = "Henon slice (t-plane) for fixed p0: (y, x+y^2+sy+t)";
const char* CHILD = "henonslicephase.so";
const char* COLORMAP = "default.map";

const double XL = -2.25;
const double YT = 1.5;
const double XR = 0.75;
const double YB = -1.5;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N=6;
double VALUE[N] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "Re(s)", "Im(s)",
			      "Re(x0)", "Im(x0)",
			      "Re(y0)", "Im(y0)" };

C2 f(const Complex &s, const Complex &t, const C2 &z)
{
  C2 w;
  w.x = z.y;
  w.y = z.x + z.y*z.y + s*z.y + t;
  return w;
}

bool incond(const C2 &z)
{
  return (norm(z.x)+norm(z.y) < 100);
}

extern "C" {
int iter(Point c, Parameter param, int max)
{
  Complex t(c.x, c.y), s(param.Value(0), param.Value(1));
  C2 z(Complex(param.Value(2), param.Value(3)),
       Complex(param.Value(4), param.Value(5)));
  
  register int i;
  for ( i=0; (i<max) && incond(z); i++ ) {
    z = f(s,t,z);
  }
  
  if (i<max)
    return i;
  else
    return -1;
}

// so far no way to iterate point in C^2
Point map(Point z, Point c, Parameter param)
{
  return z;
  /*
  Complex t(c.x, c.y), s(param.Value(0), param.Value(1));
  C2 z(Complex(param.Value(2), param.Value(3)),
       Complex(param.Value(4), param.Value(5)));
  
  z = f(s, t, z);

  return Point(re(z.x),im(z.x));
  */
  
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
