/*
���κʹ��ļ��м���3D����:
���ļ�����3Dģ�ͣ�����ƽ���Ĵ�һ��ģ�ͱ任Ϊ��һ��ģ�͡�
 */

#ifndef GLWIDGET25_H
#define GLWIDGET25_H

#include "glwidget.h"

struct Vertex1
{
    float x, y, z;
};

struct Object1
{
    int verts; // �����ж����ĸ���
    Vertex1 *points; // �����������ݵ�ָ��
};

class GLWidget25 : public GLWidget
{
public:
    GLWidget25(int timeInterval = 0, QWidget *parent = 0);
    ~GLWidget25();
protected:
    void allocateObject1(Object1 *o, int n);
    void freeObject1(Object1 *o);
    void readLine(FILE *f, char *string);
    void loadObject1(const char *fileName, Object1 *o);
    Vertex1 calculate(int i);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    GLfloat scaling;
    QPoint lastPos;
    GLfloat xrot, yrot, zrot; // ��ת�Ƕ�
    GLfloat xspeed, yspeed, zspeed; // ��ת�ٶ�
    GLfloat cx, cy, cz; // ����λ��
    int key; // ������ʶ��
    int step, steps; // �任�Ĳ���
    bool morph; // �Ƿ�ʹ�ñ���
    int maxVertices; // ���󶨵���
    Object1 morph1, morph2, morph3, morph4;
    Object1 helper, *sour, *dest;
};

#endif // GLWIDGET25_H
