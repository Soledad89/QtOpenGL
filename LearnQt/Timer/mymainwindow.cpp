#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QtCore>

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(timerUpDate()));
    timer->start(1000);
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

void MyMainWindow::timerUpDate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->label->setText(str);
}
