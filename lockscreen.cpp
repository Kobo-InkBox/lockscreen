#include "lockscreen.h"
#include "ui_lockscreen.h"
#include "global.h"

#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <QThread>
#include <QDir>
#include <QDebug>
#include <QScreen>
#include <QCryptographicHash>

#include <QApplication>

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

lockscreen::lockscreen(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::lockscreen)
{
    ui->setupUi(this);
    qDebug() << "Setting up lockscreen";

    // Stylesheet
    QApplication::setStyle("windows");
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Nothing other works
    QRect screenSize = QGuiApplication::screens()[0]->geometry();
    int dialogX = (screenSize.width() / 5) * 3;
    int dialogY = (screenSize.height() / 5) * 3;
    int positionX = ((screenSize.width() - dialogX) / 2);
    int positionY = ((screenSize.height() - dialogY) / 2);
    qDebug() << dialogX << dialogY << positionX << positionY;

    this->move(positionX, positionY);
    this->setFixedSize(QSize(dialogX, dialogY));

    ui->b0->setProperty("type", "borderless");
    ui->b1->setProperty("type", "borderless");
    ui->b2->setProperty("type", "borderless");
    ui->b3->setProperty("type", "borderless");
    ui->b4->setProperty("type", "borderless");
    ui->b5->setProperty("type", "borderless");
    ui->b6->setProperty("type", "borderless");
    ui->b7->setProperty("type", "borderless");
    ui->b8->setProperty("type", "borderless");
    ui->b9->setProperty("type", "borderless");
    ui->acceptBtn->setProperty("type", "borderless");
    ui->delButton->setProperty("type", "borderless");
    ui->ShowPasswordButton->setProperty("type", "borderless");

    ui->ShowPasswordButton->setIcon(QIcon("://resources/show.png"));

    ui->passTextEdit->setStyleSheet("QLineEdit { selection-background-color: white; selection-color: black }");

    // Showing time
    if(checkconfig(".config/02-clock/config") == true) {
        QTimer *t = new QTimer(this);
        t->setInterval(500);
        connect(t, &QTimer::timeout, [&]() {
           QString time = QTime::currentTime().toString("hh:mm:ss");
           ui->timeLabel->setText(time);
        } );
        t->start();
    }
    else {
        QTimer *t = new QTimer(this);
        t->setInterval(500);
        connect(t, &QTimer::timeout, [&]() {
           QString time = QTime::currentTime().toString("hh:mm");
           ui->timeLabel->setText(time);
        } );
        t->start();
    }
    if(choosedBackground == Blank) {
        ui->frame->setStyleSheet(".QFrame{background-color: white; border: 4px solid black; border-radius: 10px;}");
    }
}

lockscreen::~lockscreen()
{
    delete ui;
}

void lockscreen::on_b1_clicked()
{
    addNumber("1");
}

void lockscreen::on_b2_clicked()
{
    addNumber("2");
}

void lockscreen::on_b3_clicked()
{
    addNumber("3");
}

void lockscreen::on_b4_clicked()
{
    addNumber("4");
}

void lockscreen::on_b5_clicked()
{
    addNumber("5");
}

void lockscreen::on_b6_clicked()
{
    addNumber("6");
}

void lockscreen::on_b7_clicked()
{
    addNumber("7");
}

void lockscreen::on_b8_clicked()
{
    addNumber("8");
}

void lockscreen::on_b9_clicked()
{
    addNumber("9");
}

void lockscreen::on_b0_clicked()
{
    addNumber("0");
    /*
    struct libevdev * dev = NULL;
    int fd = ::open("/dev/input/event1", O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    int rc = libevdev_new_from_fd(fd, &dev);
    if(libevdev_grab(dev, LIBEVDEV_GRAB) == 0) {
        qDebug() << "Grabbed touchscreen";
    }
    else {
        qDebug() << "ERROR: Failed to grab touchscreen";
    }

    if (rc < 0) {
        qDebug() << "ERROR: Failed to init libevdev";
    }
    */
}

void lockscreen::addNumber(QString number) {
    passcode.append(number);

    if(showPasscode == true) {
        ui->passTextEdit->setText(passcode);
    }
    else {
        int count = passcode.count();
        QString hiddedPasscode = "";
        for (int i = 0; i < count; i++) {
            hiddedPasscode.append("●");
        }
        ui->passTextEdit->setText(hiddedPasscode);
    }
}

void lockscreen::on_ShowPasswordButton_clicked()
{
    if(showPasscode == true) {
        showPasscode = false;
        addNumber("");
    }
    else {
        showPasscode = true;
        addNumber("");
    }
}
void lockscreen::on_delButton_clicked()
{
    passcode.chop(1);
    addNumber("");
}

void lockscreen::on_acceptBtn_clicked()
{
    QByteArray passwordBytes = readFileBytes(".config/12-lockscreen/password");
    QString saltBytes = readFile(".config/12-lockscreen/salt").replace("\n", "");

    QCryptographicHash hasher(QCryptographicHash::RealSha3_512);
    hasher.addData(passcode.toLocal8Bit() + saltBytes.toLocal8Bit());

    QByteArray hash = hasher.result();
    qDebug() << "Hash is: " << hash;

    if(hash == passwordBytes) {
        qDebug() << "Correct password provided";
        for(int pid: pidList) {
            unfreezeApp(pid);
        }
        qApp->quit();
    }
    else {
        qDebug() << "Password is incorect";
        passcode = "";
        addNumber("");
        QMessageBox::critical(this, tr("Invalid passcode"), tr("Invalid passcode. Please try again."));
    }
}
