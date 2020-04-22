/*
 * $Id: canvas.cpp,v 1.8 2002/07/17 07:29:59 inou Exp $
 */

/* TRANSLATOR QFract::Canvas */

#include "pixmap.h"
#include "setparam.h"
#include "canvas.h"
#include "window.h"
using namespace QFract;

// for debug
#include <iostream>
using namespace std;

#include <qmessagebox.h>
//#include <qaccel.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qevent.h>
#include <qrect.h>
#include <qfiledialog.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPrintDialog>
#include <QPrinter>
//
using namespace Qt;
#include <QPainterPath>

Canvas::Canvas( QWidget *parent )
  : QWidget( parent), //, Qt::WA_StaticContents ), // Flag OK?
      pix( this, width(), height(), -2.5, 2, 1.5, -2, 100,
	   NULL, NULL, Parameter(0)),
      maxorbit(10), orbitstep(1),    rect(), 
      redrawEnabled( false ), isOrbit( false ),
      leftButtonPressed( false ), rightButtonPressed( false )
{

  path=NULL;
  param.set(0, (double *) NULL);

#ifndef QT_NO_CURSOR
  setCursor( Qt::CrossCursor );
#endif
  setMouseTracking( true ) ;

  connect(&pix, SIGNAL( haveDrawn() ), this, SLOT( repaint() ) );

  setAttribute(Qt::WA_StaticContents);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void Canvas::resizeEvent( QResizeEvent *e )
{
  QWidget::resizeEvent( e );
  //resize(e->size().width(), e->size().height() );  
  
  //cerr << size().width() << ", " << size().height() << endl;

  pix.resize( e->size().width(), e->size().height(), redrawEnabled );
  update();
}

void Canvas::paintEvent( QPaintEvent *e )
{
  QPainter p( this );
  QPen pen( white );
  p.setPen(pen);
  p.drawImage( 0, 0, *pix.getImage() );
  if ( path != NULL) {
      p.drawPath(*path);
  }
  p.drawRect(rect);
}

// set plugin and region
// (func, iter, param is already set by MainWindow::loadPlugin)
void Canvas::setPlugin( Plugin *p )
{
  plugin=p;
  pix.setRegion(plugin->TopLeft(),plugin->RightBottom());
}

void Canvas::mouseEvent( QMouseEvent *e )
{
  Point z = getPoint( e );
  if ( leftButtonPressed == true ) {
    emit sendParam( z, param );
  }
  
  //repaint(); // clear screen
  
  if (rightButtonPressed == true ) {
    zoomRect( e );
  } 
  if ( isOrbit == true ) {
	  drawOrbit( e );
  }  
  emit _setStatusBar_(QString("( ") +
		    QString::number(z.x, 'f', 10) + ", " +
		    QString::number(z.y, 'f', 10) + " )" );
}

void Canvas::toggleOrbit( bool b )
{
    isOrbit=b;
    if (!b) {
	path=NULL;
    }
    repaint();
}

void Canvas::mousePressEvent( QMouseEvent *e )
{
  if (e->button() == LeftButton)
    leftButtonPressed = true;
  if (e->button() == RightButton) {
    rightButtonPressed = true;
    zoomStart=e->pos();
  }
  mouseEvent( e );
}

void Canvas::mouseReleaseEvent( QMouseEvent *e )
{
  if (e->button() == LeftButton)
    leftButtonPressed = false;
  if (e->button() == RightButton) {
    rightButtonPressed = false;
    zoom(e);
  }
}

void Canvas::mouseMoveEvent( QMouseEvent *e )
{
  mouseEvent( e );
}

Point Canvas::getPoint( QMouseEvent *e )
{
  return pix.getPoint(e->x(), e->y());
}

void Canvas::drawOrbit( QMouseEvent *e )
{
  Point c=getPoint(e);
  Point z=init(c, param);
  
  if (path != NULL) {
      delete path;
  }
  path = new QPainterPath();
  
  path->moveTo(pix.getPos(z));
  
  Point tl=pix.getPoint(0,0), br=pix.getPoint(pix.dWidth()-1,pix.dHeight()-1);
  double xl2=tl.x*tl.x, xr2=br.x*br.x,
         yt2=tl.y*tl.y, yb2=br.y*br.y;
  double xmax2 = (xl2 > xr2) ? xl2 : xr2,
         ymax2 = (yt2 > yb2) ? yt2 : yb2;
  double r=100*(xmax2 + ymax2
                + pix.dWidth()*pix.dWidth() + pix.dHeight()*pix.dHeight()+1);
  
  for (int i=0; i<maxorbit; i++) {
    for (int j=0; j<orbitstep; j++) {
      if (z.abs2() > r) break;
      z = map(z,c,param);
    }
    path->lineTo( pix.getPos(z) );
  }
  repaint();
};

void Canvas::draw()
{
    pix.draw();
    repaint();
}

void Canvas::save()
{
  QString fn =
      QFileDialog::getSaveFileName( this, tr("Save File"),"~/",
				    tr("PNG format (*.png)"));
  if ( fn.isEmpty() )
    return; // aborted
  QFileInfo fi( fn );
  if (fi.suffix() != "png" )
    fn += ".png";
  // image
  QImage *image = pix.getImage();

  // plugin name
  image->setText("PluginName", pluginname);

  // parameters
  image->setText("NumParameters", QString::number(param.Num()));
  for (int i=0; i<param.Num(); i++) {
      image->setText(QString("Parameter%1").arg(i), QString("%1").arg(param.Value(i),0,'g',30));
  }
  // region
  Rect region=pix.Region();
  image->setText("XLeft", QString("%1").arg(region.xleft,0,'g',30));
  image->setText("YTop", QString("%1").arg(region.ytop,0,'g',30));
  image->setText("XRight", QString("%1").arg(region.xright,0,'g',30));
  image->setText("YBottom", QString("%1").arg(region.ybottom,0,'g',30));

  // max iteration times
  image->setText("Iteration", QString("%1").arg(pix.maxIter()));
  image->setText("Orbit", QString("%1").arg(maxorbit));
  image->setText("OrbitStep", QString("%1").arg(orbitstep));

  // colormap
  image->setText("ColorMap", colorMap().Name());

  image->save( fn, "PNG" );
}

void Canvas::zoom(QMouseEvent *e)
{
  Point z1 = pix.getPoint( zoomStart.x(), zoomStart.y() );
  Point z2 = getPoint( e );
  
  MainWindow *win=new MainWindow();
  win->loadPlugin(qPrintable(pluginname));
  win->setRegion(Rect(z1,z2));
  
  win->setMaxOrbit( maxOrbit() );
  win->setOrbitStep( orbitStep() );
  win->setMaxIter( pix.maxIter() );
  
  win->setParam( param );
  win->setColorMap( pix.colorMap() );
  win->show();

  //win->setInitialSize(320,320);
  Rect r(z1,z2);
  if (r.width() >= r.height()) {
      win->setInitialSize(320,320*r.height()/r.width());
  } else {
      win->setInitialSize(320*r.width()/r.height(),320);
  }
  win->setRedrawEnabled(true);
  
  rect = QRect();
  /*
  pix.setRegion(z1, z2);
  draw();
  */
  win->draw();
  update();
}

void Canvas::zoomRect(QMouseEvent *e)
{
    rect = QRect(zoomStart, e->pos());
    update();
}

void Canvas::setStatusBar( QString s )
{
    emit _setStatusBar_(s);
}

void Canvas::print()
{
     QPrinter printer;

     QPrintDialog *dialog = new QPrintDialog(&printer, this);
     dialog->setWindowTitle(tr("Print Picture"));
     if (dialog->exec() != QDialog::Accepted)
         return;

     QPainter painter(&printer);

     /*
     {
	 qreal l, r, t, b;
	 printer.getPageMargins(&l, &t, &r, &b, QPrinter::Millimeter);
	 cerr << l << ", " << r << endl
	      << t << ", " << b << endl;
     }
     */
     QRect pagerect=printer.pageRect();
     QRectF pagerectf=printer.pageRect(QPrinter::Millimeter);
     // millimeters in QPrinter::DevicePixel
     double wunit=pagerect.width()/pagerectf.width();
     double hunit=pagerect.height()/pagerectf.height();

     /*
     int left=pagerect.left(), top=pagerect.top();
     cerr << left << ", " << top << endl;
     */
     int w=pagerect.width(), h=pagerect.height();
     
     int textheight = hunit*80;
     int marginheight = hunit*5;
     int h1 = h-(textheight+marginheight); // max height for image
     int texttop = h - textheight; // top position for text
     
     // plugin name
     painter.drawText(0, texttop, tr("Name:\t%1").arg(pluginname));
     // region
     Rect region=pix.Region();
     painter.drawText(0, texttop + 7*hunit,
		      tr("XLeft:\t%1").arg(region.xleft));
     painter.drawText(100*wunit, texttop + 5*hunit,
		      tr("YTop:\t%1").arg(region.ytop));
     painter.drawText(0, texttop + 12*hunit,
		      tr("XRight:\t%1").arg(region.xright));
     painter.drawText(100*wunit, texttop + 10*hunit,
		      tr("YBottom:\t%1").arg(region.ybottom));

     // max iteration times
     painter.drawText(0, texttop + 19*hunit,
		      tr("Iteration:\t%1").arg(pix.maxIter()));
     painter.drawText(100*wunit, texttop + 19*hunit,
		      tr("Orbit:\t%1").arg(maxorbit));
     painter.drawText(100*wunit, texttop + 19*hunit,
		      tr("Orbit step:\t%1").arg(orbitstep));

     // colormap
     painter.drawText(0, texttop + 26*hunit,
		      tr("ColorMap:\t%1").arg(colorMap().Name()));

     // parameters
     if (param.Num() > 0) {
	 painter.drawText(0, texttop + 33*hunit,
			  tr("Parameters:"));
	 for (int i=0; i<param.Num(); i++) {
	     painter.drawText(100*wunit*(i % 2),
			      texttop + 40*hunit + (i/2)*5*hunit,
			      tr("Parameter %1:\t%2").arg(i).arg(param.Value(i),0,'g',20));
	 }
     } else {
	 painter.drawText(0, texttop + 33*hunit,
			  tr("No parameters"));
     }	 
     //painter.drawImage( target, *pix.getImage() );
     painter.drawImage( 0, 0,
			pix.getImage()->scaled(w, h1, Qt::KeepAspectRatio) );
     
     painter.end();
}
