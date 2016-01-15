/*
 * $Id: main.cpp,v 1.2 2001/06/15 07:42:54 inou Exp $
 */

#include <iostream> //for test
using namespace std;
#include <qapplication.h>
#include <qtranslator.h>
#include <qlocale.h>

#include "window.h"
#include "setparam.h"
#include "plugin.h"
#include "config.h"

#ifdef Q_OS_MAC
#include <CoreFoundation/CFBundle.h>
QString resource_path;
char PLUGIN_PATH[PATH_MAX], COLORMAP_PATH[PATH_MAX];
#include <QDir>
#endif // Q_OS_MAC

#ifdef Q_WS_WIN
#include <windows.h>
#include <tchar.h>
TCHAR path[PATH_MAX];
char PLUGIN_PATH[PATH_MAX], COLORMAP_PATH[PATH_MAX];
#endif

using namespace QFract;

int main( int argc, char ** argv ) {

// For Mac OS X, get the resource path of the application bundle 
// to read plugins, colormaps, and .qm file(s) for i18n.
#ifdef Q_OS_MAC

  char pathPtr[PATH_MAX];
  CFURLRef resUrlRef = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
  CFURLGetFileSystemRepresentation(resUrlRef, true, (UInt8 *) pathPtr, PATH_MAX);
  resource_path=QString(pathPtr);
  qDebug("Path = %s", pathPtr);
  strcpy(PLUGIN_PATH,
	 qPrintable(resource_path + "/plugins"));
  strcpy(COLORMAP_PATH,
	 qPrintable(resource_path + "/colormaps"));
  
  cerr << "PLUGIN_PATH = " << PLUGIN_PATH << endl;
  cerr << "COLORMAP_PATH = " << COLORMAP_PATH << endl;

  QCoreApplication::setLibraryPaths(QStringList(resource_path + "/plugins"));
  //QCoreApplication::addLibraryPath(resource_path + "/plugins");
  //printf("after change, libraryPaths=(%s)\n", QCoreApplication::libraryPaths().join(",").toUtf8().data());

#endif // Q_OS_MAC
  
  QApplication app( argc, argv );
// get the application path for Windows
#ifdef Q_WS_WIN
  QString qpath;
  if (::GetModuleFileName(NULL, path, PATH_MAX)) {
#ifdef UNICODE
    qpath = QString::fromUtf16((ushort *) path);
#else
    qpath = QString::fromLocal8Bit(path);
#endif
    int i=qpath.lastIndexOf("\\");
    if (i == -1) {
      cerr << "Unable to get the application path." << endl;
      exit(0);
    }
    qpath.replace("\\","/");
    qpath.truncate(i);
  } else {
    cerr << "Unable to get the application path." << endl;
    exit(0);
  }
  strcpy(PLUGIN_PATH,
	 qPrintable(qpath + "/plugins"));
  strcpy(COLORMAP_PATH,
	 qPrintable(qpath + "/colormaps"));
#endif

  QTranslator translator;
  cerr << "locale = " << qPrintable(QLocale::system().name()) << endl;
  if (QLocale::system().name() == "ja_JP")
  {
#ifdef Q_OS_MAC
      translator.load("qfract-"+QLocale::system().name(), resource_path);
#else
#ifdef Q_WS_WIN
      translator.load("qfract-"+QLocale::system().name(), qpath);
#else
      if ( translator.load("qfract-"+QLocale::system().name()) ) {
	  qDebug("Translator loaded.");
      };
#endif // Q_WS_WIN
#endif // Q_OS_MAC
      app.installTranslator(&translator);
  }

  const QStringList args = QCoreApplication::arguments();
  if (args.count() == 1) {
      MainWindow *window = new MainWindow();
      window->loadPlugin(DEFAULT_PLUGIN);
      window->show();
      window->setInitialSize(640,640);
      window->setRedrawEnabled(true);
      window->draw();
  } else {
      MainWindow *window;
      for (int i=1; i<args.count(); i++) {
	  window = new MainWindow();
	  window->show();
	  window->open(args.at(i));
	  window->setRedrawEnabled( true );
      }
  }
  app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );

  return app.exec();
}
