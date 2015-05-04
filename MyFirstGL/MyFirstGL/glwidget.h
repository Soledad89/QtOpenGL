#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>

namespace Ui {
class glWidget;
}

class glWidget : public QWidget
{
    Q_OBJECT

public:
    explicit glWidget(QWidget *parent = 0);
    ~glWidget();

private:
    Ui::glWidget *ui;
};

#endif // GLWIDGET_H
