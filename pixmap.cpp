/*
 * $Id: pixmap.cpp,v 1.5 2002/07/17 07:29:59 inou Exp $
 */

#include "pixmap.h"
#include "colormap.h"
#include "canvas.h"
using namespace QFract;

// for sleep
//#include <unistd.h>

// for debug
#include <iostream>
using namespace std;
//#include <sys/time.h>

#include <QImage>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QColor>
#include <QWidget>
#include <QMainWindow>
#include <QStatusBar>
#include <QApplication>
#include <QProgressBar>
//
using namespace Qt;

#include <boost/thread.hpp>
#include <boost/bind.hpp>
using namespace boost;

Pixmap::Pixmap( Canvas* c, int w, int h, 
		double xl, double yt, double xr, double yb, 
		int m, int (*f)( Point, Parameter, int ),
		void (*pi)( Parameter ), Parameter p)
    : colormap()
{
  parent=c;
  image = new QImage( w, h, QImage::Format_RGB32 );
  region.setValue(xl,yt,xr,yb); max=m; iter=f; param.set(p); paraminit=pi;
  setDiff(); if (paraminit!=NULL) paraminit(param);
}

Pixmap::Pixmap( const Pixmap& d )
{
  parent=d.parent; region=d.region; iter = d.iter; param.set(d.param); 
  paraminit = d.paraminit; diff=d.diff;
  image = new QImage( *d.image );
 if (paraminit!=NULL) paraminit(param);
}

Point Pixmap::getPoint(int ix, int iy)
{ return Point( region.topLeft()+Point(diff*ix, -diff*iy) ); }

void Pixmap::draw()
{
  //thr_grp.join_all();
  image->fill( black );
  
  /*
  struct timeval tv1,tv2;
  gettimeofday(&tv1, NULL);
  int stime=tv1.tv_sec*1000000+tv1.tv_usec;
  */

  thread_group thr_grp;
  //int p[NUM_THREADS];
  for (int iy=0; iy<YDIV; iy++) {
    int ys = iy*height()/YDIV, ye = (iy+1)*height()/YDIV;
    for (int ix=0; ix<XDIV; ix++) {
      int xs = ix*width()/XDIV, xe = (ix+1)*width()/XDIV;
      thr_grp.create_thread(bind(&Pixmap::do_draw, this,
				 xs, xe, ys, ye, ix*YDIV+iy));
    }
  }

  //boost::thread thr(bind(&Pixmap::progress, this, width()*height()));
  thr_grp.join_all();
  
  do_draw(0, width(), 0, height(), 0);
	  
  /*
  int i, k=1;
  for (int ix=0; ix<width(); ix++) {
    for (int iy=0; iy<height(); iy++) {
      Point z=getPoint(ix,iy);
      i=iter( z, param, max );
      if (i>=0) {
	image->setPixel( ix, iy, colormap.Rgb(i%NUM) );
      }
    }
    if ( (ix & 3) == 0 ) {
	gettimeofday(&tv2, NULL);
	int etime = tv2.tv_sec*1000000+tv2.tv_usec-stime;
	if ( (etime / 200000) >= k ) {
	    parent->setStatusBar( qApp->translate("QFract::Pixmap", "%1% done.").arg((ix*100)/width()) );
	    k = etime / 200000 +1;
	}
    }
  }
  */

  /*
    gettimeofday(&tv2, NULL);
  cerr << "pixmap::draw(): "
       << tv2.tv_sec*1000000+tv2.tv_usec-stime << endl;
  */
  //parent->setStatusBar(qApp->translate("QFract::Pixmap", "Done."));
}

void Pixmap::do_draw(int xs, int xe, int ys, int ye, int j)
{
  int i;
  //p[j]=0;
  for (int iy=ys; iy<ye; iy++) {
    for (int ix=xs; ix<xe; ix++) {
      Point z=getPoint(ix,iy);
      i=iter( z, param, max );
      if (i>=0) {
	//mtx_.lock();
	image->setPixel( ix, iy, colormap.Rgb(i%NUM) );
	//mtx_.unlock();
      } else if (i == -2) {
	//mtx_.lock();
	//image->setPixel( ix, iy, Qt::white);
	image->setPixel( ix, iy, qRgb(192,192,192));
	//mtx_.unlock();
      }
      //p[j]++;
    }
  }
}

void Pixmap::progress(int max)
{
  cerr << "Pixmap::progress called." << endl;
  int n=0;
  while(n<max) {
    n=0;
    for (int i=0; i<NUM_THREADS; i++) {
      n += p[i];
    }
    cerr << n << "/" << max << endl;
     parent->setStatusBar( qApp->translate("QFract::Pixmap", "%1% done.").arg((n*100)/max) );
    boost::this_thread::sleep(boost::posix_time::seconds(1)); 
  }
  parent->setStatusBar(qApp->translate("QFract::Pixmap", "Done."));
  cerr << "Done." << endl;
  emit( haveDrawn() );
}

void Pixmap::resize( int w, int h, bool redrawEnabled )
{ 
  if ( (w == width()) && (h == height()) )
    return;

  if (image) delete image;
  image = new QImage( w, h, QImage::Format_RGB32 );
  setDiff();
  if (redrawEnabled) draw();
}

QPoint Pixmap::getPos(Point z)
{
  int ix= (int) ((z.x-region.xleft)/diff);
  int iy= (int) ((z.y-region.ytop)/(-diff));
  return QPoint(ix,iy);
}

void Pixmap::setDiff()
{
  double xd = region.width()/width();
  double yd = region.height()/height();
  diff =  (xd > yd) ? xd : yd;
}
