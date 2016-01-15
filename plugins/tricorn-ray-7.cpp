/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (parameter space with external rays)
 */

#include "../plugin.h"
using namespace QFract;
#include <cmath>
#include <complex>
using namespace std;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = -2*re*im + aim; \
                 re = re2-im2 + are;
/*
#define FUNCTION im = 2*re*im - aim; \
                 re = re2-im2 + are; \
		 RE2IM2 \
		 im = 2*re*im + aim; \
                 re = re2-im2 + are;
*/

#define INCOND  ( (re2+im2)<10000 )

const char* NAME = "Tricorn (parameter space)";
const char* CHILD = "tricorn-ray-7julia.so";

const double XL = -2.5;
const double YT = 2.0;
const double XR = 1.5;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=0.0;
  register double im=0.0;
  register double are=z.x;
  register double aim=z.y;
  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else {
    // 3/7 -> 1/7 -> 5/7 -> 4/7 -> 6/7 -> 2/7 -> 3/7
    const int shift[6]={3,1,5,4,6,2};
    double t = arg(complex<double>(-re,-im))/(2*M_PI)+0.5;
    int a=t*7;
    return i+21*a;
  }
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double are=c.x;
  register double aim=c.y;
  register double re2=re*re;
  register double im2=im*im;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return Point(0,0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
