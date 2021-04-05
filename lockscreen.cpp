#include "lockscreen.h"
#include "ui_lockscreen.h"

#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QFile>

lockscreen::lockscreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lockscreen)
{
    ui->setupUi(this);

    // Stylesheet and style
    QFile stylesheetFile(":/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->timeLabel->setStyleSheet("font-size: 25pt");

    ui->unlockBtn->setProperty("type", "borderless");
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

    ui->unlockBtn->setStyleSheet("padding: 25px");
    ui->b0->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b1->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b2->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b3->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b4->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b5->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b6->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b7->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b8->setStyleSheet("padding: 35px; font-size: 12pt");
    ui->b9->setStyleSheet("padding: 35px; font-size: 12pt");

    // Invert screen if dark mode setting is set to "true"
    if(checkconfig("/opt/inkbox_genuine") == true) {
        if(checkconfig(".config/10-dark_mode/config") == true) {
            string_writeconfig("/tmp/invertScreen", "y");
        }
        else {
            string_writeconfig("/tmp/invertScreen", "n");
        }
    }

    // Checking if we have to set a passcode
    if(checkconfig("/tmp/setPasscode") == true) {
        set_passcode = true;
        string_writeconfig("/tmp/setPasscode", "false");
    }
    else {
        set_passcode = false;
        get_passcode();
        string_writeconfig("/tmp/setPasscode", "false");
    }

    if(set_passcode == true) {
        ui->welcomeLabel->setText("Type in a passcode");
        ui->unlockBtn->setText("Set passcode");
    }

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

                QFile::copy("wake.sh", "/external_root/tmp/wake.sh");

                QString prog ("chroot");
                QStringList args;
                args << "/external_root" << "/tmp/wake.sh";
                QProcess *proc = new QProcess();
                proc->startDetached(prog, args);
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
    string_checkconfig("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n") {
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
