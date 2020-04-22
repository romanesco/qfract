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
const char* CHILD = "";
const char* COLORMAP = "per2.map";

const double XL = -1;
const double YT = 2;
const double XR = 2.0;
const double YB = -1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N=5;
double VALUE[N]={0.4, 1.0, 100, 0, 5000};
const Parameter PARAM( N, VALUE );
const char * PARAMDESC[N] = { "c", "a", "R", "1=ratio, 0=max", "scale for coloring" };

typedef matrix<double> dmatrix;
typedef complex<double> dcomplex;

matrix<double> df(double x, double y, double a, double c)
{
  dmatrix A(2,2);
  A(0,0) = 0;   A(0,1) = 1;
  A(1,0) = -a;  A(1,1) = 2*y;
  return A;
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double x=z.x, y=z.y, tmp;
  register double c=param.Value(0);
  register double a=param.Value(1);
  register double R=param.Value(2);
  register double absx, absy;
  dmatrix D(2,2);
  D = identity_matrix<double> (2);
  
  ABS;

  register int i, j;
  for ( i=0; (i<max) && INCOND; i++ ) {
    D = prod(df(x,y,a,c),D);
    FUNCTION;
    ABS;
  }
  
  x = z.x; y=z.y; ABS;
  
  /*
  for ( j=0; (j<max) && INCOND_INV; j++ ) {
    INVERSE;
    ABS;
  }
  */

  if (i< max) {
    return i*2;
  } else {
    double tr = D(0,0)+D(1,1), det = D(0,0)*D(1,1)-D(1,0)*D(0,1);
    double l1 = abs(tr + sqrt((dcomplex) tr*tr-4*det)),
      l2 = abs(tr - sqrt((dcomplex) tr*tr-4*det));
    if (param.Value(3)) {
      return ((int) (abs(log(l1)-log(l2))/max * param.Value(4))) *2 + 1;
    } else {
      if (l1<l2) l1=l2;
      return ((int) (abs(log(l1))/max * param.Value(4))) *2 + 1;
    }
  }
  /*
  if ((i>=max) && (j>=max))
    return -1;
  else if (i>=max)
    return (j%15)+1;
  else if (j>=max)
    return ((i%15)+1)*16;
  else 
    return ((i%15)+1)*16+(j%15)+1;
  */
}

Point map(Point z, Point c0, Parameter param)
{
  register double x=z.x, y=z.y, tmp;
  register double c=param.Value(0);
  register double a=param.Value(1);

  //if (param.Value(3)) {
    FUNCTION;
  //} else {
    //INVERSE;
  //}

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
