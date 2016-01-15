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

#define INCOND  ( (re2+im2) < rad )

const char* NAME = "Cubic with superattr 3-per pt (parameter space with ray)";
const char* CHILD = "cubicsuperper3rayjulia.so";

const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double cre=z.x;
  register double cim=z.y;

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

  complex<double> a(are,aim), sqrta=sqrt(a);

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else {
      complex<double> x(re,im);
      x *= sqrta;
      if (imag(x) < 0)
	  return i*4+11;
      return i*4;
  }
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;
  register double tmp1, tmp2, tmp3;
  register double cre=c.x;
  register double cim=c.y;
  register double are, aim, bre, bim;

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
    register double tmp1, tmp2, tmp3;
    register double cre=c.x;
    register double cim=c.y;
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
				 PARAM, CHILD);
  return i;
}
}
