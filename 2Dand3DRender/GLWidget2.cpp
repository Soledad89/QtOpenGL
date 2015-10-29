// GLWidget2.cpp 实现绘制图形的两个函数
// 2013年1月11日 By jiangcaiyang

#include "GLWidget.h"
/*---------------------------------------------------------------------------*/
void GLWidget::DrawCube( double length,
                         const QColor topFace,
                         const QColor buttomFace,
                         const QColor leftFace,
                         const QColor rightFace,
                         const QColor frontFace,
                         const QColor backFace )
{
    const double p1[3] = { -length / 2.0, -length / 2.0, length / 2.0 };
    const double p2[3] = { length / 2.0, -length / 2.0, length / 2.0 };
    const double p3[3] = { length / 2.0, -length / 2.0, -length / 2.0 };
    const double p4[3] = { -length / 2.0, -length / 2.0, -length / 2.0 };
    const double p5[3] = { -length / 2.0, length / 2.0, length / 2.0 };
    const double p6[3] = { length / 2.0, length / 2.0, length / 2.0 };
    const double p7[3] = { length / 2.0, length / 2.0, -length / 2.0 };
    const double p8[3] = { -length / 2.0, length / 2.0, -length / 2.0 };

    glBegin( GL_QUADS );

    // 上
    qglColor( topFace );
    glVertex3dv( p8 ); glVertex3dv( p7 );
    glVertex3dv( p6 ); glVertex3dv( p5 );

    // 下
    qglColor( buttomFace );
    glVertex3dv( p1 ); glVertex3dv( p2 );
    glVertex3dv( p3 ); glVertex3dv( p4 );

    // 左
    qglColor( leftFace );
    glVertex3dv( p8 ); glVertex3dv( p5 );
    glVertex3dv( p1 ); glVertex3dv( p4 );

    // 右
    qglColor( rightFace );
    glVertex3dv( p6 ); glVertex3dv( p7 );
    glVertex3dv( p3 ); glVertex3dv( p2 );

    // 前
    qglColor( frontFace );
    glVertex3dv( p5 ); glVertex3dv( p6 );
    glVertex3dv( p2 ); glVertex3dv( p1 );

    // 后
    qglColor( backFace );
    glVertex3dv( p7 ); glVertex3dv( p8 );
    glVertex3dv( p4 ); glVertex3dv( p3 );
    glEnd( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::DrawTetrahedron( double length,
                      const QColor sideFace1,
                      const QColor sideFace2,
                      const QColor sideFace3,
                      const QColor buttomFace )
{
    const double sqrt2 = 1.414213562;
    const double sqrt6 = 2.449489743;
    const double p1[3] = { 0.0, length, 0.0 };
    const double p2[3] = { -sqrt6 / 3.0 * length, -length / 3.0, sqrt2 / 3.0 * length };
    const double p3[3] = { sqrt6 / 3.0 * length, -length / 3.0, sqrt2 / 3.0 * length };
    const double p4[3] = { 0.0, -length / 3.0, -2 * sqrt2 / 3.0 * length };

    glBegin( GL_TRIANGLES );
    // 侧面一、二、三
    qglColor( sideFace1 );
    glVertex3dv( p1 ); glVertex3dv( p3 ); glVertex3dv( p2 );
    qglColor( sideFace2 );
    glVertex3dv( p1 ); glVertex3dv( p4 ); glVertex3dv( p3 );
    qglColor( sideFace3 );
    glVertex3dv( p1 ); glVertex3dv( p2 ); glVertex3dv( p4 );

    // 底面
    qglColor( buttomFace );
    glVertex3dv( p2 ); glVertex3dv( p3 ); glVertex3dv( p4 );
    glEnd( );
}
