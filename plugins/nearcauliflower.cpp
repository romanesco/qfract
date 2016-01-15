/*
 * quadratic maps (near cauliflower)
 */

#include "../plugin.h"
using namespace QFract;
#include <complex>
using namespace std;

#define INCOND  ( ( norm(x)<100 ) && ((!inold) || in) )


const char* NAME = "Quadratic Map (near cauliflower)";
const char* CHILD = "";

const double XL = -1.7;
const double YT = 1.2;
const double XR = 0.7;
const double YB = -1.2;

const int MAXITER = 500;
const int MAXORBIT = 10;
const int N=3;
double VALUE[N]={90,2,0.01};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "# of critical orbit",
			     "unused",
			     "arg(lambda)/(2*pi)" };		     

static double r2,r1; //, r12;
static complex<double> p, lambda;
static complex<double> c1, center, center2, center3;

extern "C" {
void paraminit( Parameter param )
{
    int i;
    lambda=exp(2*M_PI*complex<double>(0,param.Value(2)));
    p=1.0/lambda-1.0;
    c1=-0.5;

    for ( i=0; (i<param.Value(0)); i++) {
	c1 = lambda*c1*(1.0+c1);
    }
    if (p != 0.0)
	center = c1*p*conj(c1-p)/(conj(c1)*p-c1*conj(p));
    else
	center = c1/2.0;

    r2 = norm(center);
    //r1 = 1/param.Value(1)/2;
    //r12= r1*r1;
    center2=complex<double>(0,r1);
    center3=p-complex<double>(0,r1);
    
}

bool incond(complex<double> z)
{
    return (norm(z-center) < r2);
}

int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x,z.y);

    register int i;
    //int k=0;
    bool inold=false;
    bool in=incond(x);
    
    if (p != 0.0) {
	for ( i=0; (i<max) && INCOND; i++ ) {
	    inold=in;
	    x = lambda*x*(1.0+x);
	    in=incond(x);
	}
    } else {
	inold=false;
	for ( i=0; (i<max) && (norm(x)<100) && !in ; i++ ) {
	    x = lambda*x*(1.0+x);
	    if (in==incond(x)) inold=true;
	}
    }	
  if (i<max) {
      if ( !inold )
	  return -1;
      //if ( (norm(x-center2)<r12) || (norm(x-center3)<r12) )
      //  k = 90;
      //if ( imag(x) < 0 )
      //return i*4+1;
      
      return i*3;
  }
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> x(z.x,z.y);  
	x = lambda*x*(1.0+x);
	return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
