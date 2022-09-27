#ifndef LOCKSCREENDIALOG_H
#define LOCKSCREENDIALOG_H

#include <QDialog>

namespace Ui {
class lockscreenDialog;
}

class lockscreenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit lockscreenDialog(QWidget *parent = nullptr);
    ~lockscreenDialog();

private:
    Ui::lockscreenDialog *ui;
};

#endif // LOCKSCREENDIALOG_H
