/*
 * real Henon map
 * (x,y) -> (y, y^2+c-ax)
 * inverse: (x,y) -> ( (x^2+c-y)/a, x)
 */

#include "../plugin.h"
using namespace QFract;

#include <cmath>
using namespace std;

#define ABS absx = abs(x); absy = abs(y);

#define FUNCTION tmp = y; \
  y = y*y + c - a*x; \
  x = tmp;

#define INVERSE tmp = x; \
  x = (x*x + c - y)/a; \
  y = tmp;

#define INCOND  ( (absx > absy) || (absy < R) )
#define INCOND_INV  ( (absx < absy) || (absx < R) )

const char* NAME = "Real Henon Map";
const char* CHILD = "";
const char* COLORMAP = "dim2.map";

const double XL = -5.0;
const double YT = 5.0;
const double XR = 5.0;
const double YB = -5.0;

const int MAXITER = 10;
const int MAXORBIT = 10;
const int N=4;
double VALUE[N]={-3.0, 0.3, 100, 1};
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "c", "a", "R", "1=fwd, 0=bkwd orbit" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double x=z.x, y=z.y, tmp;
  register double c=param.Value(0);
  register double a=param.Value(1);
  register double R=param.Value(2);
  register double absx, absy;

  ABS;

  register int i, j;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
    ABS;
  }

  x = z.x; y=z.y; ABS;
  
  for ( j=0; (j<max) && INCOND_INV; j++ ) {
    INVERSE;
    ABS;
  }

  if ((i>=max) && (j>=max))
    return -1;
  else if (i>=max)
    return (j%15)+1;
  else if (j>=max)
    return ((i%15)+1)*16;
  else 
    return ((i%15)+1)*16+(j%15)+1;
}

Point map(Point z, Point c0, Parameter param)
{
  register double x=z.x, y=z.y, tmp;
  register double c=param.Value(0);
  register double a=param.Value(1);

  if (param.Value(3)) {
    FUNCTION;
  } else {
    INVERSE;
  }

  return Point(x,y);
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
