/*
 * Kyounghee Kim's birational map Lσ, where
 * L: a linear transformation
 * σ： cremona transformation
 * (in R^2)
 */

#include "../plugin.h"
using namespace QFract;

#define FUNCTION \
  den = .43479167440053448494935455009397596083970904704335*x+.56520832559946551505064544990602403916029095295670*x*y; \
  tmp = .24684626394461912521000014860057190445472434779259*x+.75315373605538087478999985139942809554527565220750*x*y; \
  y = (26.982266348563755135569907990690679456215081241821*y-4.1285616771552447815776999394955796665894859507924*x-21.853704671408510353992208051195099789625595291032*x*y)/den; \
  x = tmp/den;

#define INVERSE_FUNCTION \
  tmp = (2.3133934121925238568526225625332412677664951441251*x-1.3133934121925238568526225625332412677664951441252) / (1.4135354813165148978836634208680817592903532035049*x+0.37061379021381916470784409356507651638130847769908e-1*y-.45059686033789681435444783022458941092848405127480); \
  y = (2.3133934121925238568526225625332412677664951441251*x-1.3133934121925238568526225625332412677664951441252) / (-3.0073004934166310778766243828166588357677518621658*x+4.0073004934166310778766243828166588357677518621659); \
  x = tmp;

#define FUNCTION2 \
  den = .59558512923808195284422700158892281943234575781578*x+.40441487076191804715577299841107718056765424218422*x*y; \
tmp = 1.2393024395737379587169359617792460276172936359160*x-.23930243957373795871693596177924602761729363591605*x*y; \
  y = (-.30633361931763964905243523088414230539388077259881*y-.30427280163792700612473214007623668904379618280477*x+1.6106064209555666551771673709603789944376769554038*x*y)/den; \
  x = tmp/den;

const double fpx = 1.4600925535825834821109097648029698044289932159347,
  fpy = -5.3810696877473602181263512453700696900042653104406;

const double fp2x = 1, fp2y = 1;

const char* NAME = "Kyounghee's birational map";
const char* CHILD = "";
//const char* COLORMAP = "default.map";
const char* COLORMAP = "dim2.map";

const double XL = -5;
const double YT = 5;
const double XR = 5;
const double YB = -5;

const int MAXITER = 100;
const int MAXORBIT = 30;
const int N = 1;
const Parameter PARAM;
/*
double VALUE[N] = { 0 };
const Parameter PARAM(N, VALUE);
const char* PARAMDESC[N] = { "0=fwd, 1=bkwd (for orbit)" };
*/

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double x=z.x;
  register double y=z.y;
  y *= 20;
  double nor, tmp, tmp2, den;
    
  register int i, j;

  for ( i=0; i<max; i++ ) {
    FUNCTION;
    tmp = x-fpx; tmp2 = y-fpy;
    nor = tmp*tmp+tmp2*tmp2;
    if (nor < 0.01)
      break; // return i;
  }
  
  x=z.x; y=z.y;

  y *= 20;

  for ( j=0; j<max; j++ ) {
    INVERSE_FUNCTION;
    tmp = x-fp2x; tmp2 = y-fp2y;
    nor = tmp*tmp+tmp2*tmp2;
    if (nor < 0.01)
      break;
  }
  
  return (i*16)+j;
  //return j;
  //return -1;
}

Point map(Point z, Point c, Parameter param)
{
  register double x=z.x;
  register double y=z.y;
  y *= 20;
  double nor = x*x+y*y, tmp, tmp2, den;
  
  FUNCTION;

  y /= 20;
  return Point(x,y);
}

Point init(Point c, Parameter param)
{
  //return c;
  // compute backward orbit in order to draw both fwd/bkwd orbits
  double x=c.x, y=c.y, tmp;
  y *= 20;
  for (int i=0; i<15; i++) {
    INVERSE_FUNCTION;
  }
  y /= 20;
  return Point(x,y);
}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				  PARAM, CHILD, COLORMAP);
				  //PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
