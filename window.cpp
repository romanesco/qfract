/*
 * $Id: window.cpp,v 1.9 2003/10/14 11:29:01 inou Exp $
 */

/* TRANSLATOR QFract::MainWindow */

#include "window.h"
#include "setparam.h"
#include "canvas.h"
#include "config.h"
#include "plugindialog.h"
using namespace QFract;

//#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qnamespace.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qimagereader.h>
#include <qsize.h>
//#include <q3accel.h>
#include <qwidget.h>
//#include <qmenudata.h>
#include <qaction.h>
#include <qfiledialog.h>
//
using namespace Qt;

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow( parent )
{
  setAttribute(Qt::WA_DeleteOnClose);
  scrollArea=new QScrollArea(this);
  setCentralWidget(scrollArea);
  canvas = new Canvas( scrollArea );
  //setCentralWidget( canvas );
  scrollArea->setWidget(canvas);
  
  param.set(0, (double *) NULL);
  
  child=NULL;
  
  //actionSetParam = new QAction( tr("Set &parameters..."), this );
  actionSetParam = new QAction( MainWindow::tr("Set &parameters..."), this );
  //actionSetParam = new QAction( qApp->translate("MainWindow", "Set &parameters..."), this );
  actionSetParam->setShortcut(CTRL+Key_P);
  connect( actionSetParam, SIGNAL( triggered() ),
	   this, SLOT( setParam() ) );
  //actionSetParam->setEnabled( false );
  
  actionSetChild =  new QAction( tr("Create &child window"), this );
  actionSetChild->setShortcut(CTRL+Key_C);
  connect( actionSetChild, SIGNAL( triggered() ),
	   this, SLOT( createChild() ) );

  QMenu *file = new QMenu( this );
  file->setTitle( tr("&File") );
  menuBar()->addMenu( file );
  QMenu *setting  = new QMenu( this );
  setting->setTitle( tr("&Settings") );
  menuBar()->addMenu( setting );
  
  file->addAction( tr("&New Window"), this, SLOT( newWindow() ), CTRL+Key_N ); 
  file->addAction( tr("&Open..."), this, SLOT( open() ), CTRL+Key_O );
  file->addAction( tr("&Save..."), canvas, SLOT( save() ), CTRL+Key_S );
  file->addSeparator();
  file->addAction( tr("&Print..."), canvas, SLOT( print() ), CTRL+SHIFT+Key_P );
  file->addSeparator();
  file->addAction( tr("&Close"), this, SLOT( close() ), CTRL+Key_W );
  file->addAction( tr("E&xit"), qApp, SLOT( closeAllWindows() ), CTRL+Key_Q );
  
  setting->addAction( tr("&Load function plugin..."),
		       this, SLOT( loadPluginDialog() ), CTRL+Key_L );
/*
  setting->addAction( tr("&Load function plugin..."),
		       this, SLOT( loadPlugin() ), CTRL+Key_L );
  setting->addAction( tr("Load function plugin via &Dialog..."),
		       this, SLOT( loadPluginDialog() ), CTRL+Key_D );
*/
  setting->addSeparator();
  setting->addAction( actionSetParam );
  setting->addAction( actionSetChild );
  setting->addAction( tr("&Revert"),
		       this, SLOT( revertPlugin() ), CTRL+Key_R );
  
  SetOrbitId = setting->addAction( tr("&Forward orbit"), this,
				    SLOT( toggleOrbit() ), CTRL+Key_F );
  SetOrbitId->setCheckable( true );
  SetOrbitId->setChecked( isOrbit=false );
  
  setting->addSeparator();
  setting->addAction( tr("Load Color &map..."),
		      this, SLOT( loadColorMap() ), CTRL+Key_M );
  setting->addAction( tr("Zoom window"),
		      this, SLOT( zoom() ), CTRL+Key_X );
 
  connect( this, SIGNAL(orbitChanged( bool )),
	   canvas, SLOT(toggleOrbit( bool )) );
  connect( canvas, SIGNAL( _setStatusBar_( QString ) ),
	   this, SLOT( setStatusBar( QString ) ) );
  statusBar()->showMessage(tr("Drawing..."));
  //resize( 320, 320+MENUHEIGHT);
  //setSize(320,320);
  
  statusBar()->showMessage(tr("Ready"));
}

void MainWindow::newWindow()
{
  MainWindow *win = new MainWindow();
  win->loadPlugin(DEFAULT_PLUGIN);
  win->show();
  win->setInitialSize(320,320);
  win->setRedrawEnabled( true );
  win->draw();
}

void MainWindow::setParam()
{
  int menuheight = menuBar()->size().height() + statusBar()->size().height();
  SetParam *sp= new SetParam( canvas->Region(), param,
			      canvas->width(), canvas->height(),
			      &maxiter, &maxorbit, &orbitstep,
			      canvas->ParamDesc(),
			      this );
  
  connect( sp, SIGNAL( orbitSet() ), this, SLOT( orbitChanged() ) );
  connect( sp, SIGNAL( paramSet( Parameter ) ), this, SLOT( paramChanged( Parameter ) ) );
  connect( sp, SIGNAL( setRegion( Rect ) ),
	   canvas, SLOT( setRegion( Rect ) ) );
  connect( sp, SIGNAL( setParamSize( Parameter, int, int ) ),
	   this, SLOT( setParamSize( Parameter, int, int ) ) );
  
  sp->show();
}

void MainWindow::createChild()
{
  if ((plugin->ChildName()==NULL) || (*(plugin->ChildName())=='\0')) return;
  if (child != NULL) 
    disconnect( canvas, SIGNAL( sendParam( Point, Parameter ) ),
		child, SLOT( receiveParam( Point, Parameter ) ) );
  
  child = new MainWindow();
  child->loadPlugin(plugin->ChildName());
  //child->setColorMap(canvas->colorMap());
  
  connect( canvas, SIGNAL( sendParam( Point, Parameter ) ),
	   child, SLOT( receiveParam( Point, Parameter ) ) );
  connect( child, SIGNAL( destroyed() ),
	   this, SLOT( childClosed() ) );
  connect( child, SIGNAL( pluginChanged() ),
	   this, SLOT( disconnectChild() ) );
  
  child->show();
  child->setInitialSize(320,320);
  child->setRedrawEnabled( true );
  //child->setSize(320,320);
  child->draw();
}

void MainWindow::loadColorMap()
{
    QString fn= QFileDialog::getOpenFileName( this, 
					      tr("Open color map"),
					      QString(COLORMAP_PATH),
					      QString(tr("Colormap (*.map)")) );
  if ( !fn.isEmpty() ) {
      canvas->loadColorMap(qPrintable(fn));
      draw();
  }
  canvas->repaint();
}

void MainWindow::open()
{
    QString fn= QFileDialog::getOpenFileName( this, 
					      tr("Open File"),
					      QString("./"),
					      QString("PNG file (*.png)") );
    if ( fn.isEmpty() ) {
	return;
    }

/* to open new window when loaded
 * does not work well because of setsize() problem...
 */
    MainWindow *win = new MainWindow();
    win->show();
    win->open(fn);
    win->setRedrawEnabled( true );
}

void MainWindow::open(const QString fn)
{
    QImageReader reader(fn);
    if (reader.supportsOption(QImageIOHandler::Description)) {
	// resize
	QSize size=reader.size();
	canvas->setRedrawEnabled(false);
	setInitialSize(size.width(),size.height());
	canvas->setRedrawEnabled(true);
	QString pluginname=reader.text("PluginName");
	cerr << "Plugin " << qPrintable(pluginname) << "..." << endl;
	loadPlugin( qPrintable(pluginname), false );
	cerr << " done." << endl;

	// parameters
	int num = reader.text("NumParameters").toInt();
	param.setNum(num);
	for (int i=0; i<num; i++) {
	    param.setValue(i,
			   reader.text(QString("Parameter%1").arg(i)).toDouble());
	    cerr << "Parameter " << i << ": " << param.Value(i) << endl;
	}
	canvas->setParam( param );
	cerr << "parameters read." << endl;

	// region
	double xl, yt, xr, yb;
	xl = reader.text("XLeft").toDouble();
	yt = reader.text("YTop").toDouble();
	xr = reader.text("XRight").toDouble();
	yb = reader.text("YBottom").toDouble();
	setRegion(Rect(xl,yt,xr,yb));
	
	// max iteration times
	int max = reader.text("Iteration").toInt();
	int maxorbit = reader.text("Orbit").toInt();
	int orbitstep = reader.text("OrbitStep").toInt();
	if (orbitstep<=0) orbitstep = 1;

	setMaxIter(max);
	setMaxOrbit(maxorbit);
	setOrbitStep(orbitstep);

	// colormap
	QString colormapname = reader.text("ColorMap");
	cerr << qPrintable(colormapname) << endl;
	canvas->loadColorMap(qPrintable(colormapname));
	cerr << "colormap set." << endl;

	// image
	QImage* image=canvas->getImage();
	reader.read(image);
	repaint();
    } else {
	QMessageBox::critical( 0, "Error",
			       fn +QString(": Unsupported file."));
    }
}

void MainWindow::loadPlugin()
{
    QString fn= QFileDialog::getOpenFileName( this, 
					      tr("Open function plugin"),
					      QString(PLUGIN_PATH),
					      QString(tr("Shared Object (*.so)")));
    if ( !fn.isEmpty() ) {
	loadPlugin(qPrintable(fn));
    }
}

void MainWindow::loadPluginDialog()
{
    PluginDialog *pd = new PluginDialog( this );
    pd->show();
}

void MainWindow::loadPlugin( const char *filename, bool needdrawing )
{
  plugin= new Plugin(filename);
  if ( (plugin == NULL) || (plugin->Handle() == NULL) )
  { loadPlugin(); return; }
  setFunc( plugin->Map() );
  setInitFunc( plugin->Init() );
  setIterFunc( plugin->Iter() );
  setParamInitFunc( plugin->paramInit() );
  setParam( plugin->Param() );
  setMaxIter( plugin->MaxIter() );
  setMaxOrbit( plugin->MaxOrbit() );
  setOrbitStep(1);

  canvas->loadColorMap( plugin->ColorMap() );

  setWindowTitle( plugin->Name() );
  
  if ((plugin->ChildName()==NULL) || (*(plugin->ChildName()) == '\0'))
    actionSetChild->setEnabled( false );
  else
    actionSetChild->setEnabled( true );
  
  if (child != NULL) 
    disconnect( canvas, SIGNAL( sendParam( Point, Parameter ) ),
		child, SLOT( receiveParam( Point, Parameter ) ) );
  
  //canvas->setParam( param );
  canvas->setPlugin( plugin );
  canvas->setPluginName(filename);
  
  if (needdrawing) {
      canvas->draw();
  }

  emit pluginChanged();
}

void MainWindow::loadPluginAndColorMap(const char *filename,
				       const char *mapname)
{
    loadPlugin(filename,false);
    canvas->loadColorMap(mapname);
    canvas->draw();
    repaint();
}

void MainWindow::orbitChanged()
{
  canvas->setMaxOrbit( maxorbit );
  canvas->setOrbitStep( orbitstep );
}

void MainWindow::paramChanged( Parameter p )
{
  param.set(p);
  paramChanged();
}

void MainWindow::paramChanged()
{
  canvas->setMaxIter( maxiter );
  canvas->setParam( param );
  canvas->draw();
}

void MainWindow::receiveParam( Point z, Parameter p )
{
  int n=param.Num();
  if (n<1) return;
  param.setValue( 0, z.x );
  if (n<2) { paramChanged(); return; }
  param.setValue( 1, z.y );
  
  for (int i=0; i<n; i++) {
    if (n <= i+2 ) { paramChanged(); return; }
    param.setValue( i+2, p.Value(i) );
  }
  paramChanged();
}

void MainWindow::toggleOrbit()
{
  isOrbit= !isOrbit;
  SetOrbitId->setChecked( isOrbit );
  emit orbitChanged( isOrbit );
}

void MainWindow::disconnectChild()
{
  disconnect(canvas, SIGNAL( sendParam( Point, Parameter ) ),
	     child, SLOT( receiveParam( Point, Parameter ) ) );
}

void MainWindow::setInitialSize( int w, int h)
{
  canvas->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
  canvas->setMinimumSize(w,h);
  resize(w+5,h+menuBar()->size().height() + statusBar()->size().height()+5);
  //canvas->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
  canvas->setMinimumSize(0,0);
  setMaximumSize(w+5, h+menuBar()->size().height() + statusBar()->size().height()+5); 
}

/*
void MainWindow::setSize( int w, int h ) 
{
    //cerr << menuBar()->size().height() + statusBar()->size().height() << endl;
    resize(w, h+menuBar()->size().height() + statusBar()->size().height()); 
}
*/
void MainWindow::setParamSize( Parameter param, int w, int h ) 
{
  param.set(param);
  canvas->setMaxIter( maxiter );
  canvas->setParam( param );
  //cerr << menuBar()->size().height() + statusBar()->size().height() << endl;
  setMaximumSize(w+5, h+menuBar()->size().height() + statusBar()->size().height()+5); 
  resize(w+5, h+menuBar()->size().height() + statusBar()->size().height()+5); 
  canvas->resize(w,h);
}

void MainWindow::setStatusBar( QString s)
{
  statusBar()->showMessage(s);
}

void MainWindow::zoom()
{
  setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  if (windowState() & Qt::WindowMaximized) {
    showNormal();
  } else {
    showMaximized();
  }

  int w=width(), h=height();
  scrollArea->resize( w-5, h-MENUHEIGHT-5 );
  canvas->resize(scrollArea->width(), scrollArea->height());
  setMaximumSize(w, h);
  canvas->draw();
}

void MainWindow::resizeEvent( QResizeEvent *e )
{
    QWidget::resizeEvent( e );
    scrollArea->resize( e->size().width(), e->size().height()-MENUHEIGHT );
    //canvas->resize( e->size().width(), e->size().height()-MENUHEIGHT );
    update();
}
