#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QtGui>

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);

    isSaved = false; //初始化文件为未保存过状态
    curFile = tr("未命名.txt"); //初始化文件名为“未命名.txt”
    setWindowTitle(curFile); //初始化主窗口的标题
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}
