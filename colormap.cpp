/*
 * $Id: colormap.cpp,v 1.5 2003/10/14 11:29:01 inou Exp $
 */

#include <iostream>
#include "colormap.h"
#include "config.h"
using namespace QFract;

#include <qcolor.h>
#include <qstring.h>
#include <qmessagebox.h>
using namespace Qt;

#include <fstream>
using namespace std;

ColorMap::ColorMap()
{
  open(DEFAULT_COLORMAP);
}

ColorMap::ColorMap(int r[NUM], int g[NUM], int b[NUM])
{
  for ( int i=0; i<NUM; i++ )
    rgb[i]= qRgb(r[i], g[i], b[i]);
  name = QString(DEFAULT_COLORMAP);
}

ColorMap::ColorMap(int c[NUM][3])
{
  for ( int i=0; i<NUM; i++ )
    rgb[i]= qRgb(c[i][0], c[i][1], c[i][2]);
  name = QString(DEFAULT_COLORMAP);
}

ColorMap::ColorMap(QRgb c[NUM])
{
  for ( int i=0; i<NUM; i++ )
    rgb[i]= c[i];
  name = QString(DEFAULT_COLORMAP);
}

ColorMap::ColorMap(const char *filename)
{
  open(filename);
}

ColorMap::ColorMap(const ColorMap& c)
{
  for (int i=0; i<NUM; i++)
    rgb[i]=c.rgb[i];
  name = c.name;
}

void ColorMap::open(const char *filename)
{
  ifstream input;
  name = QString(filename);

  char tmp[PATH_MAX];
  if ((*filename != '/') && (*filename != '.') && (*(filename+1) != ':')) {
    strcpy(tmp, COLORMAP_PATH);
    strcat(tmp, "/");
    strcat(tmp, filename);
  } else {
    strcpy(tmp, filename);
  }
  qDebug("Open colormap file: %s", tmp);
  
  input.open(tmp);
  if (!input) {
      cerr << "Cannot open colormap: " << tmp << endl;
      //QMessageBox::critical(0, "Error", tr("Cannot open colormap : %1.").arg(tmp));
    return;
  }
  
  int r, g, b;
  for (int i=0; i<NUM; i++) {
    input >> r >> g >> b;
    rgb[i] = qRgb(r,g,b);
  }
}
