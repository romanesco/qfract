/*
 * $Id: plugin.cpp,v 1.4 2002/06/17 11:41:55 inou Exp $
 */

#include "plugin.h"
#include "config.h"

#include <iostream>
using namespace std;
#include <string.h>
extern "C" {
#include <dlfcn.h>
#include <libgen.h>
}
#include <qmessagebox.h>
using namespace Qt;

using namespace QFract;

Plugin::Plugin(const char *filename) { open(filename); }

Plugin::~Plugin() { close(); }

void* Plugin::open(const char *filename)
{
  if ((filename==NULL) || (*filename == '\0')) return NULL;
  
  PluginInfo* (*d)(void);
  
  char tmp[PATH_MAX];
  bool isFullPath=false;
  if ((*filename != '/') && (*filename != '.') && (*(filename+1) != ':')) {
      strcpy(tmp, PLUGIN_PATH);
      strcat(tmp, "/");
      strcat(tmp, filename);
  } else {
    strcpy(tmp, filename);
    isFullPath=true;
  }
  qDebug( "Opening plugin file: %s...", tmp);
  
  if ( ( handle = dlopen(tmp, RTLD_LAZY) ) == NULL ) {
    if (isFullPath) {
      qDebug( "Failed. Searching in the default plugins directory..." );
      char tmp2[PATH_MAX];
      strcpy(tmp2, filename);
      // search plugin in PLUGIN_PATH
      strcpy(tmp, PLUGIN_PATH);
      strcat(tmp, "/");
      strcat(tmp, basename(tmp2));
      if ( ( handle = dlopen(tmp, RTLD_LAZY) ) != NULL ) {
	goto Opened;
    }
    
    QMessageBox::critical( 0, "Error",
			   QString("dlopen: %1 failed.").arg(tmp) );
    return NULL;
    }
  }

 Opened:
  if ( ( d = (PluginInfo* (*)(void)) dlsym(handle, "getInfo") ) == NULL ) {
      QMessageBox::critical( 0, "Error",
			   QString("dlsym: %1: cannot get function getInfo.").arg(filename));
    return NULL;
  }
  info=d();
  
  if ( ( map = (Point (*)(Point, Point, Parameter)) dlsym(handle, "map") ) == NULL ) {
      QMessageBox::critical( 0, "Error",
			     QString("dlsym: %1: cannot get function map.").arg(filename));
    return NULL;
  }
  
  if ( ( iter = (int (*)(Point, Parameter, int)) dlsym(handle, "iter") ) == NULL ) {
      QMessageBox::critical( 0, "Error",
			     QString("dlsym: %1: cannot get function iter.").arg(filename));
    return NULL;
  }
  
  if ( ( init = (Point (*)(Point, Parameter)) dlsym(handle, "init") ) == NULL ) {
      QMessageBox::critical( 0, "Error",
			     QString("dlsym: %1: cannot get function init.").arg(filename));
    return NULL;
  }
  
  paraminit = (void (*)( Parameter)) dlsym(handle, "paraminit");
  
  qDebug( "Done." );
  return handle;
}

int Plugin::close()
{
  return dlclose(handle);
}
