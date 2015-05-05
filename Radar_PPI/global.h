#ifndef GLOBAL_H
#define GLOBAL_H

#define DISP_BUF_NUM 3600*7
#define CNTVIDEO 4*512
#define PI 3.1416
#define DELTA_TRANGLE 0.08789
#define NUM_TRANGLE  64

enum  DisplayType{
    TypePPI=0,
    TypeAR,
    TypeWIN,
    TypeTV,
    TypeOther
};

typedef unsigned char VIDEODATATYPE;
const int max_azi_num_per_video = 24;
typedef struct s_ppi_video_for_disp
{
    VIDEODATATYPE videodata[CNTVIDEO];
    int znum;
    int azicnt[max_azi_num_per_video];
}s_ppi_video_for_disp; //2148


#endif // GLOBAL_H
