#include <QApplication>
#include "mainwindow.h"

#include "drawtargetwidget.h"
#include <qfile.h>
#include <QtPlugin>

#ifdef QT_STATIC
#if QT_VERSION < 0x050000
Q_IMPORT_PLUGIN(qico)
#endif
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    w.DefaultArrangement();

    Q_INIT_RESOURCE(resources);
  
    return a.exec();
}
