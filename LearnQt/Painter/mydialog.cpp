#include "mydialog.h"
#include "ui_mydialog.h"

MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
}

MyDialog::~MyDialog()
{
    delete ui;
}

void MyDialog::paintEvent(QPaintEvent *) {
    //QPainter painter(this);
    //painter.drawLine(0,0,100,100);
//    QPainter painter(this);
////    QPen pen; //画笔
////    pen.setColor(QColor(255,0,0));
////    QBrush brush(QColor(0,255,0,125)); //画刷
////    painter.setPen(pen); //添加画笔
////    painter.setBrush(brush); //添加画刷
////    painter.drawRect(100,100,200,200); //绘制矩形

//    QPainter painter(this);
//    QPixmap pix(200,200);
//    pix.fill(Qt::red); //背景填充为红色
//    painter.drawPixmap(0,0,pix);

    QPainter painter(this);
    QPixmap pix(200,200);
    pix.fill(Qt::red); //背景填充为红色
    QPainter pp(&pix); //新建 QPainter 类对象,在 pix 上进行绘图
    pp.drawLine(0,0,50,50); //在 pix 上的(0,0)点和(50,50)点之间绘制直线
    painter.drawPixmap(100,100,pix);
}
