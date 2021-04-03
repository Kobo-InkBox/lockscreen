#include "lockscreen.h"
#include "ui_lockscreen.h"

#include <QTimer>
#include <QTime>

lockscreen::lockscreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lockscreen)
{
    ui->setupUi(this);
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

