/*
 * Fatou coordinate for exp(2*pi*i*alpha)Q(z-b)
 * (Q(z) = z(1+1/z)^6/(1-1/z)^4)
 */

#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

#include "../plugin.h"
using namespace QFract;

const char* NAME = "e^{2\\pi i alpha}Q(z-b)";
const char* CHILD = "";
const char* COLORMAP = "per2.map";

const double XL = -110;
const double YT = 40;
const double XR = 110;
const double YB = -180;

const int MAXITER = 500;
const int MAXORBIT = 2500;

// Parameters
// 0: eta (for the renormalization)
// 1: eta (for the original map)
// 2: arg(b)/2\pi
// 3: |b|
// 4,5: alpha
// 6: stop iteration when in E? (bool)
// 7: steps for forward orbit (integer, iterate once if =<1)
// 8: stop iteration when 
const int N = 9;
double VALUE[N] = {2.0, 2.0, 0.0, 0.0,
		   1.0/(50.0-1.0/50.0), 0.0,
		   1.0, 50.0, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "eta", "eta1", "arg(b)/(2*pi)", "|b|",
			     "Re(alpha)", "Im(alpha)",
			     "isE (0 or 1)", "steps for fwd orbit",
			     "stop when im < -eta (0 or 1)" };

static complex<double> cv, cv2, alpha, lambda, slope, fp, b;

complex<double> map( complex<double> x)
{
    complex<double> y;
    complex<double> t;

    x -= b;
    t = x + 1.0;
    t *= t; y = t*t;
    y *= t;
    
    t = x - 1.0;
    t *= t; t *= t;
    y /= x * t ;
    y *= lambda;
    return y;
}

const double CRIT = 5+sqrt(24);

complex<double> diff(complex<double> z)
{
    complex<double> tmp, tmp2;
    z -= b;
    tmp=(z+1.0)/(z-1.0);
    tmp2 = tmp*tmp;
    tmp2 *= tmp2;
    tmp2 *= tmp;
    
    return lambda*tmp*(z*(z-10.0)+1.0)/(z*z);
}

complex<double> fixpt()
{
    complex<double> p=(10.0-b)/(2*M_PI*alpha), d=1.0;
    int i=0;
    do {
	d = (map(p)-p)/(diff(p)-1.0);
	p -= d;
	i++;
    } while ( (i < 100) && (norm(d)>0.0001) );
    return p;
}

extern "C" {
void paraminit(Parameter param)
{
    b = param.Value(3)*exp(2*M_PI*complex<double>(0,param.Value(2)));
    cv=CRIT+b;
    alpha = complex<double>(-param.Value(5), param.Value(4));
    lambda=exp(-2*M_PI*alpha);
    for (int i=0; i<20; i++)
	cv = map(cv);
    cv2 =map(cv);
    if (alpha != 0.0) {
	fp = fixpt();
	cout << "fix pt = " << fp << endl;
    }
    slope = 1.0/(cv2-cv);
    
    cout << "lambda = " << lambda << endl;
    cout << "b = " << b << endl;

    
}

int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x,z.y);
    
    if ( ( (alpha != 0.0) && 
	   ( imag(x/(cv-fp)) < imag(cv/(cv-fp)) &&
	     ( imag(x/(cv2-fp)) >= imag(cv2/(cv2-fp)) ) ))
	 || (alpha == 0.0) && ( real(x) > real(cv) ) ) {
	if (param.Value(0) > 0)
	    if (imag((x-cv)*slope) < -param.Value(0)) {
		if (!param.Value(8))
		    return 101;
		else
		    goto a;
	    } else if (imag((x-cv)*slope) > param.Value(0))
		return 100;
	if ( imag((x-cv)*slope) < 0 ) {
	    if (!param.Value(8))
		return 1;
	} else
	    return 0;
    }
    
a:
    double el=exp(2*M_PI*param.Value(1));
    double y=abs(map(x));

    if ( (y < 27.0/el) || ((abs(x) < 27.0*el) && (y > 27.0*el)) )
	return -1;

    register int i;

    for ( i=0; i<max; i++ ) {
	if ( (param.Value(6)) && 
	     ((real(x)+0.18)*(real(x)+0.18)/(1.24*1.24) 
	      + imag(x)*imag(x)/(1.04*1.04) < 1) )
	    return -1;
	x = map(x);
	if (alpha != 0.0) {
	    if ( ( imag(x/(cv-fp)) < imag(cv/(cv-fp)) ) &&
		 ( imag(x/(cv2-fp)) >= imag(cv2/(cv2-fp)) ) &&
		 (!(param.Value(8)) || (imag((x-cv)*slope) >= 0)))
		break;
	}
	else
	    if ( real(x) > real(cv) )
		break;
    }

    if (i<max) {
	if (param.Value(0) > 0)
	    if (imag((x-cv)*slope) < -param.Value(0))
		return (i % 2)*20+101+i-20;
	    else if (imag((x-cv)*slope) > param.Value(0))
		return (i % 2)*20+100+i-20;
	if ( imag((x-cv)*slope) < 0 )
	    return (((i % 2)*50+1+i-20+256) % 256);
	
	else
	    return (((i % 2)*50+i-20+256) % 256);
    }
    else
	return -1;
}

Point map(Point z, Point, Parameter param)
{
    complex<double> x(z.x,z.y);
    complex<double> a(param.Value(2), param.Value(3));
    x = map(x);
    
    if (param.Value(7))
	for (int i=1; i<param.Value(7); i++)
	    x=map(x);

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
