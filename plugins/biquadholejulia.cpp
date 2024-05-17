/*
 * Biquadratic family
 * z -> (z^2 + a)^2 + b (dynamical space)
 * parameter space spanned by two vectors
 */

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = 2*re*im + aim; \
                 re = re2-im2 + are; \
		 RE2IM2 \
		 im = 2*re*im + bim; \
                 re = re2-im2 + bre;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Biquadratic family (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 20;
/*
const double p = 0.1649077112, q = 0.687608498;
double VALUE[N] = { 0, 0, -0.1, -0.1,
  q, -p, -p, q,
  -p, -q, q, p,
  p, q, q, p,
  q, -p, p, -q };
*/
// simplified parameter
const double s = 1/sqrt(2);
double VALUE[N] = { 0, 0, 0.1, 0.1,
  s, 0, 0, 0,
  0, 0, s, 0,
  1, 0, -1, 0,
  0, 1,  0, -1 };
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "v3", "v4", "v1", "v2",
  "x1", "x2", "x3", "x4",
  "y1", "y2", "y3", "y4",
  "z1", "z2", "z3", "z4",
  "w1", "w2", "w3", "w4"};

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;

  register double v3 = param.Value(0);
  register double v4 = param.Value(1);
  register double v1 = param.Value(2);
  register double v2 = param.Value(3);

  register double x1 = param.Value(4);
  register double x2 = param.Value(5);
  register double x3 = param.Value(6);
  register double x4 = param.Value(7);
  register double y1 = param.Value(8);
  register double y2 = param.Value(9);
  register double y3 = param.Value(10);
  register double y4 = param.Value(11);
  register double z1 = param.Value(12);
  register double z2 = param.Value(13);
  register double z3 = param.Value(14);
  register double z4 = param.Value(15);
  register double w1 = param.Value(16);
  register double w2 = param.Value(17);
  register double w3 = param.Value(18);
  register double w4 = param.Value(19);
    
  register double are=v1*x1+v2*y1+v3*z1+v4*w1;
  register double aim=v1*x2+v2*y2+v3*z2+v4*w2;
  register double bre=v1*x3+v2*y3+v3*z3+v4*w3;
  register double bim=v1*x4+v2*y4+v3*z4+v4*w4;

  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i>=max)
    return -1;
  else 
    return i*5;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;

  register double v3 = param.Value(0);
  register double v4 = param.Value(1);
  register double v1 = param.Value(2);
  register double v2 = param.Value(3);

  register double x1 = param.Value(4);
  register double x2 = param.Value(5);
  register double x3 = param.Value(6);
  register double x4 = param.Value(7);
  register double y1 = param.Value(8);
  register double y2 = param.Value(9);
  register double y3 = param.Value(10);
  register double y4 = param.Value(11);
  register double z1 = param.Value(12);
  register double z2 = param.Value(13);
  register double z3 = param.Value(14);
  register double z4 = param.Value(15);
  register double w1 = param.Value(16);
  register double w2 = param.Value(17);
  register double w3 = param.Value(18);
  register double w4 = param.Value(19);
    
  register double are=v1*x1+v2*y1+v3*z1+v4*w1;
  register double aim=v1*x2+v2*y2+v3*z2+v4*w2;
  register double bre=v1*x3+v2*y3+v3*z3+v4*w3;
  register double bim=v1*x4+v2*y4+v3*z4+v4*w4;

  register double re2=re*re;
  register double im2=im*im;

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
