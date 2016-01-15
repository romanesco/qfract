/* TRANSLATOR QFract::PluginDialog */

#include <qdialog.h>
#include <qtablewidget.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
using namespace Qt;

// for debug
#include <iostream>
using namespace std;

#include "plugindialog.h"
#include "window.h"
using namespace QFract;

PluginDialog::PluginDialog( QWidget *parent, Qt::WindowFlags f)
    : QDialog( parent, f )
{
    
    table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setDragDropMode(QAbstractItemView::NoDragDrop);
    //table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setSortingEnabled(true);

    okButton = new QPushButton(tr("OK"));
    okButton->setDefault( true );
    cancelButton = new QPushButton(tr("Cancel"));
    openFileButton = new QPushButton(tr("Open &File..."));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(openFileButton);
    buttonLayout->addWidget(okButton);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(table);
    mainLayout->addLayout(buttonLayout, Qt::AlignRight);
    setLayout(mainLayout);

    loadPluginList();

    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( openFileButton, SIGNAL( clicked() ), this, SLOT( openFile() ) );
    connect( this, SIGNAL(loadPlugin( const char* )),
	     parent, SLOT(loadPlugin( const char* )) );
    connect( this, SIGNAL( openFileDialog() ), parent, SLOT( loadPlugin() ) );

    //table->resizeColumnsToContents();
    table->setColumnWidth(0,180);
    table->setColumnWidth(1,210);
    table->setColumnWidth(2,300);
    table->setColumnWidth(3,300);

    resize(1000,600);
    setWindowTitle(tr("Choose plugin"));
}

void PluginDialog::openFile()
{
    //((MainWindow *) parent())->loadPlugin();
    reject();
    emit openFileDialog();

}

// load information on plugins
#include "pluginlist.c"

void PluginDialog::loadPluginList()
{

    table->setRowCount(NUMROW);
    table->setColumnCount(NUMCOLUMN);

    // set title
    QStringList headerLabels;
    headerLabels << tr("Filename") << tr("Function")
		 << tr("Name") << tr("Comment");
    table->setHorizontalHeaderLabels(headerLabels);

    buttongroup = new QButtonGroup();
    QRadioButton * radiobutton;
    QLabel* label;
    for (int row=0; row<NUMROW; row++) {
	radiobutton = new QRadioButton(plugindesc[row][0]);
	buttongroup->addButton(radiobutton);
	table->setCellWidget(row, 0, radiobutton);
	for (int col=1; col<NUMCOLUMN; col++) {
	    label = new QLabel(plugindesc[row][col]);
	    label->setTextFormat(Qt::RichText);
	    table->setCellWidget(row, col, label);
	}
    }
}

void PluginDialog::accept()
{
    QRadioButton * checkedbutton = (QRadioButton*) buttongroup->checkedButton();
    if ( checkedbutton == 0 ) {
	QDialog::accept();
    }
	
    QString fn = checkedbutton->text()+".so";

    emit loadPlugin(qPrintable(fn));

    QDialog::accept();
}
