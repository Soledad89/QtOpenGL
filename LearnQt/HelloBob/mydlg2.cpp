#include "mydlg2.h"
#include "ui_mydlg2.h"

MyDlg2::MyDlg2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDlg2)
{
    ui->setupUi(this);
}

MyDlg2::~MyDlg2()
{
    delete ui;
}
