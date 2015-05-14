#include "mydlg3.h"
#include "ui_mydlg3.h"

MyDlg3::MyDlg3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDlg3)
{
    ui->setupUi(this);
}

MyDlg3::~MyDlg3()
{
    delete ui;
}
