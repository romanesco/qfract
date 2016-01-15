/*
 * $Id: canvas.h,v 1.6 2002/07/17 07:29:59 inou Exp $
 */

#ifndef CANVAS_H
#define CANVAS_H

#include "pixmap.h"
#include "plugin.h"

// for debug
#include <iostream>
using namespace std;

#include <qwidget.h>
#include <qpoint.h>
#include <qstring.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEvent>
//
#include <QPainterPath>
#include <QRect>

class QResizeEvent;
class QAction;

namespace QFract {
class Point;

class Canvas: public QWidget
{
  Q_OBJECT
public:
  Canvas( QWidget *parent = 0);
  
  void setParamNum( int n ) { param.setNum(n); }
  void setParam( Parameter p )
      { param.setNum(p.Num()); param.set(p); pix.setParam(p); }
  void setFunc( Point (*f)(Point, Point, Parameter ) ) { map=f; }
  void setInitFunc( Point (*f)(Point, Parameter ) ) { init=f; }
  void setIterFunc( int (*f)(Point, Parameter, int ) ) { pix.setFunc(f); }
  void setParamInitFunc( void (*pi)(Parameter) ){ pix.setParamInitFunc(pi); }
  void loadColorMap( const char *filename ) { pix.loadColorMap(filename); }
  void setColorMap( ColorMap c ) { pix.setColorMap(c); }
  
  int maxOrbit() { return maxorbit; }
  int paramNum() { return param.Num(); }
  ColorMap colorMap() { return pix.colorMap(); }
  double getParam(int i) { return param.Value(i); }
  const char ** ParamDesc() { return plugin->ParamDesc(); }
  Rect Region() { return pix.Region(); }
  
  void draw();
  void setPlugin( Plugin *i );
  void setPluginName( const char *pn ) { pluginname=QString(pn); }
  
  void mouseEvent( QMouseEvent *e );
  Point getPoint( QMouseEvent *e );
  void drawOrbit( QMouseEvent *e );
  void zoom( QMouseEvent *e );
  void zoomRect( QMouseEvent *e );
  void setStatusBar( QString s );

  QImage* getImage() { return pix.getImage(); }

  void setRedrawEnabled( bool b ) { redrawEnabled=b; }

public slots:
  void save();
  void setRegion( Rect r ) { pix.setRegion( r ); }
  void setMaxOrbit(int m) { maxorbit=m; }
  void setMaxIter(int m) { pix.setMaxIter(m); }
  void toggleOrbit( bool b ); 
  void mousePressEvent( QMouseEvent *e );
  void mouseReleaseEvent( QMouseEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void print();

signals:
  void sendParam( Point, Parameter );
  void _setStatusBar_( QString );
  
protected:
  void resizeEvent( QResizeEvent *e );
  void paintEvent( QPaintEvent * );
  
  Pixmap pix;
  int maxorbit;
  Parameter param;
  Point (*map)(Point, Point, Parameter ); // calculation function (map once)
  Point (*init)(Point, Parameter ); // get initial point
  Plugin *plugin;
  QString pluginname;
  //QString colormapname;

  QPainterPath *path;
  QRect rect;

  bool redrawEnabled;
  bool isOrbit;
  bool leftButtonPressed;
  bool rightButtonPressed;
  QPoint zoomStart;
};
}
#endif // CANVAS_H
