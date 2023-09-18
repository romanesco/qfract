/*
 * Distorted Mandelbrot set z^2+c by Möbius
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Distorted Mandelbrot set by Möbius";
const char* CHILD= "";
//

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 5000;
const int MAXORBIT = 89;
const int N = 5;
double VALUE[N]={1.5, 0.8, 3.367E-5, -0.7495155566666667, 0.037136936666666606 };
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "Re(pole)", "Im(pole)", "scale", "Re(center)", "Im(center)" };

complex<double> M(complex<double> a, Parameter param)
{
  double k=param.Value(2);
  complex<double> center(param.Value(3), param.Value(4));
  complex<double> pole(param.Value(0), param.Value(1));
  a = -k*a/(a/pole-1.0) + center;
  
  return a;
}


extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(z.x, z.y);
    a = M(a, param);
    
    complex<double> x=a;

    register int i,j;
    for ( i=0; (i<max) && (norm(x) < 100 ); i++ ) {
      x = x*x+a;
    }
    
    if (i>=max)
	return -1;
    else
	return i;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(c.x, c.y);
    a = M(a, param);
    
    complex<double> x(z.x, z.y);

    register int j;
    x = x*x + a;
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    complex<double> a(c.x, c.y);
    a = M(a, param);
    
    return Point(real(a), imag(a));
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
