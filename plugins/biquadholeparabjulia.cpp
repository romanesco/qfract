/*
 * Parabolic maps in the hole of the bifurcation measure (Julia set)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Biquadratic parabolic maps in the hole (Julia set)";
const char* CHILD = "";
//const char* COLORMAP = "per2-2.map";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={1, 0, 0.0, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "parameter (1 or 2)", "pre-Fatou=1", "Re(aσ)", "Im(aσ)" };

const complex<double> I(0,1);

const complex<double> x0[2] = {
  complex<double>(.47861646654113824793, -0.14465917860846401205),
  complex<double>(-0.35519837246850654503, 0.35190071923160954467) };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  int k = param.Value(0);
  if (k<1) k=1;
  if (k>2) k=2;

  complex<double> b = 4.0*x0[k], 
    a = 2.0*conj(x0[k]) + b*x0[k],
    sigma = complex<double>(param.Value(2), param.Value(3))/a;

  complex<double> x(z.x,z.y);

  int i;

  if (sigma == 0.0) {
    // parabolic case

    // pre-Fatou coordinate tau(x) = -1/(ax) (involution)
    if (param.Value(1)>0) {
      x = -1.0/(a*x);
    }
    
    
    for ( i=0; (i<max) && (norm(x) < 100 ); i++ ) {
      x = x*(1.0 + x*(a + x*(b + x))); // f(x) = x + ax^2 + bx^3 + x^4
    }
    
  } else {
    // perturbed case
    
    complex<double> a1 = 1.0-a*sigma,
      a2 = a-b*sigma,
      a3 = b-sigma;

    complex<double> alpha = 1.0/(2*M_PI*I)*log(1.0-sigma*a);
    if (param.Value(1)>0) {
      x = sigma/(1.0 - exp(-2*M_PI*alpha*I*x));
    }

    for ( i=0; (i<max) && (norm(x) < 100 ); i++ ) {
      x = x*(a1 + x*(a2 + x*(a3 + x))); // f(x) = a1x + a2x^2 + a3x^3 + x^4
    }

  }

  if (i>=max)
    return -1;
  return i;
}

Point map(Point z, Point c, Parameter param)
{
  int k = param.Value(0);
  if (k<1) k=1;
  if (k>2) k=2;

  complex<double> b = 4.0*x0[k], 
    a = 2.0*conj(x0[k]) + b*x0[k],
    sigma = complex<double>(param.Value(2), param.Value(3))/a;
  
  complex<double> x(z.x,z.y);

  if (sigma == 0.0) {
    if (param.Value(1)>0) {
      x = -1.0/(a*x);
    }
    
    x = x*(1.0 + x*(a + x*(b + x))); // f(x) = x + ax^2 + bx^3 + x^4
    
    if (param.Value(1)>0) {
      x = -1.0/(a*x);
    }
  } else {
    complex<double> a1 = 1.0-a*sigma,
      a2 = a-b*sigma,
      a3 = b-sigma;
    
    complex<double> alpha = 1.0/(2*M_PI*I)*log(1.0-sigma*a);
    if (param.Value(1)>0) {
      x = sigma/(1.0 - exp(-2*M_PI*alpha*I*x));
    }
    
    x = x*(a1 + x*(a2 + x*(a3 + x))); // f(x) = a1x + a2x^2 + a3x^3 + x^4

    if (param.Value(1)>0) {
      x = 1.0/(2*M_PI*I*alpha)*log(x/(x-sigma));
    }
  }
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
