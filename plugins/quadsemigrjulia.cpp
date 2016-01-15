/*
 * Semigroup generated by z^2+c and z^2-c (Julia set, by random trials)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
#include <random>
using namespace std;

const char* NAME = "Semigroup generated by z^2+c, z^2-c (Julia set, by random trials)";
const char* CHILD = "";
const char* COLORMAP = "default2.map";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 10;
const int MAXORBIT = 10;
const int N = 4;
double VALUE[N]={1.0, 0.0, 10, 10};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "Re(c)", "Im(c)", "unused", "trials" };

std::random_device rnd;
std::mt19937 mt(rnd());
std::uniform_int_distribution<int> dis(0,1);

extern "C" {
int iter(Point z, Parameter param, int max)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x0(z.x,z.y), x;
    complex<double> tmp;
    int N(param.Value(3));

    int i,j, n=-1;
    for ( j=0; j<N; j++) {
      x = x0;
      for ( i=0; (i<max) && (norm(x) < 100 ); i++ ) {
	x *=x;
	x += dis(mt)  ? a : -a;
	//x = x*((dis(mt)  ? a : -a)+x);
      }
      if (i >= max) n++;
    }
    return n;
}

Point map(Point z, Point c, Parameter param)
{
    complex<double> a(param.Value(0), param.Value(1));
    complex<double> x(z.x,z.y);
    complex<double> tmp;

    register int j;
    tmp=x;
    for ( j=1; j<param.Value(2); j++)
	tmp *= x;
    x = tmp + a;
    
    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
