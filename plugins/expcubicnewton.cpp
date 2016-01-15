/*
 * Newton maps for exp(cubic polnomial) (parameter space)
 * z -> z - 1/(z^2+a)
 */

#include <complex>
using namespace std;
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Newton map for exp(cubic polyn) (parameter space)";
const char* CHILD = "expcubicnewtonjulia.so";
const char* COLORMAP = "per3.map";

const double XL = -2.5;
const double YT = 3.0;
const double XR = 3.5;
const double YB = -3.0;
//const double CRE = 0.0;
//const double CIM = 1.0;

const int MAXITER = 1000;
const int MAXORBIT = 10;
const int N=1;
double VALUE[N]={0.00001};
const Parameter PARAM(N,VALUE);
const char* PARAMDESC[N] = { "epsilon" };

#define FUNCTION x = x - 1.0/(x*x+a); 

#define INCOND ( abs(-1.0/(x*x*x) - eps) > eps )
               
const complex<double> omega(-1,sqrt(3));

extern "C" {
int iter(Point z, Parameter param, int max)
{
  complex<double> a(z.x, z.y), x, t, alpha, beta;
    double eps = param.Value(0);

    // 1st critical point
    /*
    alpha = a*a*a;
    alpha = power(alpha + 27.0 + 3.0*sqrt(6.0*alpha + 81.0),1.0/3.0);
    beta = (alpha - a*a/alpha)/3.0;
    alpha = (alpha + a*a/alpha)/3.0;
    */

    x = 0.0;
    register int i;
    for (i=0; (i<max) && (INCOND); i++) {
      FUNCTION
    }
    
    if (i>=max)
	return -1;
    else if (abs(arg(-x)) < M_PI/3)
	return i*3;
    else if (abs(arg(-omega*x)) < M_PI/3)
	return i*3+1;
    else
	return i*3+2;
}
    
    Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y), x(z.x,z.y), t;

    FUNCTION

    return Point(real(x),imag(x));
}

    Point init(Point c, Parameter param) {return Point((c.x+1)/3,c.y/3);}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
