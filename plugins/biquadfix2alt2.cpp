/*
 * Biquadratic family with fixed point (parameter space)
 */

#include <iostream>
#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Biquad family (with fp 0)";
const char* CHILD = "biquadfixjuliaalt2.so";
const char* COLORMAP = "dim2-2.map";

const double XL = -10.0;
const double YT = 10.0;
const double XR = 10.0;
const double YB = -10.0;

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
    complex<double> a, b(z.x, z.y);
    if ( lambda != 0.0 ) {
	b /= (lambda*sqrt(lambda));
	if ( real(b) >= 0 )
	    a = b + sqrt(b*b-1.0);
	else
	    a = b - sqrt(b*b-1.0);
	a *= sqrt(lambda);
    }
    complex<double> cp=-0.5, x=cp, y;
    
    bool isAttr = (norm(lambda)<1);
    double k;
    if (isAttr) {
	k = (1.0/abs(lambda) - 1.0)/5.0;
	k = (k<1.0) ? k : 1.0;
    }
    // first critical point
    register int i,j;
    for ( i=0; (i<max) && (norm(x)<100); i++ ) {
	y = x*(1.0+x);
	x = lambda*y*(1.0+a*y);
	if (isAttr && (norm(x)<k))
	    i=max;
    }

    // second critical value
    if ( a != 0.0 ) {
	x = lambda/a*cp*(1.0+cp);
	for ( j=0; (j<max) && (norm(x)<100); j++ ) {
	    y = x*(1.0+x);
	    x = lambda*y*(1.0+a*y);
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
    complex<double> a, b(c.x, c.y);
    if ( lambda != 0.0 ) {
	b /= (lambda*sqrt(lambda));
	if ( real(b) >= 0 )
	    a = b + sqrt(b*b-1.0);
	else
	    a = b - sqrt(b*b-1.0);
	a *= sqrt(lambda);
    }
    complex<double> x(z.x,z.y), y;

    y = x*(1.0+x);
    x = lambda*y*(1.0+a*y);
    return Point(real(x),imag(x));
}

//Point init(Point c, Parameter param) {return Point(-0.5, 0.0);}
Point init(Point c, Parameter param) 
{
    complex<double> lambda(param.Value(0),param.Value(1));
    complex<double> a, b(c.x, c.y);
    if ( lambda != 0.0 ) {
	b /= (lambda*sqrt(lambda));
	if ( real(b) >= 0 )
	    a = b + sqrt(b*b-1.0);
	else
	    a = b - sqrt(b*b-1.0);
	a *= sqrt(lambda);
    }
    complex<double> cp=0.5, x;
    if (a != 0.0) {
	x = lambda/a*cp*(1.0+cp);
	return Point(real(x), imag(x));
    } else {
	return Point(-0.5, 0.0);
    }
}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
