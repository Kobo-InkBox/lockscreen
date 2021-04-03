#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>

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
