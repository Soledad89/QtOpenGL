/********************************************************************************
** Form generated from reading UI file 'mymainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYMAINWINDOW_H
#define UI_MYMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyMainWindow
{
public:
    QAction *actionNew;
    QAction *actionExit;
    QAction *actionOpen;
    QAction *actionOpen_project;
    QAction *actionClose_project;
    QAction *actionUndo;
    QAction *actionCut;
    QAction *actionCopy;
    QWidget *centralWidget;
    QTextEdit *textEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MyMainWindow)
    {
        if (MyMainWindow->objectName().isEmpty())
            MyMainWindow->setObjectName(QString::fromUtf8("MyMainWindow"));
        MyMainWindow->resize(568, 359);
        actionNew = new QAction(MyMainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/File/images/fast.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon);
        actionExit = new QAction(MyMainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/File/images/sound_off.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon1);
        actionOpen = new QAction(MyMainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/File/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon2);
        actionOpen_project = new QAction(MyMainWindow);
        actionOpen_project->setObjectName(QString::fromUtf8("actionOpen_project"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/File/images/to_end.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_project->setIcon(icon3);
        actionClose_project = new QAction(MyMainWindow);
        actionClose_project->setObjectName(QString::fromUtf8("actionClose_project"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/File/images/step_backward.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose_project->setIcon(icon4);
        actionUndo = new QAction(MyMainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionCut = new QAction(MyMainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCopy = new QAction(MyMainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        centralWidget = new QWidget(MyMainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(221, 12, 256, 192));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(99, 34, 114, 32));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(99, 90, 114, 32));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(99, 146, 114, 32));
        MyMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MyMainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 568, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MyMainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MyMainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MyMainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MyMainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MyMainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionExit);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionOpen_project);
        menuFile->addAction(actionClose_project);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);

        retranslateUi(MyMainWindow);

        QMetaObject::connectSlotsByName(MyMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MyMainWindow)
    {
        MyMainWindow->setWindowTitle(QApplication::translate("MyMainWindow", "MyMainWindow", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MyMainWindow", "new", 0, QApplication::UnicodeUTF8));
        actionNew->setShortcut(QApplication::translate("MyMainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MyMainWindow", "exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("MyMainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MyMainWindow", "open", 0, QApplication::UnicodeUTF8));
        actionOpen_project->setText(QApplication::translate("MyMainWindow", "open project", 0, QApplication::UnicodeUTF8));
        actionClose_project->setText(QApplication::translate("MyMainWindow", "close project ", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("MyMainWindow", "Undo", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("MyMainWindow", "Cut", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("MyMainWindow", "Copy", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MyMainWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MyMainWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MyMainWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MyMainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MyMainWindow", "Edit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MyMainWindow: public Ui_MyMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYMAINWINDOW_H
