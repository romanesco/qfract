/*
 * the unstable slice of K^+ for the complex Henon map
 * H: (x,y) -> (by+x^2+c,x)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
#include <iostream>
using namespace std;

#define INIT  for ( i=DEGREE; i>0; i--) \
				tmp1 = (tmp1+a[i])*x; \
  			  x=tmp1+p; \
  			  tmp1=0; \
  			  for ( i=DEGREE; i>0; i--)\
				tmp1 = (tmp1+a[i])*y; \
			  y=tmp1+p;

#define RE2IM2   absx=real(x)*real(x)+imag(x)*imag(x); \
                 absy=real(y)*real(y)+imag(y)*imag(y); 

#define FUNCTION tmp1 = b*y + x*x + c; \
				 y = x; x = tmp1;

#define INVERSE tmp1 = (x-y*y-c)/b; \
                x = y; y = tmp1;

#define INCOND  ( (absx-absy)<10000 )

#define INVERSEINCOND ( (absy-absx)<10000 )

const char* NAME = "Henon unstable slice of K+";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -0.1;
const double YT = 0.1;
const double XR = 0.1;
const double YB = -0.1;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 6;
double VALUE[N] = {-1.4, 0.0, 0.3, 0.0, 1, 1};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)", "Re(b)", "Im(b)",
			     "Option (1-4)", "1: fwd, -1:bkwd" };

const int DEGREE = 50;
static complex<long double> a[DEGREE];
static complex<long double> p, lp1, p2, b, c;

extern "C" {
void paraminit( Parameter param )
{
  // parameters
  c=complex<long double>(param.Value(0), param.Value(1));
  b=complex<long double>(param.Value(2), param.Value(3));
  // option to choose saddle and unstable eigenvalue
  // (1-p,lp1,  2-p,lp2,  3-q,lq1,  4-q,lp2)
  int opt = (int) param.Value(4);

  complex<long double> q, lp2, lq1, lq2, l, tmp1, tmp2;

  tmp1=((long double)1-b)/(long double)2;
  p=tmp1+sqrt(tmp1*tmp1-c); q=tmp1-sqrt(tmp1*tmp1-c); // fixed points 
  cout <<"fixed points:" << endl << p << endl << q << endl;

  tmp1=sqrt(p*p+b);
  lp1=p+tmp1, lp2=p-tmp1; // eigenvalues of DH(p)
  cout <<"eigenvalues for p:" << endl <<  lp1 << endl << lp2 << endl;

  tmp1=sqrt(q*q+b);
  lq1=q+tmp1, lq2=q-tmp1; // eigenvalues of DH(p)
  cout <<"eigenvalues for q:" << endl << lq1 << endl << lq2 << endl;

  // choose saddle
  if (opt>2) p=q;
  p2=p+p;
  // choose unstable eigenvalue
  if (opt==2) lp1=lp2;
  if (opt==3) lp1=lq1;
  if (opt==4) lp1=lq2;

  cout << "fixed point" << p << endl << "eigenvalue" << lp1 << endl;
  if (param.Value(5)>0) { cout << "forward" << endl; }
  else {cout << "backward" << endl; }

  // compute coefficients of unstable manifold
  a[1]=lp1; l=lp1;
  for ( register int i=2; i<DEGREE; i++ ) {
	l*=lp1; tmp1=0;
	for ( register int n=1; n<i; n++ )
	  tmp1 += a[n]*a[i-n];
	tmp2 = l - b/l - p2;
	a[i] = tmp1/tmp2;
	//cout << i << ": " << a[i] << endl;
  }
}

int iter(Point z, Parameter param, int max)
{
  complex<long double> x(z.x, z.y);
  complex<long double> y = x / lp1;
  complex<long double> tmp1 = 0;
  long double absx=0, absy=0;
   
  register int i;

  INIT

  if (param.Value(5)>0) {
	// forward iteration
	for ( i=0; (i<max) && INCOND; i++ ) {
	  FUNCTION
	  RE2IM2
	}
  } else {
	// backward iteration
	for ( i=0; (i<max) && INVERSEINCOND; i++ ) {
	  INVERSE
	  RE2IM2
	}
  }
	  
  if (i<max)
    if (imag(x)>0 )
      return i*2;
    else
      return i*2+21;
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  complex<long double> x(z.x, z.y);
  x *= lp1;
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
