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

    bool setPasscode = false;
    bool showPasscode = false;
    QString passcode = "";

    void addNumber(QString number);

    QString readFile(QString file) {
        if(QFile::exists(file)) {
            QFile fileToRead(file);
            fileToRead.open(QIODevice::ReadOnly);
            QTextStream in (&fileToRead);
            QString content = in.readAll();
            return content;
        }
        else {
            return NULL;
        }
    }
    bool checkconfig(QString file) {
        if(QFile::exists(file)) {
            QFile config(file);
            config.open(QIODevice::ReadOnly);
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
        }
        else {
            return false;
        }
        return 0;
    };
    bool writeFile(QString filename, QString content) {
        QFile file(filename);
        if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << content;
            return true;
        }
        else {
            return false;
        }
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
    void on_showPasscodeBtn_clicked();
    void on_deleteBtn_clicked();
    void on_acceptBtn_clicked();

private:
    Ui::lockscreen *ui;
};
#endif // LOCKSCREEN_H
