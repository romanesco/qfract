/*
 * Near-parabolic renormalization for cubic unicriticals by Yang Fei
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Cubic NP ren by Yang Fei";
const char* CHILD= "";
//

const double XL = -3.0;
const double YT = 4.0;
const double XR = 5.0;
const double YB = -4.0;

const int MAXITER = 1;
const int MAXORBIT = 1;
const int N = 10;
double VALUE[N]={0.03,125,16,16,16,16,16,16,16, 16};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "rho", "R", "Im Q(z)>0?",
			   "in D4?", "in D5?", "in D6?", "in D7?",
			   "|Q(z)|<rho?", "|Q(z)|>R?", "in D_{r_1}?"};

const double mu = 11-4*sqrt(6);
const double mu2 = 2.0-4.0/mu;

const complex<double> omega=complex<double>(8*sqrt(6)-3,6*sqrt(6)+4)/25.0;
const complex<double> a4(-0.22,0.69), a5(0.78,0.21);
const double eps4 = abs(a4-omega), eps5 = abs(a5-omega), eps6=0.41, eps7=0.82;

const double a0=-0.06, r0=1.07, r1=1.2;

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
  
extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x, z.y);
    double rho = param.Value(0);
    double R=param.Value(1);

    if  (norm(x) < 1) return -1;
    complex<double> y = Q(x);
    return (
	    param.Value(2)*(imag(y)>0) +
	    param.Value(3)*((abs(x-a4)<eps4) || (abs(x-conj(a4))<eps4) ) +
	    param.Value(4)*((abs(x-a5)<eps5) || (abs(x-conj(a5))<eps5) ) +
	    param.Value(5)* ((abs(x-omega)<eps6) || (abs(x-conj(omega))<eps6) ) +
	    param.Value(6)*(abs(x+1.0)<eps7) +
	    param.Value(7)* (abs(y) < rho) +
	    param.Value(8)*(abs(y) > R) +
	    param.Value(9)*(abs(x-a0)<r0*r1)
	    );
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
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
