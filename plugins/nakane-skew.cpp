/*
 * quadratic maps az(1+z) (parameter space)
 */

#include "../plugin.h"
using namespace QFract;

#include<complex>
using namespace std;

/*
#define RE2IM2   re2=re*re;			\
                 im2=im*im;

#define FUNCTION im = 2*re*im;		\
                 re = re2-im2;
*/

#define FUNCTION y = (y+s*2.0*(a-x))*y; \
  x *= x+b;

#define INCOND  ( norm(y)<10000 )

const char* NAME = "Nakane's example (base space)";
const char* CHILD = "nakane-skew2.so";
const char* COLORMAP = "default.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N=6;
double VALUE[N]={1.0, 0.0, 0.0, 0.0, 1.0, 0};
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "Re(a)", "Im(a)", "Re(b)", "Im(b)", "sgn", "CritOrbit?" };

typedef complex<double> Complex;

static Complex x;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  Complex a(param.Value(0), param.Value(1));
  Complex b(param.Value(2), param.Value(3));
  double s(param.Value(4));
  Complex x(z.x,z.y);
  Complex y=x-1.0;
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  int n = ((z.x*z.x+z.y*z.y)>1);

  if (i<max)
    {
      if (imag(y)>0)
	return i*2+n*128;
      else
	return i*2+41+n*128;
    }
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  Complex a(param.Value(0), param.Value(1));
  Complex b(param.Value(2), param.Value(3));
  double s(param.Value(4));
  if (param.Value(5)) {
    Complex y(z.x,z.y);
    FUNCTION
    return Point(real(y),imag(y));
  } else {
    return Point(z.x*z.x-z.y*z.y, 2*z.x*z.y);
  }
}

Point init(Point c, Parameter param)
{ 
  if (param.Value(5)) {
    x=Complex(c.x, c.y);
    return Point(c.x-1, c.y);
  } else {
    return c;
  }
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
