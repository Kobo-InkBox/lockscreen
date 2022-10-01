#include "mainwindow.h"
#include "lockscreen.h"
#include "global.h"

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

    if(choosedBackground == Background) {
        this->ui->centralwidget->setStyleSheet("background-image:url(\"/external_root/tmp/lockscreen.png\"); background-position: center;");
    }
    if(choosedBackground == ScreenSaver){
        QString screenSaverUsedPath = "/external_root/tmp/screensaver-used.txt";
        if(QFile(screenSaverUsedPath).exists() == true) {
            QString screenSaverFilePath = readFile(screenSaverUsedPath).replace("\n", "");
            QString path = "/mnt/onboard/onboard/.screensaver/" + screenSaverFilePath;
            QString stylesheetReplace = "background-image:url(\"PATH\"); background-position: center;";
             this->ui->centralwidget->setStyleSheet(stylesheetReplace.replace("PATH", path));
        }
        else {
            choosedBackground = Blank;
        }
    }
    if(choosedBackground == Blank) {
        this->ui->centralwidget->setStyleSheet("background-color: white");
    }
    QTimer::singleShot(100, this, &MainWindow::launchDialog);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::launchDialog() {
    qDebug() << "Launching dialog";

    lockscreen* lockscreenDialog = new lockscreen;
    lockscreenDialog->setWindowFlag(Qt::FramelessWindowHint);
    lockscreenDialog->exec();
}
