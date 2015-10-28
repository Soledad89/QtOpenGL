

//******************************
//
//  author : zhaolimin, lishidan
//
//  create time: 2007-9-5
//
//  last modify time:2007-10-16
//
//  Version: 1.3
//
//******************************

#ifndef _DATAPOOL_H_
#define _DATAPOOL_H_

#include "ppidef.h"

class CDataPool
{
public:
	//----坐标扫描变换表-----
    int** addr_near_x;
    int** addr_near_y;
    int** addr_far_x;
    int** addr_far_y;
    
	//--------余辉表---------
    int* randomx;
    int* randomy;

	//--------颜色表---------
	DISPMEMTYPE* colortablePPI[2];    // PPI显示颜色表
	DISPMEMTYPE* colortableWin[2];    // 小窗口显示颜色表
	DISPMEMTYPE* colortableTail[2];   // PPI显示颜色表
	//DISPMEMTYPE colorAR;           // AR线的颜色


    //-----视频帧缓冲区------
    s_layer_unit** layerPPI[2];          //每个窗口拥有自己的一个layer（CPU端内存），此layer存储的数据和显示频上的数据是对应的，用于比大写入
	s_layer_unit** layerWin[2];
	s_layer_unit** layerAR[2];


public:
    CDataPool();
    ~CDataPool();
	int setColorTable(DISPMEMTYPE* colortable, unsigned int theColor);
	int setColorTable(DISPMEMTYPE* colortable, unsigned int* colorTable);

private:
    int loadDataTable();     //上载两张表：坐标扫描变换表和余晖随机表

};

#endif

