#ifndef RADAR_PPI_H
#define RADAR_PPI_H

#include "glwidget.h"

class Radar_PPI : public GLWidget
{         
public:
    Radar_PPI(int timerInterval = 0, QWidget *parent = 0);

protected:
    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void paintGL();
    virtual void timeOut();

private:
    float dx;
    float d_angle;
    float d_scanangle;

};

#endif // Radar_PPI_H
