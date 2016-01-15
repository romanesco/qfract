/*
 * Real cublc family
 * z -> z^3 - 3 a^2 z + b (dynamical space with a and b real)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION tmp1 = re2-im2 - 3*A;	\
                 tmp2 = 2*re*im; \
		 tmp3 = tmp1*re - tmp2*im + bre; \
		 im = tmp1*im + tmp2*re + bim; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Real cubic polynomial (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 5;
double VALUE[N] = {0.0, 0.0, 1.0, -1.0, 0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "A", "B", "sign of leading term", "sign of A", "holo=0, anti-holo=1" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double re=z.x;
  double im=z.y;
  double e1 = (param.Value(2)>0) ? 1 : -1;
  double e2 = (param.Value(3)>0) ? 1 : -1;
  double a=param.Value(0), A=e1*a*a;
  double b=param.Value(1);
  
  double are, aim, bre, bim;
  if (e1 == 1) { 
    are=a; aim=0;
  } else {
    are=0; aim=a;
  }
  
  if (e2 == 1) {
    bre = b; bim=0;
  } else {
    bre=0; bim=b;
  }

  double re2=re*re;
  double im2=im*im;
  double tmp1,tmp2,tmp3;

  int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else 
    return i*2;
}

Point map(Point z, Point c, Parameter param)
{
  double re=z.x;
  double im=z.y;
  double e1 = (param.Value(2)>0) ? 1 : -1;
  double e2 = (param.Value(3)>0) ? 1 : -1;
  double a=param.Value(0), A=e1*a*a;
  double b=param.Value(1);
  double are, aim, bre, bim;
  if (e1 == 1) { 
    are=a; aim=0;
  } else {
    are=0; aim=a;
  }
  
  if (e2 == 1) {
    bre = b; bim=0;
  } else {
    bre=0; bim=b;
  }

  double re2=re*re;
  double im2=im*im;
  double tmp1,tmp2,tmp3;

  FUNCTION;

  if (param.Value(4)) {
    re = -re;
  }

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
