#include "lockscreen.h"
#include "ui_lockscreen.h"

#include <QTimer>
#include <QTime>
#include <QDebug>

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

    ui->timeLabel->setStyleSheet("font-size: 20pt");

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

    ui->unlockBtn->setStyleSheet("padding: 20px");
    ui->b0->setStyleSheet("padding: 35px");
    ui->b1->setStyleSheet("padding: 35px");
    ui->b2->setStyleSheet("padding: 35px");
    ui->b3->setStyleSheet("padding: 35px");
    ui->b4->setStyleSheet("padding: 35px");
    ui->b5->setStyleSheet("padding: 35px");
    ui->b6->setStyleSheet("padding: 35px");
    ui->b7->setStyleSheet("padding: 35px");
    ui->b8->setStyleSheet("padding: 35px");
    ui->b9->setStyleSheet("padding: 35px");

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
    qDebug() << passcode;
}
