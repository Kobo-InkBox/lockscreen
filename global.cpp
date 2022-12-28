#include "global.h"

#include <QString>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>

#include <signal.h>

option chosenBackground = option::blank;

bool checkconfig(QString file) {
    QFile config(file);
    config.open(QIODevice::ReadWrite);
    QTextStream in (&config);
    const QString content = in.readAll();
    std::string contentstr = content.toStdString();
    if(contentstr.find("true") != std::string::npos) {
        return true;
    }
    else {
        return false;
    }
    config.close();
};

void setDefaultWorkDir() {
    QDir::setCurrent("/mnt/onboard/.adds/inkbox");
}

QString readFile(QString file) {
   if(QFile::exists(file)) {
       QFile fileToRead(file);
       fileToRead.open(QIODevice::ReadOnly);
       QTextStream in (&fileToRead);
       QString content = in.readAll();
       qDebug() << "File: " << file << " has strings: " << content;
       return content;
   }
   else {
       return NULL;
   }
}

QByteArray readFileBytes(QString file) {
   if(QFile::exists(file)) {
       QFile fileToRead(file);
       fileToRead.open(QIODevice::ReadOnly);
       QByteArray content = fileToRead.readAll();
       qDebug() << "File: " << file << " has bytes: " << content;
       return content;
   }
   else {
       return NULL;
   }
}

QString launchProcess(QString path, QStringList args) {
    QProcess *process = new QProcess();
    process->start(path, args);
    process->waitForFinished();
    process->deleteLater();
    return QString(process->readAllStandardOutput());
}

int getPidByName(QString name) {
    QDirIterator appsDirectory("/proc", QDirIterator::NoIteratorFlags);
    while (appsDirectory.hasNext()) {
        QDir directory(appsDirectory.next());

        // This doesn't work, but it is literally copied code from InkBox...
        // QFile process = QFile(dir.path() + "/cmdline");
        QFile processPath;
        processPath.setFileName(directory.path() + "/cmdline");

        if(processPath.exists() == true) {
            processPath.open(QIODevice::ReadOnly);
            QTextStream stream(&processPath);
            if(stream.readLine().contains(name) == true) {
                processPath.close();
                qDebug() << "PID found: " << directory.dirName();
                return QVariant(directory.dirName()).toInt();
            }
            processPath.close();
        }
    }
    qDebug() << "PID wasn't found";
    return NULL;
}

QVector<int> pidList;

void freezeApp(int pid) {
    kill(pid, SIGSTOP);
    qDebug() << "Freezing " << pid;
}

void unfreezeApp(int pid) {
    kill(pid, SIGCONT);
    qDebug() << "Unfreezing " << pid;
}
