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


//    QPen pen(Qt::DotLine);
//    QBrush brush(Qt::blue);
//    brush.setStyle(Qt::HorPattern);
//    painter.setPen(pen);
//    painter.setBrush(brush);
//    painter.drawRect(100,100,200,200);

//    QPainter painter(this);
//    QLinearGradient linearGradient(100,150,300,150); //从点(100,150)开始到点(300,150)结束,确定一条直线
//    linearGradient.setColorAt(0,Qt::red);
//    linearGradient.setColorAt(0.2,Qt::black);
//    linearGradient.setColorAt(0.4,Qt::yellow);
//    linearGradient.setColorAt(0.6,Qt::white);
//    linearGradient.setColorAt(0.8,Qt::green);
//    linearGradient.setColorAt(1,Qt::blue); //将直线开始点设为 0,终点设为 1,然后分段设置颜色
//    painter.drawText(100,100,"yafeilinux");
//    painter.setBrush(linearGradient);
//    painter.drawRect(100,100,200,100); //绘制矩形,线性渐变线正好在矩形的水平中心线上

//    QPainterPath path;
//    path.addEllipse(100,100,50,50); path.lineTo(200,200);
//    QPainter painter(this);
//    painter.setPen(Qt::green);
//    painter.setBrush(Qt::yellow);
//    painter.drawPath(path);

//    QPainterPath path2;
//    path2.addPath(path);
//    path2.translate(100,0);
//    painter.drawPath(path2);


//    QPainterPath path3;
//    path3.lineTo(100,100);
//    path3.lineTo(200,100);

//    painter.drawPath(path3);
//    QPainter painter(this);
//    QPixmap pix;
//    pix.load("images/logo.jpg");
//    painter.drawPixmap(0,0,100,100,pix);

    QPainter painter(this);
    painter.setBrush(Qt::yellow);
    painter.drawRect(0,0,50,50);
    painter.translate(100,100); //将点(100,100)设为原点 painter.setBrush(Qt::red);
    painter.drawRect(0,0,50,50);
    painter.translate(-100,-100);
    painter.drawLine(0,0,20,20);
}
