/*
 * tricorn
 * z -> (z^2 + conj(c))^2 + c (dynamical space with external rays)
 */

#include "../plugin.h"
using namespace QFract;

#include <complex>
typedef std::complex<double> dcomplex;

#include <boost/thread.hpp>

#define FUNCTION x = conj(x)*conj(x)+c;
#define INCOND  ( norm(x)<10 )

const char* NAME = "Tricorn (close to Lavaurs map, Julia set)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -0.2;
const double YT = 0.2;
const double XR = 0.2;
const double YB = -0.2;

const int MAXITER = 1000;
const int MAXORBIT = 1000;
const int N = 3;
double VALUE[N] = {500, 0.0, 0.53};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(c)", "Im(c)",
			     "phase offset" };
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

//static boost::thread_specific_ptr<dcomplex> cparab, parabper3, a2, a3;
//static boost::thread_specific_ptr<double> t0;
static dcomplex cparab, parabper3, a2, a3;
static double t0;

dcomplex ptoc(dcomplex p)
{
  return p-conj(p*p);
}

dcomplex parabfix(double t)
{
  return exp(2*M_PI*I*t)/2.0;
}

extern "C" {
void paraminit(Parameter param)
{
  /*
  if (!t0.get()) {
    t0.reset(new double(10000));
    cparab.reset(new dcomplex(0));
    parabper3.reset(new dcomplex(0));
    a2.reset(new dcomplex(0));
    a3.reset(new dcomplex(0));
  }
  */
  
  double t=param.Value(1)/240;
  dcomplex c, q, z, w;
  double Gc, Gcold, remin, remax, remid, im;
  dcomplex p = parabfix(t);
  if (t==t0) {
    return;
  }

  t0=t;

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

  cparab = dcomplex(remin,im);
  c=cparab;
  //cerr << "c: " << cparab << endl;

  z=0.;
  for (int i=0; i< (max/3)*3; i++) {
    z = conj(z*z)+c;
  }
  w = conj(z*z)+c;
  w = conj(w*w)+c;
  w = conj(w*w)+c;
  
  parabper3=(z+w)/2.;
  //cerr << "per 3 pt: " << parabper3 << endl;

  dcomplex df2[3], ddf2[3], d3f2[3];
  z=parabper3;
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
  a2 = z/2.;
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
  //cerr << "a2: " << a2 << endl;
}

int iter(Point z, Parameter param, int max)
{
  //double t=param.Value(1)/240;
  dcomplex p=parabper3;
  dcomplex c0=cparab;
  double phase=param.Value(0)-param.Value(2)*param.Value(1)*param.Value(1);
  dcomplex c=c0 + 1/(64*phase);
  dcomplex x(z.x,z.y);
  
  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
  }

  if (i>=max)
    return -1;
  else if (imag(x) >= 0) 
    return i*8;
  else
    return i*8+21;
}

Point map(Point z, Point cc, Parameter param)
{
  dcomplex x(z.x,z.y);
  dcomplex p=parabper3;
  dcomplex c0=cparab;
  double phase=param.Value(0)-param.Value(2)*param.Value(1)*param.Value(1);
  dcomplex c=c0 + 1/(64*phase);
  c = c + 1/(64*phase);

  FUNCTION;

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
