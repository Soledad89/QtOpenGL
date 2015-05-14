/********************************************************************************
** Form generated from reading UI file 'mydlg2.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYDLG2_H
#define UI_MYDLG2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_MyDlg2
{
public:
    QDialogButtonBox *buttonBox;
    QFrame *frame;

    void setupUi(QDialog *MyDlg2)
    {
        if (MyDlg2->objectName().isEmpty())
            MyDlg2->setObjectName(QString::fromUtf8("MyDlg2"));
        MyDlg2->resize(400, 300);
        buttonBox = new QDialogButtonBox(MyDlg2);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(290, 20, 81, 241));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        frame = new QFrame(MyDlg2);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(50, 20, 211, 191));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        retranslateUi(MyDlg2);
        QObject::connect(buttonBox, SIGNAL(accepted()), MyDlg2, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MyDlg2, SLOT(reject()));

        QMetaObject::connectSlotsByName(MyDlg2);
    } // setupUi

    void retranslateUi(QDialog *MyDlg2)
    {
        MyDlg2->setWindowTitle(QApplication::translate("MyDlg2", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MyDlg2: public Ui_MyDlg2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYDLG2_H
