#include "radar_ppi.h"
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include "global.h"
s_ppi_video_for_disp dispFilePkg[DISP_BUF_NUM]; //数据量太大，debug段错误，全局变量不容易出错
FILE * rpkgfp;

bool radarDataInput(char * filename);

Radar_PPI::Radar_PPI(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    char* filename1 = "/home/soledad/scripts/Qt_OpenGl/Radar_PPI/radar1";
    if ( !radarDataInput(filename1)){
        printf("Data Input error");
        exit(0);
    }

    d_scanangle = 180.0f;
    d_angle = 180.0f;
    dx = 0.0f;
    setWindowTitle(tr("Radar PPI Display"));
    setFixedSize(1200,1200);
}

void Radar_PPI::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Radar_PPI::resizeGL(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Radar_PPI::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
    glLoadIdentity(); // 重置当前的模型观察矩阵
    glColor3f(0.0f, 1.0f, 0.0f);
    //打开抗锯齿，并提示尽可能进行最佳的处理
         glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
         glEnable(GL_BLEND);
         glEnable(GL_POINT_SMOOTH);
         glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
         glEnable(GL_LINE_SMOOTH);
         glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
         glEnable(GL_POLYGON_SMOOTH);
         glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
         glLineWidth(1.0);    //设置线宽
         glPointSize(10.0);    //设置点的大小

    glColor3f(0.0f, 1.0f, 0.0f);
    //绘制圆心
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    //绘制大圈
    float angle;
    for (angle = 0; angle <= PI * 2.0; angle += 0.0785f)
    {
        float x, y;
        x = sin(angle);
        y = cos(angle);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
    }
    glEnd();
//    //绘制中圈
//    for (angle = 0; angle <= PI * 2.0; angle +=  0.0785*1.5f)
//    {
//        float x, y;
//        x = 0.6f * sin(angle);
//        y = 0.6f * cos(angle);
//        glBegin(GL_LINE_LOOP);
//        glVertex2f(x, y);
//    }
//    glEnd();
//    //绘制小圈
//    for (angle = 0; angle <= PI * 2.0; angle +=  0.0785*1.5f)
//    {
//        float x, y;
//        x = 0.3f * sin(angle);
//        y = 0.3f * cos(angle);
//        glBegin(GL_LINE_LOOP);
//        glVertex2f(x, y);
//    }
//    glEnd();

//    //绘制一个扇区及扫描线
//    glLineWidth(1.0f);
//    glColor3f(1.0f, 1.0f, 1.0f);
//    float p, q;
//    int nTrangle = 4096 / NUM_TRANGLE; //512个扇区， 每个扇区32条线
//    for ( int i = 0; i < nTrangle; i++)
//    {
//        for ( int j = 0; j < NUM_TRANGLE; j++)
//        {
//            float a = dx + j * DELTA_TRANGLE / 360;
//            p = sin(a);// 转换为查表法???????????????????????????????????????/
//            q = cos(a);
//            glBegin(GL_LINES);
//            glVertex2f(0.0f, 0.0f);
//            glVertex2f(p, q);
//            glEnd();
//        }
//    }


    for (float i = 0.0f; i >= -d_scanangle; i -= 0.1f)
    {
        glColor3f(1.0+i/d_scanangle, 1.0+i/d_scanangle, 1.0+i/d_scanangle);
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(cos(PI*(d_angle - i)/ 180.0),  sin(PI*(d_angle-i) / 180.0), 0.0f);
        glEnd();
    }

    for (int i = 0; i < 360; i++)
    {
        for (int j = 0; j < 12; j ++)
        {
            for ( int k = 0; k < 600; k++)
            {
                glColor3f(1.0 - i/360, 1.0 - i/360, 1.0 - i/360);
                glBegin(GL_POINTS);
                glVertex3f(0.5f, 0.5f, 0.0f);
                glEnd();
            }
        }
    }

}

void Radar_PPI::timeOut()
{
    d_angle -= 4.5f;
    if (d_angle >= 360.0f)
        d_angle = 0.0f;
    dx  += DELTA_TRANGLE * NUM_TRANGLE / 360;
    if (dx > 2.0 * PI)
        dx = 0.0f;
    updateGL();
}

bool radarDataInput(char * filename)
{
    rpkgfp = NULL;
    if( (rpkgfp = fopen(filename,"r")) == NULL ){
            printf("error open pkg file!\n");
            return false;
            }
    memset(dispFilePkg, 0, sizeof(s_ppi_video_for_disp)* DISP_BUF_NUM);
    s_ppi_video_for_disp * dispbuf = dispFilePkg;
    if (!dispbuf) return false;

    size_t result = fread(dispbuf, sizeof(s_ppi_video_for_disp), DISP_BUF_NUM, rpkgfp);

    if (result != DISP_BUF_NUM) {
            printf ("Reading error!\n");
            return false;
    }
    return true;

}
