//
// Per_2(lambda)
// z -> z^3 + a z^2 + b z^2 + c
// (parameter space)
// 

#include <cmath>
#include "../plugin.h"
using namespace QFract;

const char* NAME = "Per_2(lambda) for cubic family (lambda-plane)";
const char* CHILD = "per2curves.so";
const char* COLORMAP = "per2.map";

const double XL = -1.2;
const double YT = 1.2;
const double XR = 1.2;
const double YB = -1.2;

const int MAXITER = 0;
const int MAXORBIT = 0;
const int N = 0;
double VALUE[N] = {  };
const Parameter PARAM( N, VALUE );

extern "C" {
int iter(Point z, Parameter param, int max)
{
    double r2 = z.x*z.x+z.y*z.y;
    if (r2 == 0)
	return -1;

    if (r2 > 1.0)
	return ((int) log(r2))*10;
    else 
	return -((int) log(r2))*10+1;
}

Point map(Point z, Point c, Parameter param)
{
    return z;
}

Point init(Point e, Parameter param)
{
    return e;
}

PluginInfo* getInfo()
{
    PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				   PARAM, CHILD, COLORMAP);
  return i;
}

}
