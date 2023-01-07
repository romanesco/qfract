/*
 * $Id: pixmap.h,v 1.6 2002/07/17 07:29:59 inou Exp $
 */

#ifndef PIXMAP_H
#define PIXMAP_H

#include <boost/thread.hpp>

#include <qimage.h>
#include <qpoint.h>
#include <qobject.h>

#include "double.h"
#include "colormap.h"
#include "config.h"

using namespace boost;

namespace QFract {
class Canvas;

//class Pixmap //: public QPixmap
class Pixmap : public QObject
{
  Q_OBJECT
public:
  Pixmap( Canvas* c, int w, int h, 
	    double xl, double yt, double xr, double yb, int m,
	  int (*f)( Point, Parameter, int ), void (*pi)( Parameter ),
	  Parameter p );
  Pixmap( const Pixmap& d );
  
  int paramNum() { return param.Num(); }
  void setRegion(double xl, double yt, double xr, double yb)
  { region.setValue(xl,yt,xr,yb); setDiff(); }
  void setRegion(Point tl, Point br) { region.setValue(tl, br); setDiff(); }
  void setFunc( int (*f)(Point, Parameter, int ) ) { iter=f; }
  void setParamInitFunc( void (*pi)( Parameter ) ) { paraminit=pi; }
  void setRegion( Rect r ) { region=r; setDiff();}
  void setParamNum( int n ) { param.setNum(n);}
  void setParam( Parameter p )
  { param.setNum(p.Num()); param.set(p);
  if (paraminit!=NULL) paraminit(param); }
  void setMaxIter( int m ) { max=m; }
  void loadColorMap( const char *filename ) { colormap.open(filename); }
  void setColorMap( ColorMap c ) { colormap=c; }
  
  int maxIter() { return max; }
  Rect Region() { return region; }
  ColorMap colorMap() { return colormap; }
  int width() { return image->width(); }
  int height() { return image->height(); }
  double dWidth() { return region.width(); }
  double dHeight() { return region.height(); }
  
  Point getPoint(int ix, int iy);
  QPoint getPos(Point z);
  void draw(); 
  void do_draw(int xs, int xe, int ys, int ye, int j); 
  void progress(int max);
  void resize( int w, int h, bool redrawEnabled=true );
  QImage* getImage() { return image; }
  
signals:
  void haveDrawn();

private:
  QImage *image;
  Rect region;
  double diff;
  int max; // max iteration time
  Parameter param;
  int (*iter)(Point, Parameter , int); // calculation function
  void (*paraminit)( Parameter );
  ColorMap colormap;

  Canvas* parent;

  void setDiff();

  int p[NUM_THREADS];
  thread_group thr_grp;
  boost::mutex mtx_;
};
}
#endif // PIXMAP_H
