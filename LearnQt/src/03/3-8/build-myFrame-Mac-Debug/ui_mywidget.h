/********************************************************************************
** Form generated from reading UI file 'mywidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWIDGET_H
#define UI_MYWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QListWidget>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyWidget
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *label_2;
    QWidget *page_2;
    QLabel *label_3;
    QLabel *label;
    QLCDNumber *lcdNumber;
    QToolBox *toolBox;
    QWidget *page_3;
    QWidget *page_4;
    QWidget *page_5;
    QFrame *frame;
    QListWidget *listWidget;

    void setupUi(QWidget *MyWidget)
    {
        if (MyWidget->objectName().isEmpty())
            MyWidget->setObjectName(QString::fromUtf8("MyWidget"));
        MyWidget->resize(597, 472);
        stackedWidget = new QStackedWidget(MyWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(130, 260, 131, 181));
        stackedWidget->setFrameShape(QFrame::StyledPanel);
        stackedWidget->setFrameShadow(QFrame::Plain);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        label_2 = new QLabel(page);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 60, 54, 12));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        label_3 = new QLabel(page_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 80, 81, 16));
        stackedWidget->addWidget(page_2);
        label = new QLabel(MyWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 100, 121, 111));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(false);
        label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        lcdNumber = new QLCDNumber(MyWidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(260, 10, 231, 51));
        lcdNumber->setSmallDecimalPoint(true);
        lcdNumber->setDigitCount(7);
        lcdNumber->setProperty("value", QVariant(456.123));
        toolBox = new QToolBox(MyWidget);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        toolBox->setGeometry(QRect(360, 130, 151, 291));
        toolBox->setFrameShape(QFrame::Box);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setGeometry(QRect(0, 0, 149, 187));
        toolBox->addItem(page_3, QString::fromUtf8("\345\245\275\345\217\213"));
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        page_4->setGeometry(QRect(0, 0, 149, 187));
        toolBox->addItem(page_4, QString::fromUtf8("\351\273\221\345\220\215\345\215\225"));
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        page_5->setGeometry(QRect(0, 0, 149, 187));
        toolBox->addItem(page_5, QString::fromUtf8("\351\231\214\347\224\237\344\272\272"));
        frame = new QFrame(MyWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(40, 10, 120, 80));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        frame->setLineWidth(5);
        frame->setMidLineWidth(10);
        listWidget = new QListWidget(MyWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(30, 250, 81, 201));

        retranslateUi(MyWidget);
        QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

        stackedWidget->setCurrentIndex(1);
        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MyWidget);
    } // setupUi

    void retranslateUi(QWidget *MyWidget)
    {
        MyWidget->setWindowTitle(QApplication::translate("MyWidget", "MyWidget", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MyWidget", "\347\254\254\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MyWidget", "\347\254\254\344\272\214\351\241\265", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MyWidget", "123", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("MyWidget", "\345\245\275\345\217\213", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_4), QApplication::translate("MyWidget", "\351\273\221\345\220\215\345\215\225", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_5), QApplication::translate("MyWidget", "\351\231\214\347\224\237\344\272\272", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MyWidget", "\347\254\254\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("MyWidget", "\347\254\254\344\272\214\351\241\265", 0, QApplication::UnicodeUTF8));
        listWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class MyWidget: public Ui_MyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWIDGET_H
