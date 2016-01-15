/*
 * $Id: setparam.cpp,v 1.6 2002/07/17 07:29:59 inou Exp $
 */

/* TRANSLATOR QFract::SetParam */

#include "setparam.h"
#include "double.h"
using namespace QFract;

#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qpushbutton.h>
//#include <qhbox.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

SetParam::SetParam( Rect r, Parameter p, int w, int h, int *mi, int *mo,
		    const char **pdesc, QWidget *parent, Qt::WindowFlags f )
  : QDialog( parent, f)
{
  region=r;
  param.set(p);
  maxiter=mi;
  maxorbit=mo;
  width=w;
  height=h;

  QVBoxLayout* vbox = new QVBoxLayout(this);
  QGridLayout* grid = new QGridLayout();
  vbox->addLayout(grid);

  QLabel *label;
  
  for (int i=0; i<2; i++) {
      edsize[i] = new QLineEdit(this);
      grid->addWidget( edsize[i], 1, i*2+1 );
  }
  edsize[0]->setText( QString::number(width) );
  edsize[1]->setText( QString::number(height) );

  label= new QLabel( tr("Width"), edsize[0] );
  grid->addWidget( label, 1, 0 );
  label= new QLabel( tr("Height"), edsize[1] );
  grid->addWidget( label, 1, 2 );

  // QLineEdit for region
  for (int i=0; i<2; i++) {
    for (int j=0; j<2; j++) {
      edregion[i][j]=new QLineEdit(this);
      grid->addWidget( edregion[i][j], i+2, j*2+1 );
    }
  }
  for (int i=0; i<2; i++) {
      edsize[i]->setModified(false);
  }
  
  edregion[0][0]->setText( QString::number(region.xleft, 'g', 20) );
  edregion[0][1]->setText( QString::number(region.ytop, 'g', 20) );
  edregion[1][0]->setText( QString::number(region.xright, 'g', 20) );
  edregion[1][1]->setText( QString::number(region.ybottom, 'g', 20) );
  
  label= new QLabel( tr("XLeft"), edregion[0][0] );
  grid->addWidget( label, 2, 0 );
  label= new QLabel( tr("YTop"), edregion[0][1] );
  grid->addWidget( label, 2, 2 );
  label= new QLabel( tr("XRight"), edregion[1][0] );
  grid->addWidget( label, 3, 0 );
  label= new QLabel( tr("YBottom"), edregion[1][1] );
  grid->addWidget( label, 3, 2 );
  
  for (int i=0; i<2; i++) {
    for (int j=0; j<2; j++) {
      edregion[i][j]->setModified(false);
    }
  }
  
  // create textboxes
  // max iteration
  grid->addWidget( new QLabel( tr("Max iteration times:"), this) , 4, 2);
  
  // iteration
  edmaxiter = new QLineEdit( this );
  grid->addWidget(edmaxiter, 5, 3 );
  edmaxiter->setText( QString::number(*maxiter) );
  edmaxiter->setModified(false);
  
  label= new QLabel( tr("Iteration"), edmaxiter );
  grid->addWidget( label, 5, 2 );
  
  // orbit
  edmaxorbit = new QLineEdit( this );
  grid->addWidget(edmaxorbit, 6, 3 );
  edmaxorbit->setText( QString::number(*maxorbit) );
  edmaxorbit->setModified(false);
  
  label= new QLabel( tr("Forward orbit"), edmaxorbit );
  grid->addWidget( label, 6, 2 );
  
  edmaxiter->setFocus();
  
  // parameters
  grid->addWidget( new QLabel( tr("Parameters:"), this) , 4, 0);
  
  int n=p.Num();
  if (n==0) {
    grid->addWidget( new QLabel( tr("no parameter"), this) , 5, 0);
  } else {
    edparam = new QLineEdit *[n];
    
    for (int i=0; i<n; i++) {
      edparam[i] = new QLineEdit( this );
      grid->addWidget(edparam[i], i+5, 1 );
      edparam[i]->setText( QString::number(p.Value(i), 'g', 20) );
      edparam[i]->setModified(false);      
      QString s;
      if ( ( pdesc != NULL ) && (pdesc[i] != NULL) ) {
	  s=pdesc[i];
      } else { 
	  s = QString::number(i);
      }
      QLabel *label= new QLabel( s, edparam[i] );
      grid->addWidget( label, i+5, 0 );
    }
    edparam[0]->setFocus();
  }
  
  grid->setColumnStretch( 0, 0 );
  grid->setColumnStretch( 1, 10 );
  grid->setColumnStretch( 2, 0 );
  grid->setColumnStretch( 3, 10 );
  
  // buttons
  QHBoxLayout* hbox = new QHBoxLayout;
  vbox->addLayout(hbox);
  QPushButton* cancel = new QPushButton(tr("Cancel"), this);
  hbox->addWidget(cancel);
  QPushButton* ok = new QPushButton(tr("OK"), this);
  ok->setDefault( true );
  hbox->addWidget(ok);
  
  setFixedWidth(600);
  
  connect( ok, SIGNAL(clicked()), this, SLOT(accept()) );
  connect( cancel, SIGNAL(clicked()), this, SLOT(reject()) );
  
}

void SetParam::accept()
{
  bool b;

  edmaxorbit->text().toInt(&b);
  if (b==false) return;
  *maxorbit = edmaxorbit->text().toInt();
  emit orbitSet();
  
  for (int i=0; i<2; i++) {
      edsize[i]->text().toInt(&b);
      if(b==false) return;
  }
  
  for (int i=0; i<2; i++) {
    for (int j=0; j<2; j++) {
      edregion[i][j]->text().toDouble(&b);
      if (b==false) return;
    }
  }
  
  edmaxiter->text().toInt(&b);
  if (b==false) return;
  
  for (int i=0; i<param.Num(); i++ ) {
    edparam[i]->text().toDouble(&b);
    if (b==false) return;
  }		
  
  bool edited=false;
  
  for (int i=0; i<2; i++) {
      if (edsize[i]->isModified()) {
	  edited=true;
      }
  }

  for (int i=0; i<2; i++) {
    for (int j=0; j<2; j++) {
      if (edregion[i][j]->isModified()) {
	edited=true;
      }
    }
  }
  
  for (int i=0; i<param.Num(); i++ ) {
    if ( edparam[i]->isModified() )
      edited=true;
  }
  
  if ( edmaxiter->isModified() )
    edited=true;
  
  if (edited) {
    Rect r( edregion[0][0]->text().toDouble(),
	    edregion[0][1]->text().toDouble(),
	    edregion[1][0]->text().toDouble(),
	    edregion[1][1]->text().toDouble() );
    emit setRegion( r ); // do not redraw
    
    for (int i=0; i<param.Num(); i++ ) {
      param.setValue( i, edparam[i]->text().toDouble() );
    }
    
    *maxiter = edmaxiter->text().toInt();
    

    int w,h;
    w = edsize[0]->text().toInt();
    h =  edsize[1]->text().toInt();
    if ((width == w) && (height == h))
	emit paramSet( param );
    else {
	width=w; height=h;
	emit setParamSize(param, width, height); // redraw
    }
  }
  
  QDialog::accept();
}
