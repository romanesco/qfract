#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <qdialog.h>

#include "config.h"

class QTableWidget;
class QTableWidgetItem;
class QPushButton;
class QButtonGroup;

namespace QFract {

class PluginDialog : public QDialog
{
    Q_OBJECT
public:
    PluginDialog( QWidget *parent, Qt::WindowFlags f=Qt::WindowFlags() );
    void loadPluginList();

public slots:
    void accept();
    void openFile();

signals:
    void pluginSet( const char* );
    void loadPlugin( const char* );
    void openFileDialog();

private:
    QTableWidget *table;
    QButtonGroup *buttongroup;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *openFileButton;
};
}
#endif // PLUGINDIALOG_H
