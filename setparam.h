/*
 * $Id: setparam.h,v 1.5 2002/06/17 11:41:56 inou Exp $
 */

#ifndef SETPARAM_H
#define SETPARAM_H

#include "double.h"

#include <qdialog.h>

class QLineEdit;

namespace QFract {

class SetParam : public QDialog
{
  Q_OBJECT
public:
  SetParam( Rect r, Parameter p, int w, int h, int *mi, int *mo,
	    const char **pdesc, QWidget *parent=0, Qt::WindowFlags f=0 );
  void accept();
  
signals:
  void orbitSet();
  void paramSet( Parameter );
  void setRegion( Rect );
  void setParamSize( Parameter, int, int );
  
private:
  Rect region;
  Parameter param;
  int width, height;
  int *maxiter;
  int *maxorbit;
  
  QLineEdit *edsize[2];
  QLineEdit *edregion[2][2];
  QLineEdit **edparam;
  QLineEdit *edmaxiter;
  QLineEdit *edmaxorbit;
  
};
}
#endif // SETPARAM_H
