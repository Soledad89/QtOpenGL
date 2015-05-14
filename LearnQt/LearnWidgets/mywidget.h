#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "mydlg3.h"

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MyWidget *ui;
    MyDlg3 mydl3;
};

#endif // MYWIDGET_H
