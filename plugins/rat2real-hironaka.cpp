/*
 * Real quadratic rational map with x → 0 (cp) → ∞ → y (parameter space)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Real quadratic rational map (parameter space)";
const char* CHILD= "rat2real-hironakajulia.so";
//

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;

const int MAXITER = 1000;
const int MAXORBIT = 10;
const int N = 1;
double VALUE[N]={0.01};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "ε" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  double eps=param.Value(0);
  eps *= eps;
  double a = z.x, b=z.y;
  double r = (1-a-b)/(1-a);
  
  double x=b;

  register int i;
  for ( i=0; (i<max) && (norm(x-a) > eps ); i++ ) {
    x = (x-a)*(x-r)/(x*x);
  }
  
  if (i>=max)
    return -1;
  else
    return i*11;
}

Point map(Point z, Point c, Parameter param)
{
  double a = c.x, b=c.y;
  double r = (1-a-b)/(1-a);

  double x = z.x;

  x = (x-a)*(x-r)/(x*x);

  return Point(x, z.x);
}

Point init(Point c, Parameter param) 
{
  return Point(c.y,0);
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
