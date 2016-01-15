/*
 * The domain of the renormalization
 * for Q(z) = z(1+1/z)^6/(1-1/z)^4)
 */

#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

#include "../plugin.h"
using namespace QFract;

const char* NAME = "e^{2\\pi i alpha}Q(z-b)";
const char* CHILD = "";
const char* COLORMAP = "per2.map";

const double XL = -50;
const double YT = 50;
const double XR = 50;
const double YB = -50;

const int MAXITER = 500;
const int MAXORBIT = 2500;

// Parameters
// 0: eta (for the renormalization)
// 1: eta (for the original map)
// 2: arg(b)/2\pi
// 3: |b|
// 4: |alpha|
// 5: arg(alpha)/(2*pi)
// 6: stop iteration when in E? (bool)
// 7: steps for forward orbit (integer, iterate once if =<1)
// 8: stop iteration when 
const int N = 9;
double VALUE[N] = {2.0, 2.0, 0.0, 0.0,
		   0.0, //1.0/(50.0-1.0/50.0),
		   0.0,
		   1.0, 50.0, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "eta", "eta1", "arg(b)/(2*pi)", "|b|",
			     "|alpha|", "arg(alpha)/(2*pi)",
			     "isE (0 or 1)", "steps for fwd orbit",
			     "stop when im < -eta (0 or 1)" };

static complex<double> cv, cv2, alpha, lambda, slope, fp, b,
    logcv, logcv2;

complex<double> map( complex<double> x)
{
  complex<double> y;
  complex<double> t;
  
  x -= b;
  t = x + 1.0;
  t *= t; y = t*t;
  y *= t;
  
  t = x - 1.0;
  t *= t; t *= t;
  y /= x * t ;
  y *= lambda;
  return y;
}

const double CRIT = 5+sqrt(24);
const complex<double> I(0,1);

complex<double> diff(complex<double> z)
{
  complex<double> tmp, tmp2;
  z -= b;
  tmp=(z+1.0)/(z-1.0);
  tmp2 = tmp*tmp;
  tmp2 *= tmp2;
  tmp2 *= tmp;
  
  return lambda*tmp*(z*(z-10.0)+1.0)/(z*z);
}

complex<double> fixpt()
{
  complex<double> p=(10.0-b)/(2*M_PI*I*alpha), d=1.0;
  int i=0;
  do {
    d = (map(p)-p)/(diff(p)-1.0);
    p -= d;
    i++;
  } while ( (i < 100) && (norm(d)>0.0001) );
  return p;
}

extern "C" {
void paraminit(Parameter param)
{
  b = param.Value(3)*exp(2*M_PI*complex<double>(0,param.Value(2)));
  cv=CRIT+b;
  alpha = param.Value(4)*exp(2*M_PI*I*param.Value(5));
  int a1 = (int) real(1.0/alpha);
  lambda=exp(-2*M_PI*I*alpha);
  cout << "alpha = " << alpha << endl
       << "lambda = " << lambda << endl;
  int n;
  if (alpha != 0.0) 
    n=a1/2;
  else
    n=20;
  for (int i=0; i<n; i++)
    cv = map(cv);
  cv2 =map(cv);
  if (alpha != 0.0) {
    fp = fixpt();
    cout << "fix pt = " << fp << endl;
  }
  slope = 1.0/(cv2-cv);
  
  cout << "b = " << b << endl;
  
  logcv = log(cv-fp);
  logcv2 = log(cv2-fp);
}

int iter(Point z, Parameter param, int max)
{
  complex<double> x(z.x,z.y);
  if (alpha == 0.0) {
    x = (10.0-b)*(log(x)/(2.0*M_PI*I)-250.0)+(M_PI*49)/(10.0-b)*I;
  } else {
    // alpha != 0
    x = log(x)/(2.0*M_PI*I)+2.0;
    x = exp((logcv2-logcv)*x+logcv)+fp;
  }

  complex<double> w=(log(x-fp)-logcv)/(logcv2-logcv);
  if ( ( (alpha != 0.0) && 
	 ( real(w) > 0 ) && ( real(w) <= 1) )
       || (alpha == 0.0) && ( real((x-cv)*slope) > 0 ) ) {
    if (param.Value(0) > 0)
      if ( imag(w) < - param.Value(0)) {
	if (!param.Value(8))
	  return 100;
	else
	  goto a;
      } else if ( imag(w) > param.Value(0)) 
	return 101;
    if ( imag(w) < 0 ) {
      if (!param.Value(8))
	return 0;
    } else
      return 1;
  }
  
 a:
  double el=exp(2*M_PI*param.Value(1));
  double y=abs(map(x));
  
  if ( (y < 27.0/el) || ((abs(x) < 27.0*el) && (y > 27.0*el)) )
    return -1;
  
  register int i;
  
  for ( i=0; i<max; i++ ) {
    if ( (param.Value(6)) && 
	 ((real(x-b)+0.18)*(real(x-b)+0.18)/(1.24*1.24) 
	  + imag(x-b)*imag(x-b)/(1.04*1.04) < 1) )
      return -1;
    x = map(x);
    if (alpha != 0.0) {
      w = (log(x-fp)-logcv)/(logcv2-logcv);
      if ( ( real(w) > 0 ) && ( real(w) <= 1) )
	break;
    } else {
      // alpha = 0
      w=(x-cv)*slope;
     if ( real(w) > 0 )
      break;
    }
  }
  
  if (i<max) {
    if (param.Value(0) > 0)
      if ( imag(w) < -param.Value(0))
	return (i % 2)*20+101+i-20;
      else if ( imag(w) > param.Value(0))
	return (i % 2)*20+100+i-20;
    if ( imag(w) < 0 )
      return (((i % 2)*50+1+i-20+256) % 256);
    else
      return (((i % 2)*50+i-20+256) % 256);
  } else 
    return -1;
}

Point map(Point z, Point, Parameter param)
{
  complex<double> x(z.x,z.y);
  complex<double> a(param.Value(2), param.Value(3));
  x = map(x);
  
  if (param.Value(7))
    for (int i=1; i<param.Value(7); i++)
      x=map(x);
  
  return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
    PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				   PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
