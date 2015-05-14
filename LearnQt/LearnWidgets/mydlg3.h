#ifndef MYDLG3_H
#define MYDLG3_H

#include <QDialog>

namespace Ui {
class MyDlg3;
}

class MyDlg3 : public QDialog
{
    Q_OBJECT

public:
    explicit MyDlg3(QWidget *parent = 0);
    ~MyDlg3();

private:
    Ui::MyDlg3 *ui;
};

#endif // MYDLG3_H
