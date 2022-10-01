#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class lockscreen; }
QT_END_NAMESPACE

using namespace std;

class lockscreen : public QDialog
{
    Q_OBJECT

public:
    lockscreen(QDialog *parent = nullptr);
    ~lockscreen();

    bool showPasscode = false;
    QString passcode = "";

    void addNumber(QString number);


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

    void on_ShowPasswordButton_clicked();

    void on_delButton_clicked();

    void on_acceptBtn_clicked();

private:
    Ui::lockscreen *ui;
};
#endif // LOCKSCREEN_H
