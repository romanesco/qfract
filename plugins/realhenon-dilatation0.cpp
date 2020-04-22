/*
 * real Henon map
 * (x,y) -> (y, y^2+c-ax)
 * inverse: (x,y) -> ( (x^2+c-y)/a, x)
 */

#include "../plugin.h"
using namespace QFract;

#include <cmath>
#include <complex>
using namespace std;

#include <boost/numeric/ublas/matrix.hpp>
using namespace boost::numeric::ublas;

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
const char* CHILD = "realhenon-dilatation.so";
const char* COLORMAP = "default.map";

const double XL = -1;
const double YT = 1;
const double XR = 1.0;
const double YB = -1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=3;
double VALUE[N]={100, 1000, 1};
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "R", "scale for coloring", "fix pt # (0 or 1)" };

typedef matrix<double> dmatrix;
typedef complex<double> dcomplex;

const dcomplex I(0,1);

matrix<double> df(double x, double y, double a, double c)
{
  dmatrix A(2,2);
  A(0,0) = 0;   A(0,1) = 1;
  A(1,0) = -a;  A(1,1) = 2*y;
  return A;
}

// solution of y^2+c-ax=y ( (y,y) is a fixed point)
double fixpt(double a, double c, bool sign=1)
{
  double d=(a+1)*(a+1)-4*c;
  if (d < 0) {
    return 10000000;
  } else {
    double e=-1;
    if (sign) {
      e=1;
    }    
    return (a+1+e*sqrt(d))/2;
  }
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double y, tmp;
  register double c=z.x;
  register double a=z.y;
  register double absx, absy;
  y=fixpt(a,c, (bool) param.Value(2));
  if (y==10000000) {
    return -1;
  }
  
  dmatrix D(2,2);
  D = df(y,y,a,c);
  
  // 1/2 is omitted!
  dcomplex dz=D(0,0)+D(1,1)+I*(D(1,0)-D(0,1)),
    dzbar=D(0,0)-D(1,1)+I*(D(1,0)+D(0,1));
  double k=abs(dzbar/dz), K=(1+k)/(1-k);
  return ((int) (log(K)/max * param.Value(1)));
  //return ((int) (abs(log(norm(dzbar))-log(norm(dz)))/max * param.Value(3))) *2 + 1;
}

Point map(Point z, Point c0, Parameter param)
{
  return z;
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
