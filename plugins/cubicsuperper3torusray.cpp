/*
 * Cubic polynomial with period 3 critical point
 * z -> a z^3 + b z^2 +1
 * with a = -(c^3-c^2+1)/(c^3-c^2) and b = (c^4-c^3+1)/(c^3-c^2)
 * ( f(0)=1, f(1)=a+b+1=c, f(c)=0 )
 * parametrized using Weierstrass p-function
 */

#include <cmath>
#include <complex>
using namespace std;

#include "../plugin.h"
using namespace QFract;
#include "math.h"
#define NEED_LIBWEIERSTRASSP 1
#include "weierstrassp.cpp"

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION tmp1 = re2-im2; \
                 tmp2 = 2*re*im; \
                 tmp3 = are * re - aim * im + bre; \
                 im   = aim * re + are * im + bim; \
                 re   = tmp1 * tmp3 - tmp2 * im + 1; \
                 im   = tmp2 * tmp3 + tmp1 * im;

#define INCOND  ( (re2+im2) < rad )

const char* NAME = "Cubic with superattr 3-per pt (parameter space with ray)";
const char* CHILD = "cubicsuperper3torusrayjulia.so";

/*
const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;
*/

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

const complex<double> g2=-20.0/3.0, g3=-44.0/27.0;

static WeierstrassP P(g2,g3);

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x,z.y), p=P.P(x), dp=P.dP(x);
    complex<double> sqrta = -3.0*I*dp/(2.0*(3.0*p+2.0)),
	c = 1.0 + 1.0/(p-1.0/3.0), a=sqrta*sqrta, 
	b = (c-1.0)*c*c;
    b = (b*c+1.0)/b;
    
    double are, aim, bre, bim, tmp1, tmp2, tmp3;
    are = real(a); aim = imag(a);
    bre = real(b); bim = imag(b);
	
    // first critical point -2b/3a
    tmp1 = 3.0 * (are * are + aim * aim);
    tmp2 = -2.0 * (are * bre + aim * bim);
    tmp3 = -2.0 * (-aim * bre + are * bim);
    register double re= tmp2 / tmp1;
    register double im= tmp3 / tmp1;
    
    register double re2=re*re;
    register double im2=im*im;
    
    //register double rad = (are*are + aim*aim >5) ? are*are + aim*aim : 5;
    register double rad = are*are + aim*aim;
    rad = sqrt(rad) + re*re+im*im; rad = sqrt(rad);
    rad = 100*(rad+10/rad);
    
    register int i;
    for ( i=0; (i<max) && INCOND; i++ ) {
	FUNCTION
        RE2IM2
    }
    
    if (i>=max)
	return -1;
    else {
	complex<double> x(re,im);
	x *= sqrta;
	if (imag(x) < 0)
	    return i*4+11;
	return i*4;
    }
}

Point map(Point z, Point d, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double re2=re*re;
  register double im2=im*im;

    complex<double> x(d.x,d.y), p=P.P(x), dp=P.dP(x);
    complex<double> sqrta = -3.0*I*dp/(2.0*(3.0*p+2.0)),
	c = 1.0 + 1.0/(p-1.0/3.0), a=sqrta*sqrta, 
	b = (c-1.0)*c*c;
    b = (b*c+1.0)/b;
    
    double are, aim, bre, bim, tmp1, tmp2, tmp3;
    are = real(a); aim = imag(a);
    bre = real(b); bim = imag(b);

  FUNCTION

  return Point(re,im);
}

Point init(Point d, Parameter param)
{
    complex<double> x(d.x,d.y), p=P.P(x), dp=P.dP(x);
    complex<double> sqrta = -3.0*I*dp/(2.0*(3.0*p+2.0)),
	c = 1.0 + 1.0/(p-1.0/3.0), a=sqrta*sqrta, 
	b = (c-1.0)*c*c;
    b = (b*c+1.0)/b;
    
    double are, aim, bre, bim, tmp1, tmp2, tmp3;
    are = real(a); aim = imag(a);
    bre = real(b); bim = imag(b);

    // first critical point
    tmp1 = 3.0 * (are * are + aim * aim);
    tmp2 = -2.0 * (are * bre + aim * bim);
    tmp3 = -2.0 * (-aim * bre + are * bim);
    register double re= tmp2 / tmp1;
    register double im= tmp3 / tmp1;
    
    return Point(re,im);
}

PluginInfo* getInfo()
{
    double s = -imag(P.getOmega1());
    PluginInfo* i= new PluginInfo( NAME, 0.0, s, s, 0.0, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
