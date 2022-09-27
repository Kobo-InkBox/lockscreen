#include "mainwindow.h"
#include "lockscreen.h"

#include "ui_mainwindow.h"

#include <QTimer>
#include <QScreen>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
