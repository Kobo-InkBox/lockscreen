#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    const QScreen* screen = qApp->primaryScreen();
    w.setGeometry(QRect(QPoint(0,0), screen->geometry ().size()));
    w.show();
    return a.exec();
}
