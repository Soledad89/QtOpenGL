#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QPainter>

namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget *parent = 0);
    ~MyDialog();

private:
    Ui::MyDialog *ui;

    void paintEvent(QPaintEvent *);
};

#endif // MYDIALOG_H
