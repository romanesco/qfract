/*
 * $Id: config.h,v 1.3 2003/10/14 11:29:01 inou Exp $
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifndef PLUGIN

#include <qglobal.h>

#ifndef PATH_MAX
#include <limits.h>
#endif

#ifdef Q_WS_WIN

extern char PLUGIN_PATH[PATH_MAX], COLORMAP_PATH[PATH_MAX];

#else // Q_WS_WIN

#ifdef Q_OS_MAC

extern char PLUGIN_PATH[PATH_MAX], COLORMAP_PATH[PATH_MAX];

#else

//#define PLUGIN_PATH "/home/inou/prog/qfract4-mt/plugins"
//#define COLORMAP_PATH "/home/inou/prog/qfract4-mt/colormaps"

#define PLUGIN_PATH "/Users/inou/prog/qfract4-mt/plugins"
#define COLORMAP_PATH "/Users/inou/prog/qfract4-mt/colormaps"

#endif // Q_OS_MAC
#endif // Q_WS_WIN

#endif // PLUGIN

#define DEFAULT_PLUGIN "quadratic.so"
#define DEFAULT_COLORMAP "default.map"

// for boost::thread
const int XDIV = 2;
const int YDIV = 2;
const int NUM_THREADS = XDIV*YDIV;

#endif // CONFIG_H
