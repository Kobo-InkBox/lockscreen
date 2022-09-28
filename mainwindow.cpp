#include "mainwindow.h"
#include "lockscreen.h"

#include "ui_mainwindow.h"

#include <QTimer>
#include <QScreen>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::ofstream fhandler;
    fhandler.open("/opt/ibxd");
    fhandler << "screenshot\n";
    fhandler.close();

    QDir::setCurrent("/mnt/onboard/.adds/inkbox");

    QString file = ".config/12-lockscreen/background";
    if(QFile::exists(file)) {
        QFile fileToRead(file);
        fileToRead.open(QIODevice::ReadOnly);
        QTextStream in (&fileToRead);
        QString content = in.readAll();
    }

    this->ui->centralwidget->setStyleSheet("background-image:url(\"/external_root/tmp/lockscreen.png\"); background-position: center;");

    QTimer::singleShot(100, this, &MainWindow::launchDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::launchDialog() {
    qDebug() << "Launching dialog";

    lockscreen* lockscreenDialog = new lockscreen;
    lockscreenDialog->exec();
}
