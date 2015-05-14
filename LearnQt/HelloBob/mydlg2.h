#ifndef MYDLG2_H
#define MYDLG2_H

#include <QDialog>

namespace Ui {
class MyDlg2;
}

class MyDlg2 : public QDialog
{
    Q_OBJECT

public:
    explicit MyDlg2(QWidget *parent = 0);
    ~MyDlg2();

private:
    Ui::MyDlg2 *ui;
};

#endif // MYDLG2_H
