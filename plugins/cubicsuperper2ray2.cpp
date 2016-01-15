/*
 * Cubic polynomial with period 2 critical point
 * z -> a z^3 - (a+1) z^2 +1 (parameter space with external rays)
 */

#include <cmath>
#include <complex>
using namespace std;

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     

#define FUNCTION tmp1 = re2-im2; \
                 tmp2 = 2*re*im; \
                 tmp3 = are * re - aim * im - are - 1; \
                 im   = aim * re + are * im - aim; \
                 re   = tmp1 * tmp3 - tmp2 * im + 1; \
                 im   = tmp2 * tmp3 + tmp1 * im;

#define INCOND  ( (re2+im2) < rad )

const char* NAME = "Cubic with superattr 2-per pt (parameter space with ray)";
const char* CHILD = "cubicsuperper2ray2julia.so";

const double XL = -3.5;
const double YT = 3.5;
const double XR = 3.5;
const double YB = -3.5;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> sqrta(z.x, z.y);
  sqrta = 1.0/sqrta;
  register double are=real(sqrta*sqrta);
  register double aim=imag(sqrta*sqrta);

  register double tmp1, tmp2, tmp3;

  INIT

  // first critical point
  tmp1 = 3.0 * (are * are + aim * aim);
  tmp2 = 2.0 * (are * (are + 1.0) + aim * aim);
  tmp3 = 2.0 * (-aim * (are + 1.0) + aim * are);
  register double re= tmp2 / tmp1;
  register double im= tmp3 / tmp1;

  register double re2=re*re;
  register double im2=im*im;

  register double rad = are*are + aim*aim;
  rad = sqrt(rad); rad = sqrt(rad);
  rad = 100*(rad+10/rad);

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else {
      complex<double> a(are,aim), x(re,im);
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
  complex<double> sqrta(c.x, c.y);
  sqrta = 1.0/sqrta;
  register double are=real(sqrta*sqrta);
  register double aim=imag(sqrta*sqrta);

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
    register double tmp1, tmp2, tmp3;
    complex<double> sqrta(c.x, c.y);
    sqrta = 1.0/sqrta;
    register double are=real(sqrta*sqrta);
    register double aim=imag(sqrta*sqrta);

    INIT

    // first critical point
    tmp1 = 3.0 * (are * are + aim * aim);
    tmp2 = 2.0 * (are * (are + 1.0) + aim * aim);
    tmp3 = 2.0 * (-aim * (are + 1.0) + aim * are);
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
