#include "dialog.h"
#include "mydlg2.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    MyDlg2 my1;
    if (my1.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }
    return 0;
}
