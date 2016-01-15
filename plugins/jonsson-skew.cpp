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

#define FUNCTION y = (y+2.0*(b-x))*y; \
  x *= x;

#define INCOND  ( norm(y)<10000 )

const char* NAME = "Jonsson's example (base space)";
const char* CHILD = "jonsson-skew2.so";
const char* COLORMAP = "default.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 200;
const int MAXORBIT = 10;
const int N=3;
double VALUE[N]={1.0, 0.0, 0};
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "Re(a)", "Im(a)", "CritOrbit?" };

typedef complex<double> Complex;

static Complex x;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  Complex b(param.Value(0), param.Value(1));
  Complex x(z.x,z.y);
  Complex y=x-1.0;
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  int a = ((z.x*z.x+z.y*z.y)>1);

  if (i<max)
    {
      if (imag(y)>0)
	return i*2+a*128;
      else
	return i*2+41+a*128;
    }
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  Complex b(param.Value(0), param.Value(1));
  if (param.Value(2)) {
    Complex y(z.x,z.y);
    FUNCTION
    return Point(real(y),imag(y));
  } else {
    return Point(z.x*z.x-z.y*z.y, 2*z.x*z.y);
  }
}

Point init(Point c, Parameter param)
{ 
  if (param.Value(2)) {
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
