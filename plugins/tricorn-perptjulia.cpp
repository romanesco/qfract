/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (dynamical space)
 */

#include <cmath>
using namespace std;

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = -2*re*im + aim; \
                 re = re2-im2 + are; \
		 RE2IM2

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Tricorn (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 6;
const int MAXORBIT = 6;
const int N = 2;
double VALUE[N] = {-1.0, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double are=param.Value(0);
  register double aim=param.Value(1);
  register double re2=re*re;
  register double im2=im*im;

  register int i;
  double rmax=0.1, r;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
    r = (z.x-re)*(z.x-re)+(z.y-im)*(z.y-im);
    rmax = (r>rmax) ? r : rmax;
  }

  if (i<max)
    return -1;

  //cout << r << "\t" << rmax << "\t";
  r = -log(r/rmax)*100;
  //cout << r << endl;
  return (int) r;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double are=param.Value(0);
  register double aim=param.Value(1);
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
