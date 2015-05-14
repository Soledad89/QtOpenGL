/********************************************************************************
** Form generated from reading UI file 'logindlg.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDLG_H
#define UI_LOGINDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_loginDlg
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *usrLineEdit;
    QLineEdit *pwdLineEdit;
    QPushButton *loginButton;
    QPushButton *exitButton;

    void setupUi(QDialog *loginDlg)
    {
        if (loginDlg->objectName().isEmpty())
            loginDlg->setObjectName(QString::fromUtf8("loginDlg"));
        loginDlg->resize(400, 300);
        label = new QLabel(loginDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 80, 45, 16));
        label_2 = new QLabel(loginDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 120, 44, 16));
        usrLineEdit = new QLineEdit(loginDlg);
        usrLineEdit->setObjectName(QString::fromUtf8("usrLineEdit"));
        usrLineEdit->setGeometry(QRect(130, 80, 181, 21));
        pwdLineEdit = new QLineEdit(loginDlg);
        pwdLineEdit->setObjectName(QString::fromUtf8("pwdLineEdit"));
        pwdLineEdit->setGeometry(QRect(130, 120, 181, 21));
        loginButton = new QPushButton(loginDlg);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(60, 220, 114, 32));
        exitButton = new QPushButton(loginDlg);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(210, 220, 114, 32));

        retranslateUi(loginDlg);
        QObject::connect(exitButton, SIGNAL(clicked()), loginDlg, SLOT(close()));

        QMetaObject::connectSlotsByName(loginDlg);
    } // setupUi

    void retranslateUi(QDialog *loginDlg)
    {
        loginDlg->setWindowTitle(QApplication::translate("loginDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("loginDlg", "\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("loginDlg", "\345\257\206\347\240\201\357\274\232 ", 0, QApplication::UnicodeUTF8));
        loginButton->setText(QApplication::translate("loginDlg", "login", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("loginDlg", "exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class loginDlg: public Ui_loginDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDLG_H
