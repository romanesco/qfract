/*
 * Q (Fatou coordinate for renormalization)
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <iomanip>
using namespace std;

#include "../plugin.h"
using namespace QFract;

const char* NAME = "e^{2\\pi i alpha}Q(z-b)";
const char* CHILD = "";
const char* COLORMAP = "per2-2.map";

/*
const double XL = -110;
const double YT = 40;
const double XR = 110;
const double YB = -180;
*/
const double XL = -21;
const double YT = -163;
const double XR = -1;
const double YB = -184;

const int MAXITER = 1;
const int MAXORBIT = 2500;

// Parameters
// 0: eta (for the original map)
// 1: eta (for the renormalization)
// 2: eta (for the second renormalization
// 3: a1 
// 4: a2 (alpha=1/(a1-1/a2))
// 5: stop iteration when in E? (bool)
// 6: steps for forward orbit (integer, iterate once if =<1)
// 7: do not stop iteration when im <-2 in Fatou coord
const int N=8;
double VALUE[N] = {2.0, 2.0, 2.0, 
		   50, 50,
		   1.0, 50, 0.0};
const Parameter PARAM( N, VALUE );
const char* PARAMDESC[N] = { "eta", "eta1", "eta2", "a1", "a2",
			     "isE (0 or 1)", "steps for fwd orbit",
			     "continue iteration when im < -eta (0 or 1)" };

const double CRIT = 5+sqrt(24);
const complex<double> I(0,1);

static double alpha;
static complex<double> cv, cv2, cv3, cv4, lambda, slope, fp, pp,
    cv3ren, cv4ren, ppren, b(0);
static int a1, a2;

bool inFundDom(complex<double> z)
{
    return ( ( imag(z/(cv-fp)) < imag(cv/(cv-fp)) ) &&
	     ( imag(z/(cv2-fp)) >= imag(cv2/(cv2-fp)) ) );
}

complex<double> renCoord(complex<double> z)
{
    complex<double> x = (log(z-fp) - log(cv-fp))/(log(cv2-fp)-log(cv-fp));
    return exp(-2*M_PI*I*x);
}

bool inFundDom2(complex<double> z)
{
    complex<double> w = renCoord(z);
    complex<double> y = (log(w-ppren) - log(cv3ren-ppren))
	/ (log(cv4ren-ppren) - log(cv3ren-ppren));
    return ( (real(y) > 0) && (real(y) <= 1.05) );
    /*
    return ( ( imag(w/(cv3ren-ppren)) > imag(cv3ren/cv3ren-ppren) ) &&
	     ( imag(w/(cv4ren-ppren)) <= imag(cv4ren/cv4ren-ppren) ) );
    */
}

complex<double> Q( complex<double> x)
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
    complex<double> p=(10.0-b)/(2*M_PI*alpha), d=1.0;
    int i=0;
    do {
	d = (Q(p)-p)/(diff(p)-1.0);
	p -= d;
	i++;
    } while ( (i < 100) && (norm(d)>0.0001) );
    return p;
}

complex<double> perpt(int N, complex<double> p0)
{
    complex<double> p=p0, d=1.0, z;
    int i=0;
    do {
	z = p;
	d = 1.0;
	for (int j=0; j<N; j++) {
	    d *= diff(z);
	    z = Q(z);
	}
	d = (z-p)/(d-1.0);
	p -= d;
	i++;
    } while (i < 100);
    return p;
}

extern "C" {
void paraminit(Parameter param)
{
    b=0;
    cv=CRIT;
    alpha = 1.0/(param.Value(3)-1.0/param.Value(4));
    a1 = (int) param.Value(3);
    a2 = (int) param.Value(4);
    lambda=exp(-2*M_PI*I*alpha);
    int n;
    cout << setprecision(20) << endl;
    if (alpha != 0.0) 
	n=abs(a1)/2-2;
    else
        n=20;
    for (int i=0; i<n; i++)
	cv = Q(cv);
    for (int i=0; i<abs(a1); i++)
	cv2 =Q(cv);
    if (alpha != 0.0) {
	fp = fixpt();
	cout << "fix pt = " << fp << endl;
    }
    cout << "cv = " << cv << endl;
    cout << "cv2 = " << cv2 << endl;

    //complex<double> x = (log(z-fp) - log(cv-fp))/(log(cv2-fp)-log(cv-fp));
    // return exp(2*M_PI*I*x);
    //pp = log(((double) a2)*I)/(2*M_PI*I);
    pp = 0;
    pp = pp*(log(cv2-fp)-log(cv-fp))+log(cv-fp);
    pp = exp(pp)+fp;
    pp = perpt(a1,pp);
    ppren = renCoord(pp);
    while (!inFundDom(pp))
    {
	pp = Q(pp);
    }

    cout << "periodic pt = " << pp << endl;
    
    //if (alpha != 0.0) 
    n=abs(a1*a2)/2;

    cv3=cv;
    for (int i=0; i<n; i++)
	cv3 = Q(cv3);
    if (alpha != 0.0) {
	while (!inFundDom(cv3))
	{
	    cout << ".";
	    cv3 = Q(cv3);
	}
    }
    cv4=cv3;
    for (int i=0; i<abs(a1); i++)
	cv4 =Q(cv4);
    
    cv3ren = renCoord(cv3);
    cv4ren = renCoord(cv4);
    
    slope = 1.0/(cv4ren-cv3ren);

    cout << "cv3 = " << cv3 << endl;
    cout << "cv4 = " << cv4 << endl;
    cout << "ppren = " << ppren << endl;
    cout << "cv3ren = " << cv3ren << endl;
    cout << "cv4ren = " << cv4ren << endl;
    cout << (cv4ren-ppren)/(cv3ren-ppren) << endl;
    cout << "lambda = " << lambda << endl;
    //cout << "b = " << b << endl;
}

int iter(Point z, Parameter param, int max)
{
    complex<double> x(z.x,z.y);
    
    double el=exp(2*M_PI*param.Value(1));
    double y=abs(Q(x));

    if ( (y < 27.0/el) || ((abs(x) < 27.0*el) && (y > 27.0*el)) )
	return -1;

    register int i;

    for ( i=0; i<max; i++ ) {
	// check if x lies in E
	if ( (param.Value(5)) && 
	     ((real(x-b)+0.18)*(real(x-b)+0.18)/(1.24*1.24) 
	      + imag(x-b)*imag(x-b)/(1.04*1.04) < 1) )
	    return -1;
	// check if x lies in fund dom of attr Fatou coord
	if (alpha != 0.0) {
	    if ( inFundDom(x) && inFundDom2(x) &&
		 //( norm(renCoord(x)-ppren) < 10000 ) )
		(!(param.Value(7)) || (imag((renCoord(x)-cv3ren)*slope) >= -param.Value(0))))
		break;
	} else if ( real((x-cv)*slope) > 0 )
		break;
	x = Q(x);
    }
    
    if (i<max) {
	complex<double> w = renCoord(x);
	i /= a1;
	if (param.Value(0) > 0)
	    if (imag((w-cv3ren)*slope) < -param.Value(0))
		return (i % 2)*20+101+i-20;
	    else if (imag((w-cv3ren)*slope) > param.Value(0))
		return (i % 2)*20+100+i-20;
	if ( imag((w-cv3ren)*slope) < 0 )
	    return (((i % 2)*50+1+i-20+256) % 256);
	
	else
	    return (((i % 2)*50+i-20+256) % 256);
    }
    else
	return -1;
}

Point map(Point z, Point, Parameter param)
{
    complex<double> x(z.x,z.y);
    //complex<double> a(param.Value(2), param.Value(3));
    x = Q(x);
    
    if (param.Value(6))
	for (int i=1; i<param.Value(6); i++)
	    x=Q(x);

    return Point(real(x),imag(x));
}

Point init(Point c, Parameter param) 
{
    return c;
}

PluginInfo* getInfo()
{
    PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				   PARAM, CHILD, COLORMAP, PARAMDESC);
  return i;
}
}
