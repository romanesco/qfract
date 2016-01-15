/*
 * Biquadratic family with fixed point (parameter space)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Biquad family (with fp 0)";
const char* CHILD = "biquadfixjuliaalt.so";
const char* COLORMAP = "dim2-2.map";

const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;

const int MAXITER = 50;
const int MAXORBIT = 10;
const int N=2;
double VALUE[N]={ 0.9, 0.0 };
const Parameter PARAM(N,VALUE);
const char* PARAMDESC[N] = { "Re(lambda)", "Im(lambda)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> lambda(param.Value(0),param.Value(1));
    complex<double> t(z.x, z.y), a, b, c;
    complex<double> x, cv;
    if (t != 0.0) {
	a = lambda/t+t*t;
	b = 2.0*t;
	c = 1.0;
	x = -0.5*t;
	cv = -0.25*lambda*lambda/(t*t);
	//cv = -lambda*lambda*lambda*(1.0-lambda)/(2.0*t*t);
    } else { // quadratic polynomial
	a = 1.0;
	b = 0.0;
	c = 0.0;
	x = -0.5;
    }
    bool isAttr = (norm(lambda)<1);
    double k=abs(t);
    if (isAttr) 
	k = (1.0-abs(lambda))/5*(abs(t)<1 ? abs(t) : 1.0/abs(t));
    // first critical point
    register int i,j;
    for ( i=0; (i<max) && (norm(x)<100); i++ ) {
	x = x*(lambda+x*(a+x*(b+x*c)));
	if (isAttr && (norm(x)<k))
	    i=max;
    }

    // second critical value
    if ( a != 0.0 ) {
	x = cv;
	for ( j=0; (j<max) && (norm(x)<100); j++ ) {
	    x = x*(lambda+x*(a+x*(b+x*c)));
	    if (isAttr && (norm(x)<k))
		j=max;
	}
    } else { j = max; }
    
    if ((i>=max) && (j>=max))
	return -1;
    else if (i>=max)
	return (j%15)+1;
    else if (j>=max)
	return ((i%15)+1)*16;
    else
	return ((i%15)+1)*16+(j%15)+1;
    //return ((i<j) ? j*2: i*2+1);
}
    
Point map(Point z, Point c, Parameter param)
{
    complex<double> lambda(param.Value(0),param.Value(1));
    complex<double> t(c.x, c.y), a, b, d;
    if (t != 0.0) {
	a = lambda/t+t*t;
	b = 2.0*t;
	d = 1.0;
    } else { // quadratic polynomial
	a = 1.0;
	b = 0.0;
	d = 0.0;
    }
    complex<double> x(z.x,z.y);

    x = x*(lambda+x*(a+x*(b+x*d)));
    return Point(real(x),imag(x));
}

//Point init(Point c, Parameter param) {return Point(-0.5, 0.0);}
Point init(Point c, Parameter param) 
{
    complex<double> lambda(param.Value(0),param.Value(1));
    complex<double> t(c.x, c.y);
    complex<double> x;
    if (t != 0.0) {
	x = -lambda*lambda*lambda*(1.0-lambda)/(2.0*t*t);
    } else { // quadratic polynomial
	x = -0.5;
    }
    return Point(real(x), imag(x));
}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
