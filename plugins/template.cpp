/*
 * template for new plugins
 */

#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

#include "../plugin.h"
using namespace QFract;

// Description (displayed on window title)
const char* NAME = "";
// Child plugin filename (foo.so)
const char* CHILD = "";
// Default colormap file
const char* COLORMAP = "default.map";

// region to display (in (x,y)-coordinate)
const double XL = ; // x left = minimal x value
const double YT = ; // y top = maximal y value
const double XR = ; // x right = maximal x value
const double YB = ; // y bottom = minimal y value

// number of maximal iteration
const int MAXITER = ;
// number of maximal iteration for forward orbit
const int MAXORBIT = ;

// number of parameters
const int N = ;
// default parameter values
double VALUE[N] = {};
const Parameter PARAM( N, VALUE );

// descriptions of parameters
const char* PARAMDESC[N] = { };

// I=sqrt(-1)
const complex<double> I(0,1);

extern "C" {
// setting static parameters
void paraminit(Parameter param)
{
}

// do iteration for given point z
int iter(Point z, Parameter param, int max)
{
  complex<double> x(z.x,z.y);

  int i;
  for (i=0; (i<max) && (INCOND); i++) {
    FUNCTION
  }
  
  if (i>=max){
    return -1;
  }

}

// for drawing forward orbit
Point map(Point z, Point c, Parameter param)
{
  complex<double> x(z.x,z.y);

  return Point(real(x),imag(x));
}

// initial point for drawing forward orbit
Point init(Point c, Parameter param) 
{
  return c;
}

// returning information of the plugin (normally no need to edit)
PluginInfo* getInfo()
{
    PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				   PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
