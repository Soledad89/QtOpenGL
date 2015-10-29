// main.cpp 主函数所在的文件
// 2013年1月10日 By jiangcaiyang

#include <QApplication>
#include <QTranslator>
#include "GLWidget.h"

int main( int argc, char** argv )
{
    QApplication app( argc, argv );
    QTranslator trans;
    trans.load( ":/zh_CN.qm" );
    app.installTranslator( &trans );

    // 显示主窗口
    GLWidget widget;
    widget.show( );

    return app.exec( );
}

