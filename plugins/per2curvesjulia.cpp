//
// Per_2(lambda)
// z -> z^3 + a z^2 + b z^2 + c
// (phase space)
// 

#include <cmath>
#include <complex>
using namespace std;

#include "../plugin.h"
using namespace QFract;

#define NEED_LIBWEIERSTRASSP 1
#include "weierstrassp.cpp"

static WeierstrassP* P;
static complex<double> l;

const char* NAME = "Per_2(lambda) for cubic family";
const char* CHILD = "";

const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N] = { 1.0, 0.0, 0.1, 0.0 };
const Parameter PARAM( N, VALUE );

// calculate g2 from l=sqrt(lambda)
complex<double> getg2(complex<double> l)
{
    return -(16.0/3)*(3.0*l*l-4.0)*(l+1.0)
	/ (l*pow(l*(l+1.0),1.0/3));
}


complex<double> getg3(complex<double> l)
{
    return (64.0/27)*(l+1.0)*(9.0*l*l-8.0)/(l*l);
}

// Delta = (c^2-2)^2 - 4*lambda
// need: (c, sqrt(Delta))

// f: value of P-function
// d: intermediate coordinate
complex<double> getd(complex<double> f, complex<double> l)
{
    complex<double> d, e, w;
    w = pow(l*(l+1.0),1.0/3.0);
    e = f/(2.0*w);
    d = e - (2.0/3)/l;
    return d;
}

complex<double> getc(complex<double> d, complex<double> l)
{
    complex<double> c;
    c = sqrt(2.0*(1.0+l))*(d+1.0)/(d-1.0);
    return c;
}

// sqrt(Delta*(d-1)^4) = dP
complex<double> getSqrtDelta(complex<double> dp, complex<double> d,
			 complex<double> l)
{
    complex<double> z=d-1.0;
    z = z*z;
    return dp/z;
}

extern "C" {
void paraminit(Parameter param)
{
    complex<double> lambda(param.Value(2),param.Value(3));
    l = sqrt(lambda);
    P = new WeierstrassP(getg2(l),getg3(l));
    complex<double> a, b, c, d, sqrtdelta, p, dp, 
	x(param.Value(0), param.Value(1));
    p = P->P(x);
    dp = P->dP(x);
    
    d = getd(p,l);
    c = getc(d,l);
    sqrtdelta = getSqrtDelta(dp,d,l);

    a = (-3.0*c*c+sqrtdelta)/(2.0*c);
    b = (c*c-2.0-sqrtdelta)/2.0;
    cout << "a: " << a << endl;
    cout << "b: " << b << endl;

}

int iter(Point z, Parameter param, int max)
{
    complex<double> a, b, c, d, sqrtdelta, p, dp, 
	x(param.Value(0), param.Value(1));
    p = P->P(x);
    dp = P->dP(x);
    
    d = getd(p,l);
    c = getc(d,l);
    sqrtdelta = getSqrtDelta(dp,d,l);

    a = (-3.0*c*c+sqrtdelta)/(2.0*c);
    b = (c*c-2.0-sqrtdelta)/2.0;

    int i;
    x = complex<double>(z.x,z.y);
    for (i=0; (i<max) && (norm(x)<100); i++) {
	x = ((x+a)*x+b)*x+c;
    }

    if (i>=max)
	return -1;
    else 
	return i*2;
}

Point map(Point z, Point e, Parameter param)
{
    complex<double> a, b, c, d, sqrtdelta, p, dp, 
	x(param.Value(0), param.Value(1));
    p = P->P(x);
    dp = P->dP(x);
    
    d = getd(p,l);
    c = getc(d,l);
    sqrtdelta = getSqrtDelta(dp,d,l);

    a = (-3.0*c*c+sqrtdelta)/(2.0*c);
    b = (c*c-2.0-sqrtdelta)/2.0;
    
    x = complex<double>(z.x, z.y);
    x = ((x+a)*x+b)*x+c;

    return Point(real(x),imag(x));
}

Point init(Point e, Parameter param)
{
    return e;
}

PluginInfo* getInfo()
{
    PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}

}
