/*
 * Near-parabolic renormalization for cubic unicriticals by Yang Fei
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Cubic NP ren (Section 14) by Yang Fei";
const char* CHILD= "";
//

const double XL = -20.0;
const double YT = 20.0;
const double XR = 20.0;
const double YB = -20.0;

const int MAXITER = 1;
const int MAXORBIT = 1;
const int N = 5;
double VALUE[N]={ 1, 48, 0, 0, 0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Q(x) in V", "x in W0", "Q(x) in W0",
			   "x in D5, D6", "x in Omega0"};
const char* COLORMAP = "per3.map";

const double cp = (1+4*sqrt(6)+2*sqrt(18+2*sqrt(6)))/5;
const double cv = 3*(8*sqrt(6)+3)/4;

const double mu = 11-4*sqrt(6);
const double mu2 = 2.0-4.0/mu;

const complex<double> omega=complex<double>(8*sqrt(6)-3,6*sqrt(6)+4)/25.0;
const complex<double> a4(-0.22,0.69), a5(0.78,0.21);
const double eps4 = abs(a4-omega), eps5 = abs(a5-omega), eps6=0.41, eps7=0.82;
const double E4 = eps4*eps4, E5 = eps5*eps5, E6 = eps6*eps6, E7 = eps7*eps7;

const double a0=-0.06, r0=1.07, r1=1.2;

const double h0 = real(omega)+imag(omega)+eps6;

// Q(z) = (1+1/z)^6*(1-1/z)^4 / (1+mu2/z+1/z^2)^4
complex<double> Q(complex<double> z)
{
  complex<double> zinv = 1.0/z;
  complex<double> a = 1.0+zinv, b = 1.0-zinv, c = a*b;
   c *= c; c *= c; c *= a*a; // c: numerator
  a = 1.0 + (mu2+zinv)*zinv;
  a *= a; a *=a; // a: denominator
  return z*c/a;
}
  
bool inW0(complex<double> x)
{
  return ( real(x) > cp) || (real(x)+abs(imag(x)) > sqrt(2)*6.5);
}

// check if y lies in V(cv,3pi/4)
bool inV(complex<double> y)
{
  return (real(y)+abs(imag(y)) > cv);
}

bool inD5(complex<double> x)
{
  if (imag(x)<0) { x = conj(x); }
  return (norm(x-a5)<E5);
}

bool inD6(complex<double> x)
{
  if (imag(x)<0) { x = conj(x); }
  return (norm(x-omega)<E6);
}

bool inOmega0(complex<double> x)
{
  return ( (real(x)>real(omega)) || (abs(imag(x))>h0+1) || (abs(imag(x)) > -real(x)+h0) ) && !(inD5(x) || inD6(x));
}
  
extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x, z.y);
    if (norm(x) < 1) return -1;
    
    complex<double> y = Q(x);

    return inV(y) * param.Value(0) +
      inW0(x)  * param.Value(1) +
      inW0(y) * param.Value(2) + 
      (inD5(x) + inD6(x) + (imag(y)>0) ) *param.Value(3) +
      inOmega0(x) * param.Value(4);
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x, z.y);

    x = Q(x);
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    return c;
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
