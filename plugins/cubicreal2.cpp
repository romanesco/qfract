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
const char* CHILD = "cubicreal2julia.so";
//const char* COLORMAP = "per2.map";
const char* COLORMAP = "default.map";

const double XL = 0.0;
const double YT = 1.2;
const double XR = 1.2;
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


//
// in the 2nd quadrant (A<0, B>0).
//

// attracting fp of adjacent type
bool isAttrFP_A_Q2(Point z)
{
  double a=z.x, b=z.y, A=-a*a, B=b*b, t=3*A+1;
  return ( 27*B < 4*t*t*t );
}

// attracting fp of disjoint type
bool isAttrFP_D_Q2(Point z)
{
  double a=z.x, b=z.y, A=-a*a, B=b*b, t=3*A+1;
  return ( (27*B > 4*t*t*t) && ( 27*B < (3*A+2)*(6*A+1)*(6*A+1)) );
}

// attracting per2 pt of disjoint type
bool isAttrPer2_Q2(Point z)
{
  double A=-z.x*z.x, B=z.y*z.y, c0, c1, c2, t;
  t = 20+(-51+(-36+108*A)*A)*A;
  c0 = 4*(3*A+2)*(9*A*A-5)*t*t;
  c1 = 51705 + (-145800 + (-126117 + (449064 + (-209952 +  944784*A*A)*A)*A)*A)*A ;
  c2 = -55404 + (91854 + 236196*A*A)*A;
  t = c0 + (-c1 + (c2 - 19683*B)*B)*B;
  return (t > 0);
}

//
// in the 4th quadrant (A>0, B<0).
//

// Per_1(1): -4*(3*a^2+2)^2*(-1+3*a^2) = 27*b^2
bool isAttrFP_Q4(Point z)
{
  double a=z.x, b=z.y, t=3*a*a;
  return ( (4*(t+2)*(t+2)*(t-1)+27*b*b) < 0 );
  /*
  double a=z.x, b=z.y, t=a*a + 1.0/3;
  double alpha = -b/2 + sqrt(b*b/4 + t*t*t),
         beta  = -b/2 - sqrt(b*b/4 + t*t*t);
  double x = pow(alpha,1.0/3) - pow(-beta,1.0/3);
  //std::cout << "alpha: " << alpha << ", beta: " << beta << "x : " << x << "result: " << -x*x*x-3*a*a*x+b-x << std::endl;
  return ( (x*x) < (1.0/3-a*a) );
  */
}

// Per_1(2): -4*(3*a^2-2)^3 = 27*b^2
bool isAttrPer2_1_Q4(Point z)
{
  //return false;
  double a=z.x, b=z.y, t=3*a*a -2;
  return ( (t*t*t*4+27*b*b) < 0);
}

// multone_2:
// (4*(3*A+2))*(9*A^2-5)*(20-51*A-36*A^2+108*A^3)^2 +
//  (449064*A^3-209952*A^4-145800*A+51705-126117*A^2+944784*A^6)*B +
//   (91854*A+236196*A^3-55404)*B^2 + 
//    19683*B^3
// = 0
bool isAttrPer2_2_Q4(Point z)
{
  double A=z.x*z.x, B=-z.y*z.y, c0, c1, c2, t;
  t = 20+(-51+(-36+108*A)*A)*A;
  c0 = 4*(3*A+2)*(9*A*A-5)*t*t;
  c1 = 51705 + (-145800 + (-126117 + (449064 + (-209952 +  944784*A*A)*A)*A)*A)*A ;
  c2 = -55404 + (91854 + 236196*A*A)*A;
  t = c0 + (-c1 + (c2 - 19683*B)*B)*B;
  return (t < 0);
}

int iter(Point z, Parameter param, int max)
{
  double e1 = (param.Value(0)>0) ? 1 : -1;
  double e2 = (param.Value(1)>0) ? 1 : -1;
  if ( (e1<0) && (e2>0) ) {
    // 2nd quadrant
    if ( isAttrFP_A_Q2(z) ) { return 64; }
    if ( isAttrFP_D_Q2(z) ) { return 128; }
    if ( isAttrPer2_Q2(z) ) { return 192; }
  }

  if ( (e1>0) && (e2<0) ) {
    // 4th quadrant
    if ( isAttrFP_Q4(z) ) { return 64; }
    if ( isAttrPer2_1_Q4(z) ) { return 128; }
    if ( isAttrPer2_2_Q4(z) ) { return 192; }
  }


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
      return i*2+1;
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
