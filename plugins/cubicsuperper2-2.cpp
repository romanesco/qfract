/*
 * Cubic polynomial with period 2 critical point
 * z -> a z^3 - (a+1) z^2 +1 (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"
#include <complex>
using namespace std;

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

const char* NAME = "Cubic with superattr 2-per pt (parameter space)";
const char* CHILD = "cubicsuperper2-2julia.so";

const double XL = -10.0;
const double YT = 6.5;
const double XR = 3.0;
const double YB = -6.5;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x,z.y);
  a = 1.0/a;
  register double are=real(a);
  register double aim=imag(a);

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
  rad = 5*(rad + 1/rad);

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else 
    return i;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;
  register double tmp1, tmp2, tmp3;
  complex<double> a(c.x,c.y);
  a = 1.0/a;
  register double are=real(a);
  register double aim=imag(a);

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
    register double tmp1, tmp2, tmp3;
    complex<double> a(c.x,c.y);
    a = 1.0/a;
    register double are=real(a);
    register double aim=imag(a);

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
