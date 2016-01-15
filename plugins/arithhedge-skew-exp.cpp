/*
 * Arithmetic hedgehog
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Arithmetic Hedgehog";
const char* CHILD = "";
const char* COLORMAP = "per3.map";

const double XL = -1.0;
const double YT = 1.0;
const double XR = 1.0;
const double YB = -1.0;

const int MAXITER = 5;
const int MAXORBIT = 1;
const int N = 5;
double VALUE[N]={2,2,2,2,2};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "a1", "a2", "a3", "a4", "a5" };

const int NUM = 256;
const int COLNUM = 3;

const complex<double> I(0,1);

complex<double> phi(double alpha, complex<double> w)
{
    double a,b,c,d;
    double x=real(w),y=imag(w), e=exp(-x);
    d = 2*M_PI*alpha; d *= d;
    a = d-1;
    b = cos(y)*e - d;
    c = d-e*e;
    d = b*b-a*c;
    if (d < 0) return complex<double>(-100,y/alpha);
    e = (-b-sqrt(d))/a;
    if (e < 0) return complex<double>(-100,y/alpha);
    
    return complex<double>(
	(x+log(e))/alpha, y/alpha);
}

complex<double> psi(double alpha, complex<double> x)
{
    double t=real(x), s=imag(x);
    return complex<double>(
	alpha*t+log(1+abs(1.0-exp(-alpha*x))/(2*M_PI*alpha)),
	alpha*s);
}

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x, z.y);
    if (x == 0.0) return -1;
    x = -log(x);
    if (real(x)<0) return 2*((int) -real(x))+COLNUM-1;

    register double alpha=1/param.Value(N-1);
    register int i;
    for ( i=N-2; i>=0; i--)
    {
	alpha=1.0/(param.Value(i)+alpha);
    }

    for ( i=0; i<max; i++ ) {
	x = phi(alpha,x);
	if (real(x)<0) return i+COLNUM*(((int) (-real(x)/M_PI)) % (NUM/COLNUM));
	alpha = 1/alpha;
	alpha = alpha - (int) alpha;
    }
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x, z.y);
    x *= 2.0*M_PI;
    double alpha=1/param.Value(N-1);
    for ( int i=N-2; i>=0; i--)
    {
	alpha=1.0/(param.Value(i)+alpha);
    }
    x=phi(alpha,x);
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
