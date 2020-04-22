/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (dynamical space)
 */

#include <iostream>
#include <iomanip>
using namespace std;

#include <boost/thread.hpp>

#include "../plugin.h"
using namespace QFract;
#include <complex>
typedef std::complex<double> dcomplex;

#define FUNCTION x = conj(x*x)+c;
#define INCOND  ( norm(x)<10 )

const char* NAME = "Tricorn with parabolic 3-periodic point (Julia-Lavaurs set, parameter space)";
const char* CHILD = "tricornjulialavaursper3.so";
const char* COLORMAP = "per2.map";

const double XL = 0;
const double YT = 0.5;
const double XR = 1.0;
const double YB = -0.5;

const int MAXITER = 30000;
const int MAXORBIT = 10000;
const int N = 1;
double VALUE[N] = {0.01};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "eps" };

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

static boost::thread_specific_ptr<dcomplex> cparab, parabper3, a2, a3;
static boost::thread_specific_ptr<double> t0;

dcomplex ptoc(dcomplex p)
{
  return p-conj(p*p);
}

dcomplex parabfix(double t)
{
  return exp(2*M_PI*I*t)/2.0;
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

void initparams(double t)
{
  if (!t0.get()) {
    t0.reset(new double(10000));
    cparab.reset(new dcomplex(0));
    parabper3.reset(new dcomplex(0));
    a2.reset(new dcomplex(0));
    a3.reset(new dcomplex(0));
  }
  
  dcomplex c, q, z, w;
  double Gc, Gcold, remin, remax, remid, im;
  dcomplex p = parabfix(t);
  if (t==*t0) {
    return;
  }

  *t0=t;

  c = ptoc(p);
  // approximate value
  c = c0 + alpha*dcomplex(real(c),3*imag(c)/5);
  //q = delta * p;

  im = imag(c);
  remin = real(c)-0.001;
  remax = real(c)+0.001;
  
  const int max=1000000;
  
  for (int i=0; i<20; i++) {
    //cerr << remin << ", " << remax << endl;; 
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
  //cerr << setprecision(20);
  //cerr << remin << ", " << remax << endl;;

  *cparab = dcomplex(remin,im);
  c=*cparab;
  //cerr << "c: " << cparab << endl;

  z=0.;
  for (int i=0; i< (max/3)*3; i++) {
    z = conj(z*z)+c;
  }
  w = conj(z*z)+c;
  w = conj(w*w)+c;
  w = conj(w*w)+c;
  
  *parabper3=(z+w)/2.;
  //cerr << "per 3 pt: " << parabper3 << endl;

  dcomplex df2[3], ddf2[3], d3f2[3];
  z=*parabper3;
  for (int i=0; i<3; i++) {
    df2[i] = 4.*z*(z*z+conj(c));
    ddf2[i] = 4.*(3.*z*z+conj(c));
    d3f2[i] = 24.*z;
    
    z = z*z + conj(c);
    z = z*z + c;
  }

  z = df2[2]*df2[1]*ddf2[0]
    + df2[2]*ddf2[1]*df2[0]*df2[0]
    + ddf2[2]*df2[1]*df2[1]*df2[0]*df2[0];
  *a2 = z/2.;
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
  *a3 = z/6.;
  //cerr << "a2: " << a2 << endl;
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double t=z.y/6;
  initparams(t);
  dcomplex c=*cparab;
  dcomplex p=*parabper3;
  dcomplex x(c);
  double phase=z.x*10;
  double eps=param.Value(0);

  register int i;
  int j=0;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION;
    if (norm(*a2*(x-p)+eps)<eps*eps) {
      x=lavaurs(p, *a2, *a3, x, phase);
      j++;
    }
  }

  if (i>=max)
    return -1;
  else 
    return i*2+j;
}

Point map(Point z, Point c0, Parameter param)
{
  dcomplex x(z.x,z.y);
  dcomplex c=*cparab;
  dcomplex p=*parabper3;
  double phase=c0.x*10;
  double eps=param.Value(0);

  FUNCTION;
  if (norm(*a2*(x-p)+eps)<eps*eps) {
    x=lavaurs(p, *a2, *a3, x, phase);
  }
  
  return Point(real(x),imag(x));
}

Point init(Point c0, Parameter param)
{
  double t=c0.y/6;
  initparams(t);
  dcomplex c=*cparab;
  return Point(real(c),imag(c));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
