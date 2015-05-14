#include "loginwidget.h"
#include <QApplication>
#include "logindlg.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWidget w;
    loginDlg ld;

    if (ld.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }

    return 0;
}
