/********************************************************************************
** Form generated from reading UI file 'mydlg3.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYDLG3_H
#define UI_MYDLG3_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyDlg3
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *widget;

    void setupUi(QDialog *MyDlg3)
    {
        if (MyDlg3->objectName().isEmpty())
            MyDlg3->setObjectName(QString::fromUtf8("MyDlg3"));
        MyDlg3->resize(400, 300);
        buttonBox = new QDialogButtonBox(MyDlg3);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(290, 20, 81, 241));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        widget = new QWidget(MyDlg3);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(130, 60, 120, 80));

        retranslateUi(MyDlg3);
        QObject::connect(buttonBox, SIGNAL(accepted()), MyDlg3, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MyDlg3, SLOT(reject()));

        QMetaObject::connectSlotsByName(MyDlg3);
    } // setupUi

    void retranslateUi(QDialog *MyDlg3)
    {
        MyDlg3->setWindowTitle(QApplication::translate("MyDlg3", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MyDlg3: public Ui_MyDlg3 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYDLG3_H
