#include "glwidget.h"
#include "ui_glwidget.h"

glWidget::glWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::glWidget)
{
    ui->setupUi(this);
}

glWidget::~glWidget()
{
    delete ui;
}
