/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (dynamical space)
 */

#include <iostream>
#include <iomanip>
using namespace std;
#include "../plugin.h"
using namespace QFract;
#include <complex>
typedef std::complex<double> dcomplex;

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
typedef boost::numeric::ublas::vector<double> dvector;
typedef boost::numeric::ublas::matrix<double> dmatrix;
#include "invert_matrix.hpp"


#define FUNCTION x = conj(x*x)+c;
/*
#define FUNCTION im = 2*re*im - aim; \
                 re = re2-im2 + are; \
		 RE2IM2 \
		 im = 2*re*im + aim; \
                 re = re2-im2 + are;
*/

#define INCOND  ( norm(x)<10 )

const char* NAME = "Tricorn with parabolic 3-periodic point (Julia-Lavaurs set)";
const char* CHILD = "";
//const char* COLORMAP = "default2.map";
const char* COLORMAP = "per2-2.map";

const double XL = -104.0;
const double YT = 2.0;
const double XR = -100.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 3000;
const int MAXORBIT = 10000;
const int N = 4;
double VALUE[N] = {0.0, 0.0, 0.01, 0};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "phase", "t","eps", "1=apply Lavaurs map" };

const dcomplex I(0,1);
// c0: airplane,
// when c=c0+delta*a, z=alpha*w,
// then Q^3(z) is approximately w^2+a and
// when c=c0+delta*(x+3/5*i*y), z=alpha*w,
// then f^3(z) is approximately conjugate(w)^2 + x+i*y.
const double sq69=sqrt(69.), beta = 100+12*sq69,
  c0 = -pow(beta,1/3.)/6 - 2/(3*pow(beta,1/3.)) - 2/3.,
  alphainv = 52/3. + (49-sq69*5)/3 *pow(beta,1/3.) + (95-11*sq69)/6*pow(beta,2/3.),
  alpha=1/alphainv,
  deltainv = -4 + (-3+sq69/3)*pow(beta,1/3.) + (-3/2.+sq69/6)*pow(beta,2/3.),
  delta = 1/deltainv;

static dcomplex cparab, parabper3, a2, a3;

dcomplex ptoc(dcomplex p){

  return p-conj(p*p);
}

dcomplex parabfix(double t)
{
  return exp(2*M_PI*I*t)/2.0;
}

dcomplex fatou(const dcomplex &p, const dcomplex &a2, const dcomplex &a3,
	       const dcomplex &z)
{
  // first approximation w = -1/(a2*(z-p)
  dcomplex w=-1.0/(a2*(z-p));
  // second approximation 
  dcomplex c = (1.-a3/(a2*a2));
  return w - c*log(w);
}

dcomplex fatouinv(const dcomplex &p, const dcomplex &a2, const dcomplex &a3,
	       const dcomplex &z)
{
  dcomplex w=z;
  dcomplex c = (1.-a3/(a2*a2));
  // second approximation (notice about the branch of log!)
  w = w + c* (log(-w)+M_PI);
  // first approximation
  w= -1.0/(a2*w)+p;
  return w;
}

dcomplex lavaurs(const dcomplex &p, const dcomplex &a2, const dcomplex &a3,
		 const dcomplex &z, const double &phase)
{
  // first approximation w = -1/(a2*(z-p)
  dcomplex w=-1.0/(a2*(z-p));
  // second approximation 
  dcomplex c = (1.-a3/(a2*a2));
  w = w - c*log(w);
  
  // translation
  w += -((int) real(w))*2 + phase;
  
  // second approximation (notice about the branch of log!)
  w = w + c* (log(-w)+M_PI);
  // first approximation
  w= -1.0/(a2*w)+p;

  return w;
}


extern "C" {

// Given imaginary part of c, solve for the real part
void paraminit(Parameter param)
{
  dcomplex c, q, z, w;
  double Gc, Gcold, remin, remax, remid, im;
  double t = param.Value(1)/6;
  dcomplex p = parabfix(t);
  c = ptoc(p);
  // approximate value
  c = c0 + alpha*dcomplex(real(c),3*imag(c)/5);
  //q = delta * p;

  im = imag(c);
  remin = real(c)-0.001;
  remax = real(c)+0.001;
  
  const int max=1000000;
  
  for (int i=0; i<20; i++) {
    cerr << remin << ", " << remax << endl;; 
    remid = (remin+remax)/2;
    c = dcomplex(remid, im);
    z = c;
    double n=norm(c);
    int j;
    for (j=0; ((j<max) && (n<4)); j++) {
      z = conj(z*z)+c;
      n=norm(z);
    }
    if (j<max) {
      remax=remid;
    } else {
      double d=0;
      for (int i=0; i<1000; i++) {
	w=conj(z*z)+c;
	w=conj(w*w)+c;
	w=conj(w*w)+c;	
	d=(d>norm(w-z)) ? d : norm(w-z);
	z=w;
      }
      
      if ( d < 1./max ) {
	remin=remid;
      } else {
	remax=remid;
      }
    }
  }
  cerr << setprecision(20);
  cerr << remin << ", " << remax << endl;;
  
  cparab = dcomplex(remin,im);
  c=cparab;
  cerr << "c: " << cparab << endl;

  z=0.;
  for (int i=0; i<(max/3)*3; i++) {
    z = conj(z*z)+c;
  }
  w = conj(z*z)+c;
  w = conj(w*w)+c;
  w = conj(w*w)+c;
  
  parabper3=(z+w)/2.;
  cerr << "per 3 pt: " << parabper3 << endl;

  dcomplex df2[3], ddf2[3], d3f2[3];
  z=parabper3;
  for (int i=0; i<3; i++) {
    df2[i] = 4.*z*(z*z+conj(c));
    ddf2[i] = 4.*(3.*z*z+conj(c));
    d3f2[i] = 24.*z;
    z = z*z + conj(c);
    z = z*z + c;
  }
  a2 = df2[2]*df2[1]*ddf2[0]
    + df2[2]*ddf2[1]*df2[0]*df2[0]
    + ddf2[2]*df2[1]*df2[1]*df2[0]*df2[0];
  a2 /= 2.;
  cerr << "a2: " << a2 << endl;
  // (fgh)''' =   f''' (g')^3 (h')^3
  //          + 3 f'' g' g'' (h')^3
  //          + 3 f'' (g')^2 h' h''
  //          +   f' g''' (h')^3
  //          + 3 f' g'' h' h''
  //          +   f' g' h'''
  z = d3f2[2]*df2[1]*df2[1]*df2[1]*df2[0]*df2[0]*df2[0]
    + 3.*ddf2[2]*df2[1]*ddf2[1]*df2[0]*df2[0]*df2[0]
    + 3.*ddf2[2]*df2[1]*df2[1]*df2[0]*ddf2[0]
    + df2[2]*ddf2[1]*df2[0]*df2[0]*df2[0]
    + 3.*df2[2]*ddf2[1]*df2[0]*ddf2[0]
    + df2[2]*df2[1]*df2[0]*df2[0]*df2[0];
  a3 = z/6.;
  cerr << "a3: " << a3 << endl;
}
  
int iter(Point z, Parameter param, int max)
{
  dcomplex x(z.x,z.y);
  dcomplex c=cparab;
  double phase=param.Value(0)*10;
  double eps=param.Value(2);

  x = fatouinv(parabper3, a2, a3, x);
  
  register int i;
  int j=0;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
    if ((param.Value(3)) && (norm(a2*(x-parabper3)+eps)<eps*eps)) {
      x=lavaurs(parabper3, a2, a3, x, phase);
      j++;
    }
  }
  
  if (i>=max)
    return -1;
  else if (imag(x) >= 0) 
    return i*8;
  else
    return i*8+21;
}

Point map(Point z, Point c0, Parameter param)
{
  dcomplex x(z.x,z.y);
  dcomplex c=cparab;
  double phase=param.Value(0)*10;
  double eps=param.Value(2);
  
  for (int i=0; i<3; i++) {
    FUNCTION;
    if ((param.Value(3)) && (norm(a2*(x-parabper3)+eps)<eps*eps)) {
      x=lavaurs(parabper3, a2, a3, x, phase);
    }
  }
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
