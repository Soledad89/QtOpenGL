#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QPixmap>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL( void );
    void resizeGL( int width, int height );
    void paintEvent( QPaintEvent *event );

    // 本类的成员函数
    void Init2D( void );
    void Init3D( void );
    void Draw2D( void );
    void Draw3D( void );

    void DrawCube( double length,
                   const QColor topFace,
                   const QColor buttomFace,
                   const QColor leftFace,
                   const QColor rightFace,
                   const QColor frontFace,
                   const QColor backFace );
    void DrawTetrahedron( double length,
                          const QColor sideFace1,
                          const QColor sideFace2,
                          const QColor sideFace3,
                          const QColor buttomFace );
signals:
    
public slots:
private:
    enum { Cube = 0, Tetrahedron, ListCount };
    GLuint          m_List[ListCount];
    QPixmap         m_Background;

    float           m_Angle;
};

#endif // GLWIDGET_H
