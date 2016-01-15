/*
 * $Id: colormap.h,v 1.4 2002/06/17 11:41:55 inou Exp $
 */

#ifndef COLORMAP_H
#define COLORMAP_H

#include <qobject.h>
#include <qcolor.h>
#include <qstring.h>

namespace QFract {
const int NUM=256;

class ColorMap
{
public:
  ColorMap();
  ColorMap(int r[NUM], int g[NUM], int b[NUM]);
  ColorMap(int c[NUM][3]);
  ColorMap(QRgb c[NUM]);
  ColorMap(const char *filename);
  ColorMap(const ColorMap& c);
  
  QRgb Rgb(int i) { return rgb[i]; }
  
  void open(const char *filename);
  
  QString Name() { return name; }

private:
  QRgb rgb[NUM];
  QString name;
};
}
#endif // COLORMAP_H
