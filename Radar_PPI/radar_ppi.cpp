#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <deque>

#include "radar_ppi.h"
#include "global.h"

using namespace std;

s_ppi_video_for_disp dispFilePkg[DISP_BUF_NUM];
FILE * datafile_header;
s_ppi_video_for_disp * dispbuf = dispFilePkg;
char* filename1 = "/home/soledad/scripts/Qt_OpenGl/Radar_PPI/radar1";
vector<deque<Point3i> > bitplane(2);

bool radarDataInput(char * filename);

Radar_PPI::Radar_PPI(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    if ( !radarDataInput(filename1)){
        printf("Data Input error");
        exit(0);
    }

    color_layer[0] = 0.5;
    color_layer[1] = 0.25;
    scan_strip = DISP_BUF_NUM;
    afterglow_width = 1024; //保留先前的1024条线
    step = 0;

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
    glPointSize(4.0);    //设置点的大小

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


    //读入数据
    size_t result = fread(dispbuf, sizeof(s_ppi_video_for_disp), DISP_BUF_NUM, datafile_header);
    if (result != DISP_BUF_NUM) {
        fclose(datafile_header);
        radarDataInput(filename1);
        step = 0;
    }

    float angle_per_strip = 360.0/4096.0;

    float x, y;
    for (int i = step; i < step + scan_strip; i++)
    {
        for (int j = 0; j < 600; j ++)
        {
            float angle_i =  i * angle_per_strip * PI / 180.0;
            x = sin(angle_i) * j / 600.0;
            y = cos(angle_i) * j / 600.0;

            Point3i point;
            point.index = dispFilePkg[i - step].znum;
            point.x = x;
            point.y = y;

            int brightness;
            //int layer;
            brightness = dispFilePkg[i - step].videodata[j];
            if (brightness > 215)
                 bitplane[0].push_back(point);
            else
                if (brightness > 190)
                     bitplane[1].push_back(point);

        }
    }

    for (int i = 0; i < 2; i++)
    {
        while(!bitplane[i].empty())
        {
           Point3i p;
           p = bitplane[i].front();
           if ( p.index < step - afterglow_width)
               bitplane[i].pop_front();
           else
               break;
        }

    }

    //画前面的引导线
    float angle_step =  angle_per_strip * (step + scan_strip) *  PI / 180.0;
    glColor3f(0.2f, 0.2f, 0.0f);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(sin(angle_step), cos(angle_step));
    glEnd();

    glBegin(GL_POINTS);
    glPointSize(1.0);
    //分别画三个位平面
    //位平面0
    float color_0layer = color_layer[0];
    glColor3f(color_0layer, color_0layer, 0.0);
    deque<Point3i> vPoint3f_0;
    vPoint3f_0 = bitplane[0];
    for (int i = 0; i < vPoint3f_0.size(); i++)
    {
        glVertex2d(vPoint3f_0[i].x, vPoint3f_0[i].y);
    }
    glEnd();
    //位平面1
    float color_1layer = color_layer[1];
    glColor3f(color_1layer, color_1layer, 0.0);
    deque<Point3i> vPoint3f_1;
    vPoint3f_1 = bitplane[1];
    for (int i = 0; i < vPoint3f_1.size(); i++)
    {
        glVertex2d(vPoint3f_1[i].x, vPoint3f_1[i].y);
    }
    glEnd();
//    //位平面2
//    float color_2layer = color_layer[2];
//    glColor3f(color_2layer, color_2layer, 0.0);
//    deque<Point3i> vPoint3f_2;
//    vPoint3f_2 = bitplane[2];
//    for (int i = 0; i < vPoint3f_2.size(); i++)
//    {
//        glVertex2d(vPoint3f_2[i].x, vPoint3f_2[i].y);
//    }
//    glEnd();


}

void Radar_PPI::timeOut()
{
//    d_angle -= 4.5f;
//    if (d_angle >= 360.0f)
//        d_angle = 0.0f;
//    dx  += DELTA_TRANGLE * NUM_TRANGLE / 360;
//    if (dx > 2.0 * PI)
//        dx = 0.0f;
    step += DISP_BUF_NUM;
    if (step >= 4096)
        step = 0;
    updateGL();
}

bool radarDataInput(char * filename)
{
    datafile_header = NULL;
    if( (datafile_header = fopen(filename,"r")) == NULL ){
            printf("error open pkg file!\n");
            return false;
            }
    memset(dispFilePkg, 0, sizeof(s_ppi_video_for_disp)* DISP_BUF_NUM);

    if (!dispbuf) return false;

    size_t result = fread(dispbuf, sizeof(s_ppi_video_for_disp), DISP_BUF_NUM, datafile_header);

    if (result != DISP_BUF_NUM) {
            printf ("Reading error!\n");
            return false;
    }
    return true;

}
