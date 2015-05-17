#include "mywidget.h"
#include "ui_mywidget.h"
#include <QPixmap>
#include <QMovie>
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap("/Users/wangbo1/scripts/Qt_OpenGl/LearnQt/src/03/3-8/logo.png"));
    QMovie *movie = new QMovie("/Users/wangbo1/scripts/Qt_OpenGl/LearnQt/src/03/3-8/donghua.gif");

    // 在标签中添加动画
    ui->label->setMovie(movie);

    // 开始播放
    movie->start();
}

MyWidget::~MyWidget()
{
    delete ui;
}
