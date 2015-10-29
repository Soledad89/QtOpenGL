
#ifdef __APPLE__
    #include <GLUT/glut.h>      // OS X version of GLUT
    #define RADA1FILEPATH "/Users/wangbo1/scripts/QtOpenGL/Radar_PPI/radar1"
#elif _WIN32
    #include <GL/glut.h>         // Windows FreeGlut equivalent
    #define RADA1FILEPATH " /home/soledad/scripts/QtOpenGL/Radar_PPI/radar1"
#else
    #include <GL/glut.h> // Windows FreeGlut equivalent
    #define RADA1FILEPATH "/home/soledad/scripts/QtOpenGL/RadarPPI/RadarPPI/save/radar1"
#endif

#include <stdio.h>
#include <vector>
#include <deque>
#include <QPainter>
#include "radar_ppi.h"
#include "Myglobal.h"

using namespace std;



s_ppi_video_for_disp dispFilePkg[DISP_BUF_NUM];
FILE * datafile_header;
s_ppi_video_for_disp * dispbuf = dispFilePkg;
char* filename1 = RADA1FILEPATH;
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
    setFixedSize(600,600);
    //this->setWindowFlags(Qt::FramelessWindowHint);
}

void Radar_PPI::initializeGL()
{
}

void Radar_PPI::resizeGL(int width, int height)
{
}

void Radar_PPI::paintGL()
{

    QPainter painter(this);
           painter.drawLine(80, 100, 650, 500);
           painter.setPen(Qt::red);
           painter.drawRect(10, 10, 100, 400);
           painter.setPen(QPen(Qt::green, 5));
           painter.setBrush(Qt::blue);
           painter.drawEllipse(50, 150, 400, 200);
}
/*
void Radar_PPI::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(rect(), Qt::AlignCenter, "Qt");
}
*/
void Radar_PPI::timeOut()
{
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
