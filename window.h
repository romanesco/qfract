/*
 * $Id: window.h,v 1.7 2002/07/17 07:30:00 inou Exp $
 */

#ifndef WINDOW_H
#define WINDOW_H

// for debug
#include <iostream>
using namespace std;

#include <QMainWindow>
#include <QScrollArea>
//Added by qt3to4:
#include <QResizeEvent>

#include "pixmap.h"
#include "plugin.h"
#include "canvas.h"

class QWidget;
class QResizeEvent;
class QAction;

namespace QFract {

class Point;
class ColorMap;

#ifdef QT_MAC_FRAMEWORK_BUILD
const int MENUHEIGHT=22;
#else
const int MENUHEIGHT=23+18;
#endif // QT_MAC_FRAMEWORK_BUILD


class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  
  void setParamNum( int n ) { param.setNum(n); }
  void setParam( Parameter p )
      { param.setNum(p.Num()); param.set(p); canvas->setParam(p); }
  void setMaxIter( int m ) { maxiter=m;canvas->setMaxIter(m); }
  void setMaxOrbit( int m ) { maxorbit=m;canvas->setMaxOrbit(m); }
  void setOrbitStep( int m ) { orbitstep=m;canvas->setOrbitStep(m); }

  int paramNum() { return param.Num(); }
  double getParam(int i) { return param.Value(i); }

  void setFunc(Point (*f)(Point, Point, Parameter)) { canvas->setFunc(f); }
  void setInitFunc(Point (*f)(Point, Parameter)) { canvas->setInitFunc(f); }
  void setIterFunc(int (*f)(Point, Parameter, int)) { canvas->setIterFunc(f); }
  void setParamInitFunc(void (*pi)(Parameter)) { canvas->setParamInitFunc(pi); }
  //void loadPlugin(const char *filename);
  void setRegion(Rect r) { canvas->setRegion( r ); }

public slots:
  void draw() { canvas->draw(); }
  void newWindow();
  void setParam();
  void toggleOrbit();
  void createChild();
  void open();
  void open(const QString fn);
  void loadPlugin();
  void loadPlugin(const char *filename, bool needdrawing=true);
  void loadPluginAndColorMap(const char *filename, const char *mapname);
  void loadPluginDialog();
  void revertPlugin() { canvas->setPlugin( plugin ); draw(); }
  void loadColorMap();
  void setColorMap( ColorMap c ) { canvas->setColorMap(c); }
  void orbitChanged();
  void paramChanged();
  void paramChanged( Parameter ); // do not redraw
  void receiveParam( Point, Parameter );
  void childClosed() { child=NULL; }
  void disconnectChild();
  void setInitialSize( int w, int h);
  //void setSize( int w, int h );
  void setParamSize( Parameter param, int w, int h );
  void setStatusBar( QString );
  Canvas* getCanvas() { return canvas; }

  void zoom();

  void setRedrawEnabled( bool b ) { canvas->setRedrawEnabled(b); }

signals:
  void orbitChanged( bool );
  void pluginChanged();

protected:
  void resizeEvent( QResizeEvent *e );

  QScrollArea* scrollArea;
  Canvas* canvas;
  Parameter param;
  int maxiter;
  int maxorbit;
  int orbitstep;
  Plugin *plugin;
  MainWindow* child;
  QAction* SetOrbitId;
  bool isOrbit;

  QAction *actionSetParam;
  QAction *actionSetChild;
};
}
#endif // WINDOW_H
