/*
 * Per_1(1) in the family of cubic polynomials
 * z -> z^3 - a z^2 + z
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

// critical points are (a +- c)/3 where c=sqrt(a^2-3)
#define CRITINIT   tmp1=(are*are-aim*aim)-3; \
                   tmp2=2*are*aim; \
                   tmp3=((tmp3 = 0.5*(sqrt(tmp1*tmp1+tmp2*tmp2)+tmp1)) \
			 >= 0.0 ) ? tmp3 : -tmp3; \
                   cim =sqrt(((tmp1 = tmp3 - tmp1) >= 0.0 ) ? tmp1 : -tmp1); \
                   cim =(tmp2 >= 0.0) ? cim : -cim; \
                   cre = sqrt(tmp3);

#define CRIT1      re=(are + cre)/3.0; im=(aim + cim)/3.0;

#define CRIT2      re=(are - cre)/3.0; im=(aim - cim)/3.0;

const char* NAME = "Cubic (Per_1(1))";
const char* CHILD = "cubicper1_1julia.so";
const char* COLORMAP = "per2.map";

const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {1.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "|b|", "arg(b)/(2*pi)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double are=z.x;
  register double aim=z.y;

  complex<double> a(are,aim);
  if (norm(a*a-0.5) < 0.25) {
    return -1;
  }

  complex<double> t(0, param.Value(1));

  INIT

  register double tmp1, tmp2, tmp3;
  register double cre, cim, re, im;

  CRITINIT
  //first critical point
  CRIT1

  register double re2=re*re;
  register double im2=im*im;

  register int i;
  int upper1=1, upper2=1;
  double imag1, imag2;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if ( i >= max) {
    imag1 = imag(-1.0/(complex<double>(re,im)*complex<double>(are,aim)));
  } else {
    if ( im < 0 ) upper1=0;
  }
  
  //second critical point
  CRIT2
  RE2IM2
  register int j;
  for ( j=0; (j<max) && INCOND; j++ ) {
    FUNCTION;
    RE2IM2;
  }
  
  if ( j >= max) {
    imag2 = imag(-1.0/(complex<double>(re,im)*complex<double>(are,aim)));
  } else {
    if ( im < 0 ) upper2=0;
  }
    
  if ((i>=max) && (j>=max))
    return i*8 + (imag1>imag2)*40+1;
  else 
    return ((i<j) ? i*8+upper1*20: j*8+upper2*20);
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double tmp1, tmp2, tmp3;
  register double are=c.x;
  register double aim=c.y;

  complex<double> t(0, param.Value(1)), b;
  b = param.Value(0)*exp(2*M_PI*t);
  double bre = real(b), bim = imag(b);

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
  register double are=c.x;
  register double aim=c.y;
  register double tmp1,tmp2,tmp3,cre,cim,re,im;

  complex<double> t(0, param.Value(1)), b;
  b = param.Value(0)*exp(2*M_PI*t);
  double bre = real(b), bim = imag(b);

  CRITINIT
  CRIT1
  return Point(re,im);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
