/*
 * Real cubic family
 * z -> z^3 - 3 A z + sqrt(B) in (a,b)-plane (A=e1*a^2, B=e2*b^2)
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

const char* NAME = "Real cubic polynomial (parameter space)";
const char* CHILD = "cubicreal2rayjulia.so";
//const char* COLORMAP = "per2.map";
const char* COLORMAP = "default.map";

const double XL = 0.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = 0.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {1.0, -1.0};
const Parameter PARAM(N,VALUE);
const char* PARAMDESC[N] = { "sign of A", "sign of B" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double e1 = (param.Value(0)>0) ? 1 : -1;
  double e2 = (param.Value(1)>0) ? 1 : -1;
  double a=z.x, A=e1*a*a;
  double b=z.y;
  
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

  // first critical point
  double re=are;
  double im=aim;

  double re2=re*re;
  double im2=im*im;
  double tmp1, tmp2, tmp3;

  int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (e1*e2 > 0) {
    // second critical point
    re=-are; im=-aim;
    RE2IM2;
    
    int j;
    for ( j=0; (j<max) && INCOND; j++ ) {
      FUNCTION
	RE2IM2
	}
    if ((i>=max) && (j>=max))
      return -1;
    else 
      return ((i<j) ? j*2: i*2+1);
  } else {
    if (i>=max) 
      return -1;
    else
      if (im > 0) {
	return i*2+1;
      } else {
	return i*2+21;
      }
  }
}

Point map(Point z, Point c, Parameter param)
{
  double re=z.x;
  double im=z.y;
  double e1 = (param.Value(0)>0) ? 1 : -1;
  double e2 = (param.Value(1)>0) ? 1 : -1;
  double a=c.x, A=e1*a*a;
  double b=c.y;

  double bre, bim;
  if (e2 == 1) {
    bre = b; bim=0;
  } else {
    bre=0; bim=b;
  }

  double re2=re*re;
  double im2=im*im;
  double tmp1, tmp2, tmp3;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
  if (param.Value(0) >= 0)
    return Point(c.x,0);
  else
    return Point(0,c.x);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
