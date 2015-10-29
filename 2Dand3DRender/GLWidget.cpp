#include <QtGui>
#include <GL/glu.h>
#include "GLWidget.h"
/*---------------------------------------------------------------------------*/
GLWidget::GLWidget(QWidget *parent) :
    QGLWidget( parent )
{
    setFont( QFont( tr( "Aerial" ), 26 ) );

    Init2D( );
    Init3D( );
    m_Angle = 0.0f;

    // 设置窗口
    setAutoFillBackground( false );
    setAutoBufferSwap( false );
    setWindowTitle( tr( "2D/3D Complex Example" ) );
}
/*---------------------------------------------------------------------------*/
void GLWidget::initializeGL( void )
{
    // 隐式调用了makeCurrent()函数
    glEnable( GL_DEPTH );
    glFrontFace( GL_CW );
    glEnable( GL_CULL_FACE );
    glShadeModel( GL_FLAT );
}
/*---------------------------------------------------------------------------*/
void GLWidget::resizeGL( int width, int height )
{
    // 此函数只在窗口创建的时候调用，因为窗口大小设为固定的了
    // 设置视口
    glViewport( 0, 0, width, height );
    // 设置投影矩阵
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45.0,                       // 视野角度
                    double( width ) / double( height ),// 宽高比
                    1.0,                        // 近裁面z值
                    1000.0 );                   // 远裁面z值

    // 转为模型/视图矩阵
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::paintEvent( QPaintEvent* )
{
    Draw2D( );
    Draw3D( );
    swapBuffers( );
    update( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::Init2D( void )
{
    if ( m_Background.load( ":/Background.png" ) )
    {
        setFixedSize( m_Background.size( ) );
    }
    else
    {
        m_Background.scaled( 640, 480 );// 如果载入失败，强制让其为长640高480
    }
}
/*---------------------------------------------------------------------------*/
void GLWidget::Init3D( void )
{
    makeCurrent( );
    m_List[Cube] = glGenLists( ListCount );
    m_List[Tetrahedron] = m_List[Cube] + 1;
    glNewList( m_List[Cube], GL_COMPILE );
    DrawCube( 25.0, Qt::white,
              Qt::yellow,
              Qt::blue,
              Qt::red,
              Qt::green,
              Qt::cyan );
    glEndList( );

    glNewList( m_List[Tetrahedron], GL_COMPILE );
    DrawTetrahedron( 25.0,
                     Qt::yellow,
                     Qt::blue,
                     Qt::red,
                     Qt::green );
    glEndList( );
}
/*---------------------------------------------------------------------------*/
void GLWidget::Draw2D( void )
{
    QPainter painter( this );
    painter.drawPixmap( 0, 0, m_Background );
}
/*---------------------------------------------------------------------------*/
void GLWidget::Draw3D( void )
{
    const float x = 80.0f,//3.0f / 8.0f * size( ).rwidth( ),
            y = 40.0f;//size( ).rheight( ) / 4.0f;

    makeCurrent( );
    glPushMatrix( );
    glTranslatef( -x, y, -200.0f );
    glRotatef( m_Angle, 1.0f, 1.0f, 0.0f );
    glCallList( m_List[Cube] );
    glPopMatrix( );

    glPushMatrix( );
    glTranslatef( x, y, -200.0f );
    glRotatef( m_Angle, 1.0f, 1.0f, 0.0f );
    glCallList( m_List[Tetrahedron] );
    glPopMatrix( );

    glPushMatrix( );
    glTranslatef( x, -y, -200.0f );
    glRotatef( m_Angle, 1.0f, 1.0f, 0.0f );
    glCallList( m_List[Cube] );
    glPopMatrix( );

    glPushMatrix( );
    glTranslatef( -x, -y, -200.0f );
    glRotatef( m_Angle, 1.0f, 1.0f, 0.0f );
    glCallList( m_List[Tetrahedron] );
    glPopMatrix( );

    glPushMatrix( );
    glTranslatef( 0, -y, -200.0f );
    qglColor( Qt::black );
    renderText( -50.0, 0.0, 0.0,                // 字体所在的位置
                tr( "Made By Jiangcaiyang" ) ); // 文本
    glPopMatrix( );

    m_Angle += 1.0f;
}
