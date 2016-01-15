/*
 * $Id: fix-twocrit.cpp $
 * polynomial with fixed point 0 and two critical point with
 * multiplicity 1 and d-1
 * z -> (z-a)^{d-1}((d-1)z - db-a)) + (-a)^{d-1}(db-a)
 * (phase space with external rays)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
#include <iostream>
#include <cmath>
using namespace std;

#define INIT	tmp = -a; \
                for (int i=1; i<deg-2; i++) \
                  tmp *= -a; \
                b = -l/(((double) deg*(deg-1))*tmp); \
                d = ((double) deg)*b-a; \
                c = tmp*(-a)*d;

#define FUNCTION tmp = z-a; \
                 for (int i=1; i<deg-1; i++) \
                   tmp *= z-a; \
                 z = tmp * (((double) (deg-1))*z-d)+c;

#define INCOND	(norm(z) < 100)

const char* NAME = "Polynomial with one fixed point & two critical points (Julia set)";
const char* CHILD = "";
const char* COLORMAP = "per2-2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 100;
const int MAXORBIT = 10;

const int N = 5;
double VALUE[N] = {1.0, 0.0, 1.0, 0.0, 3};
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "Re(a)", "Im(a)", "Re(lambda)", "Im(lambda)",
			     "degree" };
static int deg = 3;

extern "C" {
void paraminit( Parameter param )
{
    deg = (int) param.Value(4);
}

int iter(Point p, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> l(param.Value(2), param.Value(3));

    complex<double> b, c, d, tmp;
    
    INIT

    int i;

    // first critical point (=a)
    complex<double> z(p.x, p.y);
    for (i=0; (i<max) && INCOND; i++) {
	FUNCTION
    }

    if (i>=max)
	return -1;

    if (imag(z) > 0)
	return i*2;
    return i*2+1;
}

Point map(Point z1, Point p, Parameter param)
{
    complex<double> z(z1.x, z1.y);
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> l(param.Value(2), param.Value(3));

    complex<double> b, c, d, tmp;
    
    INIT
    FUNCTION
    return Point(real(z), imag(z));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
