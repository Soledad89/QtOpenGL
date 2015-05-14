#include "logindlg.h"
#include "ui_logindlg.h"
#include <QtGui>

loginDlg::loginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDlg)
{
    ui->setupUi(this);
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
}

loginDlg::~loginDlg()
{
    delete ui;
}

void loginDlg::on_loginButton_clicked()
{
    if(ui->usrLineEdit->text().trimmed() == tr("qt") && ui->pwdLineEdit->text()==tr("123456"))
        accept();
    else
    {

        QMessageBox::warning(this,tr("Warning"),tr("user name or password error!"),QMessageBox::Yes);
        ui->usrLineEdit->clear();//清空用户名输入框
        ui->pwdLineEdit->clear();//清空密码输入框
        ui->usrLineEdit->setFocus();//将光标转到用户名输入框
    }
}
