#include "lockscreen.h"
#include "ui_lockscreen.h"

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

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

lockscreen::lockscreen(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::lockscreen)
{
    ui->setupUi(this);
    qDebug() << "Setting up lockscreen";

    // Nothing other works
    QRect screenSize = QGuiApplication::screens()[0]->geometry();
    int dialogX = (screenSize.width() / 5) * 3;
    int dialogY = (screenSize.height() / 5) * 3;
    int positionX = ((screenSize.width() - dialogX) / 2);
    int positionY = ((screenSize.height() - dialogY) / 2);
    qDebug() << dialogX << dialogY << positionX << positionY;

    this->move(positionX, positionY);
    this->setFixedSize(QSize(dialogX, dialogY));

    // Stylesheet and style
    QFile stylesheetFile(":/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

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
}

lockscreen::~lockscreen()
{
    delete ui;
}

void lockscreen::setInitialBrightness() {
    setDefaultWorkDir();
    string_checkconfig_ro(".config/03-brightness/config");
    int brightness_value = checkconfig_str_val.toInt();
    string_writeconfig("/tmp/inkbox-cinematicBrightness_ran", "true");
    cinematicBrightness(brightness_value, 0);
}

void lockscreen::on_b1_clicked()
{
    passcode.append("1");
}

void lockscreen::on_b2_clicked()
{
    passcode.append("2");
}

void lockscreen::on_b3_clicked()
{
    passcode.append("3");
}

void lockscreen::on_b4_clicked()
{
    passcode.append("4");
}

void lockscreen::on_b5_clicked()
{
    passcode.append("5");
}

void lockscreen::on_b6_clicked()
{
    passcode.append("6");
}

void lockscreen::on_b7_clicked()
{
    passcode.append("7");
}

void lockscreen::on_b8_clicked()
{
    passcode.append("8");
}

void lockscreen::on_b9_clicked()
{
    passcode.append("9");
}

void lockscreen::on_b0_clicked()
{
    passcode.append("0");
}

void lockscreen::on_unlockBtn_clicked()
{
    if(passcode.size() != 8) {
        QMessageBox::critical(this, tr("Invalid argument"), tr("The passcode must be 8 characters long."));
        passcode = "";
    }
    else {
        if(set_passcode == true) {
            // TODO: find a better obfuscation technique
            string passcode_str = passcode.toStdString();
            string_writeconfig("/tmp/passcode", passcode_str);
            QString prog ("dd");
            QStringList args;
            args << "if=/tmp/passcode" << "of=/dev/mmcblk0" << "bs=256" << "seek=159745";
            QProcess *proc = new QProcess();
            proc->start(prog, args);
            proc->waitForFinished();
            QFile::remove("/tmp/passcode");
            passcode = "";

            QProcess process;
            process.startDetached("inkbox", QStringList());
            qApp->quit();
        }
        else {
            int setPasscode = get_passcode();
            passcode_int = passcode.toInt();
            if(passcode_int == setPasscode) {
                // Just in case
                passcode = "";

                if(checkconfig("/inkbox/bookIsEpub") == true) {
                    ;
                }
                else {
                    QFile::copy("splash.sh", "/external_root/tmp/splash.sh");

                    // Displaying previous framebuffer shot to speed things up
                    QString prog ("chroot");
                    QStringList args;
                    args << "/external_root" << "/tmp/splash.sh";
                    QProcess *proc = new QProcess();
                    proc->start(prog, args);
                    proc->waitForFinished();
                }

                // "Waking" InkBox
                QString wakeProg ("sh");
                QStringList wakeArgs;
                wakeArgs << "wake.sh";
                QProcess *wakeProc = new QProcess();
                wakeProc->startDetached(wakeProg, wakeArgs);
                qApp->quit();
            }
            else {
                QMessageBox::critical(this, tr("Invalid passcode"), tr("Invalid passcode. Please try again."));
                passcode = "";
            }
        }
    }
}

int lockscreen::get_passcode() {
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n" or checkconfig_str_val == "n613\n") {
        QString prog ("dd");
        QStringList args;
        args << "if=/dev/mmcblk0" << "bs=256" << "skip=159745" << "count=1" << "status=none";
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();

        QString procOutput = proc->readAllStandardOutput();
        procOutput = procOutput.left(8);

        int setPasscode = procOutput.toInt();
        return setPasscode;
    }
}

void lockscreen::set_brightness(int value) {
    std::ofstream fhandler;
    fhandler.open("/var/run/brightness");
    fhandler << value;
    fhandler.close();
}

void lockscreen::set_brightness_ntxio(int value) {
    // Thanks to Kevin Short for this (GloLight)
    /*
    int light;
    if((light = open("/dev/ntx_io", O_RDWR)) == -1) {
            fprintf(stderr, "Error opening ntx_io device\n");
    }
    ioctl(light, 241, value);
    */
}

int lockscreen::get_brightness() {
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n613") {
        string_checkconfig_ro(".config/03-brightness/config");
        int brightness;
        if(checkconfig_str_val == "") {
            brightness = 0;
        }
        else {
            brightness = checkconfig_str_val.toInt();
        }
        return brightness;
    }
    else {
        QFile brightness("/var/run/brightness");
        brightness.open(QIODevice::ReadOnly);
        QString valuestr = brightness.readAll();
        int value = valuestr.toInt();
        brightness.close();
        return value;
    }
    return 0;
}

void lockscreen::pre_set_brightness(int brightnessValue) {
    string_checkconfig_ro("/opt/inkbox_device");

    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
        set_brightness(brightnessValue);
    }
    else if(checkconfig_str_val == "n613\n") {
        set_brightness_ntxio(brightnessValue);
    }
    else {
        set_brightness(brightnessValue);
    }
}
void lockscreen::cinematicBrightness(int value, int mode) {
    /* mode can be 0 or 1, respectively
     * 0: Bring UP brightness
     * 1: Bring DOWN brightness
    */
    if(mode == 0) {
        int brightness = 0;
        while(brightness != value) {
            brightness = brightness + 1;
            pre_set_brightness(brightness);
            QThread::msleep(30);
        }
    }
    else {
        int brightness = get_brightness();
        while(brightness != 0) {
            brightness = brightness - 1;
            pre_set_brightness(brightness);
            QThread::msleep(30);
        }
    }
}

void lockscreen::setDefaultWorkDir() {
    QDir::setCurrent("/mnt/onboard/.adds/inkbox");
}
