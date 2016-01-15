/*
 * Cubic polynomial with period 3 critical point
 * z -> a z^3 + b z^2 +1 (dynamical space)
 * with a = -(c^3-c^2+1)/(c^3-c^2) and b = (c^4-c^3+1)/(c^3-c^2)
 * ( f(0)=1, f(1)=a+b+1=c, f(c)=0 )
 */

#include <cmath>
#include <complex>
using namespace std;

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     tmp1 = cre*cre - cim*cim; \
                 tmp2 = 2*cre*cim; \
                 tmp3 = (cre-1)*tmp1 - cim*tmp2; \
                 tmp1 = (cre-1)*tmp2 + cim*tmp1; \
                 tmp2 = tmp1*tmp1 + tmp3*tmp3; \
                 are = -( (tmp3+1)*tmp3 + tmp1*tmp1 )/tmp2; \
                 aim =  ( (tmp3+1)*tmp1 - tmp1*tmp3 )/tmp2; \
                 bre = cre - are - 1; \
                 bim = cim - aim;

#define FUNCTION tmp1 = re2-im2; \
                 tmp2 = 2*re*im; \
                 tmp3 = are * re - aim * im + bre; \
                 im   = aim * re + are * im + bim; \
                 re   = tmp1 * tmp3 - tmp2 * im + 1; \
                 im   = tmp2 * tmp3 + tmp1 * im;

#define INCOND  (!( ( escape = ((re2+im2) > rad) ) || \
		    ( attracted = ((re2+im2) < epsilon) ) ) )

const char* NAME = "Cubic with superattr 3-per pt (Julia set with ray)";
const char* CHILD = "";
const char* COLORMAP = "per2-2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 3;
double VALUE[N] = {0.0, 1.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)",
			     "0-airplane, 1-rabbit, 2-corabbit" };

complex<double> c0[3] = 
  {
    -.75487766624669276007, // airplane
    complex<double>(.87743883312334638002,.74486176661974423660), // rabbit
    complex<double>(.87743883312334638002,-.74486176661974423660) // co-rabbit
  };

double scale[3] = 
  {
    10,
    50,
    50
  };

complex<double> invert(complex<double> x, int i)
{
  return 1.0/(scale[i]*x)+c0[i];
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
  int j = (int) param.Value(2);
  complex<double> x(param.Value(0), param.Value(1));
  x=invert(x,j);
  register double cre=real(x);
  register double cim=imag(x);

  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;
  register double are, aim, bre, bim, tmp1, tmp2, tmp3;

  INIT

  //register double rad = (are*are + aim*aim >20) ? are*are + aim*aim : 20;
  register double rad = are*are + aim*aim;
  rad = sqrt(rad); rad = sqrt(rad);
  rad = 100*(rad+10/rad);

  complex<double> a(are,aim), b(bre,bim), c(cre,cim),p;
  p = b*c*(3.0*a+2.0*b)*(3.0*a*c+2.0*b);
  double epsilon=0.0001/abs(p);

  complex<double> sqrta=sqrt(a);
  
  register int i;
  bool escape, attracted;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else {
    if (escape) {
      complex<double> x(re,im);
      x *= sqrta;
      if (imag(x) < 0)
	return i*4+24;
      return i*4;
    }
    if (attracted) {
      complex<double> x(re,im);
      x *= p;
      if (imag(x) < 0)
	return i*4+25;
      return i*4+1;
    }
  }
}

Point map(Point z, Point c, Parameter param)
{
  int j = (int) param.Value(2);
  complex<double> x(param.Value(0), param.Value(1));
  x=invert(x,j);
  register double cre=real(x);
  register double cim=imag(x);
  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;
  register double are, aim, bre, bim, tmp1,tmp2,tmp3;

  INIT

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
