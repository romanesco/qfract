/*
 * Calculating Weierstrass p-function
 * 
 * Algorithm by Coquereaux, Grossmann, Lautrup,
 * Iterative method for calculation of the Weierstrass elliptic function
 * (IMA Journal of Numerical Analysis (1990) 10, 119-128.
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <iomanip>

//#define DEBUG 1
//#define ALTROOT 1


using namespace std;

const complex<double> I(0,1), omega=exp(2.0*M_PI*I/3.0);
const int MAXITER_FOR_K=8;
const int MAXITER_FOR_P=5;
const int MAXITER_FOR_DP=5;

// return cubic root of z with 0 <= arg < pi.
complex<double> altsqrt(complex<double> z)
{
#ifdef ALTROOT
    return conj(sqrt(conj(-z)))*I;
#else
    return sqrt(z);
#endif
}

// return cubic root of z with 0 <= arg < 2*pi/3.
complex<double> cbrt(complex<double> z)
{
#ifdef ALTROOT
    return conj(pow(conj(-z),1.0/3.0))*omega;
#else
    return pow(z,1.0/3.0);
#endif
}

// compute K(m)=\int_0^{\pi/2} 1/\sqrt{1-m\sin^2 t} dt
complex<double> CompEllipticInt1st(complex<double> m)
{
    complex<double> a=1.0, b=sqrt(complex<double>(1.0)-m), c;
#ifdef DEBUG
    cout << "m = " << m << endl;
    cout << "b0 = " << b << endl;
#endif
    for (int i=0; i<MAXITER_FOR_K; i++) {
	c = (a+b)/2.0; b = sqrt(a*b);
	a=c;
	if (a == b) break;
    }
    a = M_PI/(2.0*a);
#ifdef DEBUG
    cout << "K" << m << " = " << a << endl;
#endif
    return a;
}

class WeierstrassP
{
    complex<double> g2, g3, e1, e2, e3, A, B, Delta;
    
    complex<double> setDelta();
    void sete1e2e3();
    void setPeriods();

    // truncated P and dP at z=0 (first three terms)
    complex<double> P_trunc(complex<double> z);
    complex<double> dP_trunc(complex<double> z);
    // duplication formula
    // calculate P(2z) when y=P(z)
    complex<double> P_dup(complex<double> y);
    // calculate (P'(z))^2 when y=P(z)
    complex<double> dP2(complex<double> y);
    // calculate and P'(2z) when y=P(z) and w=P'(z)
    complex<double> dP_dup(complex<double> y, complex<double> w);
public:
    WeierstrassP(complex<double> a, complex<double> b);
    complex<double> P(complex<double> z);
    complex<double> dP(complex<double> z);

    complex<double> getG2() { return g2; }
    complex<double> getG3() { return g3; }
    complex<double> getDelta() { return Delta; }
    complex<double> getOmega1() { return A; }
    complex<double> getOmega2() { return B; }
    complex<double> getE1() { return e1; }
    complex<double> getE2() { return e2; }
    complex<double> getE3() { return e3; }

};

WeierstrassP::WeierstrassP(complex<double> a, complex<double> b)
{
    g2 = a; g3= b;
    setDelta();
    sete1e2e3();
    setPeriods();
}

complex<double> WeierstrassP::setDelta()
{
    Delta = g3*g3-g2*g2*g2/27.0;
#ifdef DEBUG
    cout << "Delta = " << Delta << endl;
#endif
    return Delta;
}

// Delta should be set before calling sete1e2e3
void WeierstrassP::sete1e2e3()
{
    complex<double> alpha, beta, gamma;
    if (g3 != 0.0) {
	if (g2 != 0.0) {
	    alpha = g2/12.0;
	    alpha = altsqrt(alpha);
	    beta = cbrt(g3+altsqrt(Delta))/(2.0*alpha);
	    gamma = 1.0/beta;
	    
	    e1 = alpha*(beta+gamma);
	    e2 = alpha*(beta*omega+gamma/omega);
	    e3 = alpha*(beta/omega+gamma*omega);
	} else {
	    e1 = cbrt(g3/4.0);
	    e2 = omega*e1;
	    e3 = omega*e2;
	}
    } else {
	e1 = altsqrt(g2)/2.0;
	e2 = e1;
	e3 = 0;
    }

#ifdef DEBUG
    cout << "e1 = " << e1 << endl;
    cout << "e2 = " << e2 << endl;
    cout << "e3 = " << e3 << endl;
    cout << "e1+e2+e3 = " << e1+e2+e3 << endl;
    cout << "e1e2+e2e3+e3e1 = " << e1*e2+e2*e3+e3*e1 << endl;
    cout << "e1e2e3 = " << e1*e2*e3 << endl;
#endif
}

void WeierstrassP::setPeriods()
{
    complex<double> m=(e2-e3)/(e1-e3), a;
    a = CompEllipticInt1st(m);
    A = 2.0/(altsqrt(e1-e3)) * a;
    a = CompEllipticInt1st(complex<double>(1.0)-m);
    B = 2.0*I/(altsqrt(e1-e3)) * a;

#ifdef DEBUG
    cout << "m = " << m << endl;
    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "A+B = " << A+B << endl;
    a = P(A);
    cout << "P(A) = " << a << endl;
    a = P(B);
    cout << "P(B) = " << a << endl;
#endif
}

complex<double> WeierstrassP::P_trunc(complex<double> z)
{
    z *= z;
    return ( 1.0/z + z*(g2/20.0 + g3/28.0*z) );
}

complex<double> WeierstrassP::dP_trunc(complex<double> z)
{
    complex<double> z2=z*z;
    return ( -2.0/(z2*z) + z*(g2/10.0 + g3/7.0*z2) );
}

complex<double> WeierstrassP::dP2(complex<double> y)
{
    return ( (4.0*y*y - g2)*y - g3 );
}

complex<double> WeierstrassP::P_dup(complex<double> y)
{
    complex<double> p, q;
    p = 6.0*y*y-g2/2.0;
    p *= p;
    q = 4.0*( (y*y*4.0-g2)*y-g3 );

    return -2.0*y+p/q;
}

complex<double> WeierstrassP::dP_dup(complex<double> y, complex<double> w)
{
    complex<double> p,q;
    // p = 8y^6 - 10g2p^4 - 40g3p^3 - 5/2g2^2p^2 - 2g2g3p + 1/8g2^3-4g3^2
    p = ( (8.0*y*y - 10.0*g2)*y - 40.0*g3)*y;
    p = ((p - 5.0/2.0*g2*g2)*y - 2.0*g2*g3)*y + 1.0/8.0*g2*g2*g2-4.0*g3*g3;
    q = 4.0*w*w*w;
    return p/q;
}
complex<double> WeierstrassP::P(complex<double> z)
{
    double u,v,a, b, c, d, det;
    // translate z to the fundamental domain
    a = real(A); b = real(B);
    c = imag(A); d = imag(B);
    det = a*d-b*c;
    // z = [a,b;c,d](u,v)
    u = (d * real(z) - b * imag(z))/det;
    v = (-c * real(z) + a * imag(z))/det;
    // z = [a,b;c,d](u - (int) u, v - (int) v) 
    //   = z - (((int) u)*A + ((int) v)*B)
   z -= ((double)( (int) u))*A + ((double)( (int) v))*B;
#ifdef DEBUG
    cout << "u, (int) u = " << u << ", " << (int) u << endl;
    cout << "v, (int) v = " << v << ", " << (int) v << endl;
    cout << z << endl;
#endif
    int i;
    for (i=0; i<MAXITER_FOR_P; i++)
	z /= 2.0;
    
    z = P_trunc(z);
    for (i=0; i<MAXITER_FOR_P; i++)
	z = P_dup(z);
    
    return z;
}

complex<double> WeierstrassP::dP(complex<double> z)
{
    double u,v,a, b, c, d, det;
    // translate z to the fundamental domain
    a = real(A); b = real(B);
    c = imag(A); d = imag(B);
    det = a*d-b*c;
    // z = [a,b;c,d](u,v)
    u = (d * real(z) - b * imag(z))/det;
    v = (-c * real(z) + a * imag(z))/det;
    // z = [a,b;c,d](u - (int) u, v - (int) v) 
    //   = z - (((int) u)*A + ((int) v)*B)
    z -= ((double)( (int) u))*A + ((double)( (int) v))*B;

    int i;
    for (i=0; i<MAXITER_FOR_DP; i++)
	z /= 2.0;
    
    complex<double> dz = dP_trunc(z);
    z = P_trunc(z);
    
#ifdef DEBUG
    cout << "z = " << z << endl;
    cout << "dz = " << dz << endl;
    cout << "error = " << dz*dz-dP2(z) << endl;
#endif
    for (i=0; i<MAXITER_FOR_DP; i++) {
	dz = dP_dup(z,dz);
	z = P_dup(z);
#ifdef DEBUG
	cout << "z = " << z << endl;
	cout << "dz = " << dz << endl;
	cout << "error = " << dz*dz-dP2(z) << endl;
#endif
    }
    
    z = sqrt(dP2(z));
    if (real(dz/z) < 0)
	z = -z;
    return z;
}

#ifndef NEED_LIBWEIERSTRASSP
int main()
{
    cout << setprecision(20);
    complex<double> g2=-20.0/3.0, g3=-44.0/27.0;
    //complex<double> g2=0.0, g3=1.0;
    //complex<double> g2=10.0, g3=2.0;
    //complex<double> g2(1,1), g3(2,-3);

    WeierstrassP P(g2,g3);

    complex<double> a, c;
    complex<double> z=0.5;
    complex<double> p=P.P(z), dp=P.dP(z);
    
    complex<double> omega1=P.getOmega1(), omega2=P.getOmega2();
    cout << "omega1 = " << omega1 << endl;
    cout << "omega2 = " << omega2 << endl << endl;

    cout << "z = " << z << endl;
    cout << "P(z) = " << p << endl;
    cout << "dP(z) = " << dp << endl;

    cout << "P(z)-P(z+omega1) = " << p - P.P(z+omega1) << endl;
    cout << "P(z)-P(z+omega2) = " << p - P.P(z+omega2) << endl;
    cout << "dP(z)-dP(z+omega1) = " << p - P.P(z+omega1) << endl;
    cout << "dP(z)-dP(z+omega2) = " << p - P.P(z+omega2) << endl;


    a = -3.0*I*dp/( 2.0*(3.0*p+2.0) );
    c = p-1.0/3.0;
    c = 1.0+1.0/c;
    cout << "a = " << a << endl; 
    cout << "c = " << c << endl;
    cout << a*a+( (c-1.0)*c*c+1.0 )/( (c-1.0)*c*c ) << endl;
}
#endif // NEED_LIBWEIERSTRASSP
