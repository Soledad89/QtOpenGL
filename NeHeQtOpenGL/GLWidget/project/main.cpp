#include "glwidget2.h"
#include "glwidget3.h"
#include "glwidget4.h"
#include "glwidget5.h"
#include "glwidget6.h"
#include "glwidget8.h"
#include "glwidget9.h"
#include "glwidget10.h"
#include "glwidget11.h"
#include "glwidget12.h"
#include "glwidget16.h"
#include "glwidget17.h"
#include "glwidget18.h"
#include "glwidget19.h"
#include "glwidget20.h"
#include "glwidget21.h"
#include "glwidget23.h"
#include "glwidget24.h"
#include "glwidget25.h"
#include "glwidget26.h"
#include "glwidget27.h"
#include "glwidget28.h"
#include "glwidget29.h"

int main(int argc, char *argv[])
{    
    Q_INIT_RESOURCE(GLWidget);

    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    qApp->addLibraryPath("plugins");

//    GLWidget *w2 = new GLWidget2;
//    GLWidget *w3 = new GLWidget3;
//    GLWidget *w4 = new GLWidget4(10);
//    GLWidget *w5 = new GLWidget5(10);
//    GLWidget *w6 = new GLWidget6(10);
//    //w2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    //w6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//    QWidget *widget = new QWidget;
//    QGridLayout *mainLayout = new QGridLayout;
//    mainLayout->addWidget(w2, 0, 0);
//    mainLayout->addWidget(w3, 0, 1);
//    mainLayout->addWidget(w4, 1, 0);
//    mainLayout->addWidget(w5, 1, 1);
//    mainLayout->addWidget(w6, 2, 0);
//    widget->setLayout(mainLayout);
//    widget->resize(1024, 768);
//    widget->showMaximized();

 // GLWidget2 widget2(20);
 //   widget2.showMaximized();

    //GLWidget3 widget3(20);
    //widget3.showMaximized();

//    GLWidget4 widget4(20);
//    widget4.showMaximized();

//    GLWidget5 widget5(20);
//   widget5.showMaximized();

 //   GLWidget6 widget6(20);
    //widget6.showMaximized();

 //  GLWidget8 widget8(8);
    //widget8.showMaximized();

 //   GLWidget9 widget9(20);
    //widget9.showMaximized();

//    GLWidget10 widget10(20);
 //    widget10.showMaximized();

 // GLWidget11 widget11(2);
 //   widget11.showMaximized();

 //GLWidget12 widget12(20);
    //widget12.showMaximized();

// GLWidget16 widget16(20);
 // widget16.showMaximized();


// GLWidget17 widget17(20);
// widget17.showMaximized();
 //widget17.setWindowFlags(Qt::FramelessWindowHint);

// GLWidget18 widget18(20);
   // widget18.showMaximized();

//GLWidget19 widget19(20);
//widget19.showMaximized();

 //   GLWidget20 widget20(20);
 //   widget20.showMaximized();

//    GLWidget21 widget21(21);
//    widget21.showMaximized();

  //GLWidget23 widget23(20);
   //widget23.showMaximized();

//   GLWidget24 widget24(20);
//    widget24.showMaximized();

  // GLWidget25 widget25(20);
 //  widget25.showMaximized();

 //   GLWidget26 widget26(20);
 //   widget26.showMaximized();

//GLWidget27 widget27(20);
 // widget27.showMaximized();

//    GLWidget28 widget28(20);
//    widget28.showMaximized();

// GLWidget29 widget29(20);
//  widget29.showMaximized();


    return a.exec();
}
