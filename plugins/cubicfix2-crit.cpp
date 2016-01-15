/*
 * Cubic polynomial with fixed point
 * z -> z^3 - a z^2 + b z(parameter space, b fixed)
 */

#include "../plugin.h"
using namespace QFract;
#include "math.h"
#include <complex>
using namespace std;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define INIT     

#define FUNCTION tmp1 = (re-are)*re-(im-aim)*im; \
                 tmp2 = (re-are)*im+(im-aim)*re; \
		 tmp3 = (tmp1+bre)*re - (tmp2+bim)*im; \
		 im = (tmp1+bre)*im + (tmp2+bim)*re; \
		 re = tmp3;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Cubic (with fix pt 0)";
const char* CHILD = "cubicfixjulia.so";
const char* COLORMAP = "per2.map";

const double XL = -4.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -4.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 2;
double VALUE[N] = {-0.737368878078320,-0.675490294261524};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(b)", "Im(b)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x,z.y), b(param.Value(0),param.Value(1));
    complex<double> cp=sqrt(x);
    complex<double> cp2=b/(3.0*cp), a = 3.0/2.0*(cp+cp2);
    
    register double are=real(a);
    register double aim=imag(a);
    
    register double bre=real(b);
    register double bim=imag(b);
    
    INIT
	
    register double tmp1, tmp2, tmp3;
    
    register double re, im;

    re=real(cp); im=imag(cp);

    register double re2=re*re;
    register double im2=im*im;
    
    register int i;
    for ( i=0; (i<max) && INCOND; i++ ) {
	FUNCTION
        RE2IM2
    }

    //second critical point
    re=real(cp2); im=imag(cp2);
    RE2IM2
    register int j;
    for ( j=0; (j<max) && INCOND; j++ ) {
	FUNCTION
        RE2IM2
    }

    if ((i>=max) && (j>=max))
	return -1;
    else 
	return ((i<j) ? i*2: j*2+1);
}
    
Point map(Point z, Point c, Parameter param)
{
    complex<double> x(c.x,c.y), b(param.Value(0),param.Value(1));
    complex<double> cp=sqrt(x);
    complex<double> cp2=b/(3.0*cp), a = 3.0/2.0*(cp+cp2);
    
    register double are=real(a);
    register double aim=imag(a);
    
    register double bre=real(b);
    register double bim=imag(b);
    
  register double re=z.x;
  register double im=z.y;
  register double tmp1, tmp2, tmp3;

  INIT
  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param)
{
    complex<double> x(c.x,c.y), cp=sqrt(x);
    return Point(real(cp),imag(cp));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
