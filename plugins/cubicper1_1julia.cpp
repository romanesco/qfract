/*
 * Cubic polynomials with fixed point
 * z -> z^3 - a z^2 + b z (dynamical space)
 */

#include "../plugin.h"
using namespace QFract;
#include <cmath>
#include <complex>
using namespace std;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     

#define FUNCTION tmp1 = (re-are)*re-(im-aim)*im; \
                 tmp2 = (re-are)*im+(im-aim)*re; \
		 tmp3 = (tmp1+1)*re - tmp2*im; \
		 im = (tmp1+1)*im + tmp2*re; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<1000 )

const char* NAME = "Cubic (with fix pt 0, Julia set)";
const char* CHILD = "";
const char* COLORMAP = "per2-2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 1000;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N] = {0.0, 0.0, 1.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "|b|", "arg(b)/(2*pi)" };

static double ceh1, ceh2;
static double phase;

extern "C" {
void paraminit( Parameter param )
{
  int max=1000;
  
  double are = param.Value(0), aim = param.Value(1);
  double tmp1, tmp2, tmp3, cre, cim, re, im;
  tmp1=(are*are-aim*aim)-3;
  tmp2=2*are*aim;
  tmp3=((tmp3 = 0.5*(sqrt(tmp1*tmp1+tmp2*tmp2)+tmp1)) >= 0.0 ) ? tmp3 : -tmp3;
  cim =sqrt(((tmp1 = tmp3 - tmp1) >= 0.0 ) ? tmp1 : -tmp1);
  cim =(tmp2 >= 0.0) ? cim : -cim;
  cre = sqrt(tmp3);

  complex<double> e1, e2;

  // first critical point
  re=(are + cre)/3.0; im=(aim + cim)/3.0;
  double re2=re*re, im2=im*im;
  int i, j;
  for (i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
    RE2IM2;
  }
  if (i < max) {
    e1 = 0.0;
  } else {
    e1 = 1.0/(complex<double>(re,im)*complex<double>(are,aim));
 }
  
  // second critical point
  re=(are - cre)/3.0; im=(aim - cim)/3.0;
  re2=re*re; im2=im*im;
  for (j=0; (j<max) && INCOND; j++ ) {
    FUNCTION;
    RE2IM2;
  }
  if (i < max) {
    e2 = 0.0;
  } else {
    e2 = 1.0/(complex<double>(re,im)*complex<double>(are,aim));
  }
  ceh1 = imag(e1); ceh2 = imag(e2);
  phase = (real(e1) > real(e2) ) ? real(e1) : real(e2);
  phase = max*10 - phase;
}

int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double tmp1,tmp2,tmp3;

  register double are=param.Value(0);
  register double aim=param.Value(1);
  register double re2=re*re;
  register double im2=im*im;

  complex<double> t(0, param.Value(3)), b;
  b = param.Value(2)*exp(2*M_PI*t);

  INIT

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max) {
    complex<double> z(re,im), a(are,aim);
    complex<double> w=1.0/(complex<double>(re,im)*complex<double>(are,aim));
    double eh = imag(w);
    return ((int) (real(w)+phase)) *8
      + ( (ceh1 > eh) && (ceh2 > eh) )*20
      + ( (ceh1 < eh) && (ceh2 < eh) )*40 + 1;
  } else if (im < 0) {
    return i*8 + 20;
  } else {
    return i*8;
  }
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double tmp1,tmp2,tmp3;
  register double are=param.Value(0);
  register double aim=param.Value(1);

  complex<double> t(0, param.Value(3)), b;
  b = param.Value(2)*exp(2*M_PI*t);
  double bre = real(b), bim = imag(b);

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
