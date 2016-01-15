/*
 * $Id: double.h,v 1.6 2002/07/17 07:29:59 inou Exp $
 */

#ifndef DOUBLE_H
#define DOUBLE_H

#include <iostream>

namespace QFract {

class Point
{
public:
  Point(double z=0, double w=0) { x=z; y=w; }
  
  Point operator+(Point p) { return Point( x+p.x, y+p.y ); }
  Point operator-(Point p) { return Point( x-p.x, y-p.y ); }
  Point multiply(int i) { return Point(x*i, y*i); }
  Point multiply(double i) { return Point(x*i, y*i); }
  Point multiply(int ix, int iy) { return Point(x*ix, y*iy); }
  double abs2() { return x*x+y*y; }
  
  double x, y;
};

inline void minmax(double *a, double *b)
{
  if (*a>*b) { double t=*a; *a=*b; *b=t; }
}

class Rect
{
public:
  Rect(double xl=-2.0, double yt=2.0, double xr=2.0, double yb=-2.0)
  { xleft=xl; ytop=yt; xright=xr; ybottom=yb; minmax();}
  Rect(Point tl, Point br)
  { xleft=tl.x; ytop=tl.y; xright=br.x; ybottom=br.y; minmax();}
  
  void minmax() 
  { QFract::minmax(&xleft, &xright); QFract::minmax (&ybottom, &ytop); }
  
  void setValue(double xl=-2.0, double yt=2.0, double xr=2.0, double yb=-2.0)
  { xleft=xl; ytop=yt; xright=xr; ybottom=yb; minmax();}
  void setValue(Point tl, Point br)
  { xleft=tl.x; ytop=tl.y; xright=br.x; ybottom=br.y; minmax();}

  Point topLeft() { return Point(xleft, ytop); }
  Point bottomRight() { return Point(xright, ybottom); }
  
  double width() { return xright-xleft; }
  double height() { return ytop-ybottom; }
  
  double xleft, ytop, xright, ybottom;
};

class Parameter
{
public:
  Parameter( int n=0 ) { num=n; value=new double[n]; }
  Parameter( int n, double *v ){ num=n; value=new double[n]; set( n, v ); }
  Parameter( Parameter& p )
  { num=p.num; value=new double[p.num];set( p.num, p.value); }
  Parameter( const Parameter& p )
  { num=p.num; value=new double[p.num];set( p.num, p.value); }
  ~Parameter() {if (value != NULL) delete value; value=NULL;}
  
  int Num() {return num;}
  double Value(int i) {if (i<num) return value[i]; else return 0.0;}

  void setValue( int n, double v ) { if ( n<num ) value[n]=v; }

  void setNum( int n )
  { if (num<n) {delete value; value = new double[n];} num=n; }
  
  void set( int n, double *v, int s=0 )
  {
    if (num<n+s) {
      double *tmp = new double[n+s];
      for (int i=0; i<s; i++)
	tmp[i]=value[i];
      delete value;
      value=tmp;
      num=n+s;
    }
    for (int i=s; i<n+s; i++)
      value[i]=*v++;
  }
  
  void set( Parameter p, int s=0 ) { set( p.num, p.value, s); }
  void set( Point z , int s=0 ) { set( 1, &z.x, s ); set( 1, &z.y, s+1 ); }
  
private:
  int num;
  double *value;
};
}
#endif // DOUBLE_H
