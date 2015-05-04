/********************************************************************************
** Form generated from reading UI file 'glwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLWIDGET_H
#define UI_GLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_glWidget
{
public:

    void setupUi(QWidget *glWidget)
    {
        if (glWidget->objectName().isEmpty())
            glWidget->setObjectName(QString::fromUtf8("glWidget"));
        glWidget->resize(400, 300);

        retranslateUi(glWidget);

        QMetaObject::connectSlotsByName(glWidget);
    } // setupUi

    void retranslateUi(QWidget *glWidget)
    {
        glWidget->setWindowTitle(QApplication::translate("glWidget", "glWidget", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class glWidget: public Ui_glWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLWIDGET_H
