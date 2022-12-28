#include "mainwindow.h"
#include "global.h"

#include <QApplication>
#include <QScreen>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QThread>
#include <QStringList>

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setDefaultWorkDir();

    QString fuserOutput = launchProcess("/usr/bin/fuser", QStringList{"/dev/input/event1"}).replace("\n", "");
    fuserOutput.chop(1);
    qDebug() << "fuser output: '" << fuserOutput << "'";

    int ipdPid = getPidByName("ipd");
    qDebug() << "ipdPid: " << ipdPid;

    int lockPid = QCoreApplication::applicationPid();
    qDebug() << "lockPid: " << lockPid;

    fuserOutput = fuserOutput.replace(QString::number(ipdPid), "");
    fuserOutput = fuserOutput.replace(QString::number(lockPid), "");

    QStringList pidListString = fuserOutput.split(" ");

    qDebug() << "pidListString: " << pidListString;
    for(QString string: pidListString) {
        if(string.isEmpty() == false) {
            pidList.append(QVariant(string).toInt());
        }
    }
    qDebug() << "pidList: " << pidList;

    for(int pid: pidList) {
        freezeApp(pid);
    }

    QString content = readFile(".config/12-lockscreen/background");
    // Options are: blank, screensaver, background
    if(content.contains("background") == true) {
        chosenBackground = background;
        qDebug() << "chosenBackground is: " << "background";
        // This propably isn't needed because IPD will handle that
            /*
            // For ibxd
            elif [[ "$line" == "screenshot" ]]; then
                    fbgrab /tmp/screenshot.png
            fi
            */
            /*
            std::ofstream fhandler;
            fhandler.open("/opt/ibxd");
            fhandler << "screenshot\n";
            fhandler.close();
            QThread::msleep(400);
        */
    }
    else if(content.contains("blank") == true) {
        chosenBackground = blank;
        qDebug() << "chosenBackground is: " << "blank";
    }
    else if(content.contains("screensaver") == true) {
        chosenBackground = screenSaver;
        qDebug() << "chosenBackground is: " << "screenSaver";
    }

    MainWindow w;
    const QScreen* screen = qApp->primaryScreen();
    w.setGeometry(QRect(QPoint(0,0), screen->geometry ().size()));

    w.show();
    return a.exec();
}
