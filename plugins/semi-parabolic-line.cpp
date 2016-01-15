/*
 * semi-parabolic implosion F(x,y)=((1+a)x-ay+x^2+e^2/(1-a),x+e^2/(1-a))
 * tangent line at the origin (parabolic direction for e=0)
 * (parameter space, do nothing)
 */

#include "../plugin.h"
#include <complex>
using namespace std;
using namespace QFract;

//#define FUNCTION 

//#define INCOND ( ( norm(x)<r ) && (norm(x)>eps) )

const char* NAME = "Semi-parabolic implosion (parameter space)";
const char* CHILD = "semi-parabolic-linejulia.so";
const char* COLORMAP = "default.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {0.3, 0.0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)" };
//const complex<double> I(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  // do nothing
  return -1;
}

Point map(Point z, Point c, Parameter param)
{
  // do nothing
  return z;
}

Point init(Point c, Parameter param)
{
  // do nothing
  return c;
}  

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
