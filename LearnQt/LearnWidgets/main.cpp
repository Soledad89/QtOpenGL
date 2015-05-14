#include "mywidget.h"
#include <QApplication>
#include "mydialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWidget w;
    MyDialog dd;
    if (dd.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }

    return 0;
}
