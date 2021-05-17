#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class lockscreen; }
QT_END_NAMESPACE

using namespace std;

class lockscreen : public QMainWindow
{
    Q_OBJECT

public:
    lockscreen(QWidget *parent = nullptr);
    ~lockscreen();
    QString passcode;
    QString checkconfig_str_val;
    int passcode_int;

    bool set_passcode = false;
    bool checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        string contentstr = content.toStdString();
        if(contentstr.find("true") != std::string::npos) {
            return true;
        }
        else {
            return false;
        }
        config.close();
    };
    void string_checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
    void string_checkconfig_ro(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadOnly);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
    void string_writeconfig(string file, string config_option) {
        ofstream fhandler;
        fhandler.open(file);
        fhandler << config_option;
        fhandler.close();
    }
    int get_passcode();

private slots:
    void on_b1_clicked();

    void on_b2_clicked();

    void on_b3_clicked();

    void on_b4_clicked();

    void on_b5_clicked();

    void on_b6_clicked();

    void on_b7_clicked();

    void on_b8_clicked();

    void on_b9_clicked();

    void on_b0_clicked();

    void on_unlockBtn_clicked();

private:
    Ui::lockscreen *ui;
};
#endif // LOCKSCREEN_H
