/*
 * Cubic polynomial with period 3 critical point
 * z -> a z^3 + b z^2 +1 (parameter space)
 * with a = -(c^3-c^2+1)/(c^3-c^2) and b = (c^4-c^3+1)/(c^3-c^2)
 * ( f(0)=1, f(1)=a+b+1=c, f(c)=0 )
 */

#include <cmath>
#include <complex>
using namespace std;

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     tmp1 = cre*cre - cim*cim; \
                 tmp2 = 2*cre*cim; \
                 tmp3 = (cre-1.0)*tmp1 - cim*tmp2; \
                 tmp1 = (cre-1.0)*tmp2 + cim*tmp1; \
                 tmp2 = tmp3*tmp3 + tmp1*tmp1; \
                 are = -( (tmp3+1.0)*tmp3 + tmp1*tmp1 )/tmp2; \
                 aim = -(-(tmp3+1.0)*tmp1 + tmp1*tmp3 )/tmp2; \
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

const char* NAME = "Cubic with superattr 3-per pt (parameter space with ray)";
const char* CHILD = "cubicsuperper3ray_invertedjulia.so";
const char* COLORMAP = "per2.map";

const double XL = -1.0;
const double YT = 1.0;
const double XR = 1.0;
const double YB = -1.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 1;
double VALUE[N] = {0};
const Parameter PARAM(N, VALUE );
const char* PARAMDESC[N] = { "0-airplane, 1-rabbit, 2-corabbit" };

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
  int j = (int) param.Value(0);
  complex<double> x(z.x,z.y);
  x=invert(x,j);
  register double cre=real(x);
  register double cim=imag(x);

  register double are, aim, bre, bim, tmp1, tmp2, tmp3;

  INIT

  // first critical point -2b/3a
  tmp1 = 3.0 * (are * are + aim * aim);
  tmp2 = -2.0 * (are * bre + aim * bim);
  tmp3 = -2.0 * (-aim * bre + are * bim);
  register double re= tmp2 / tmp1;
  register double im= tmp3 / tmp1;

  register double re2=re*re;
  register double im2=im*im;

  //register double rad = (are*are + aim*aim >5) ? are*are + aim*aim : 5;
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
  int j = (int) param.Value(0);
  complex<double> x(c.x,c.y);
  x=invert(x,j);
  register double cre=real(x);
  register double cim=imag(x);

  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;
  register double tmp1, tmp2, tmp3;
  register double are, aim, bre, bim;

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
  int j = (int) param.Value(0);
  complex<double> x(c.x,c.y);
  x=invert(x,j);
  register double cre=real(x);
  register double cim=imag(x);
  register double tmp1, tmp2, tmp3;
  register double are, aim, bre, bim;

  INIT

  // first critical point
  tmp1 = 3.0 * (are * are + aim * aim);
  tmp2 = -2.0 * (are * bre + aim * bim);
  tmp3 = -2.0 * (-aim * bre + are * bim);
  register double re= tmp2 / tmp1;
  register double im= tmp3 / tmp1;
  
  return Point(re,im);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
