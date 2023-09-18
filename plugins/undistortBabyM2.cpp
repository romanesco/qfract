/*
 * Distorted Mandelbrot set z^2+c by Möbius (pole coordinate)
 */

#include "../plugin.h"
using namespace QFract;
#include <iostream>
#include <complex>
using namespace std;

const char* NAME = "Distorted Mandelbrot set by Möbius (pole coordinate)";
const char* CHILD= "undistortBabyM.so";
//

const double XL = -3.0;
const double YT = 3.0;
const double XR = 3.0;
const double YB = -3.0;

const int MAXITER = 5000;
const int MAXORBIT = 89;
const int N = 3;
double VALUE[N]={3.367E-5, -0.7495155566666667, 0.037136936666666606 };
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N]={ "scale", "Re(center)", "Im(center)" };

extern "C" {
int iter(Point z, Parameter param, int max)
{
  return -1;
}

Point map(Point z, Point c, Parameter param)
{
  return z;
}

Point init(Point c, Parameter param) 
{
  return c;
}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo(NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				PARAM, CHILD, PARAMDESC);
  return i;
}
}
