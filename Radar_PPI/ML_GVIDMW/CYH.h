// Name:   CYH.h
// Author: Yan Guo
// Date:   Mar 7, 2013 in the canteen of Rohm Building

#include "CDataPool.h"
#include "CGraphicsManager.h"
#include "ppidef.h"


#ifndef _C_YH_H_
#define _C_YH_H_

//#define YH_NUM    3000

class CYH
{
private:
	char VideoChannel;
	int winth;

	int* randomx;
	int* randomy;

	s_layer_unit ** layerPPI;
	s_layer_unit ** layerWin;
	s_layer_unit ** layerAR;

	unsigned char base;
	int yhLen;                  // 每次更新时，所需要进行余晖衰减的点数   0: PPI1   1:PPI2
	int yhIndex;                // 更新之后的位置
	int yhDecay;                // 每次更新时，余晖衰减的程度
	unsigned char tailGate;     // 尾迹门限，余晖衰减到此门限时，将其标记成尾迹颜色，余晖的衰减按照尾迹衰减规律进行
	short tailSwitch;           // 尾迹开关
	int tailDecay;              // 尾迹点在做更新时，衰减的程度

	DISPMEMTYPE* colortablePPI;    // PPI显示颜色表
	DISPMEMTYPE* colortableWin;    // PPI显示颜色表
	DISPMEMTYPE* colortableTail;   // PPI显示颜色表


	CGraphicsManager* pGraphicsManager;
	int* vertices;
	unsigned int* colorsPPI;
	unsigned int* colorsWin;


public:
	CYH(CDataPool *dp, CGraphicsManager* pGM, char videoChannel);
	~CYH();

	int setPPIDecay(int theTail);
	int setPPITailSwitch(short theSwitch);
	int setPPITailGate(unsigned char theGate);
	int setPPITailTimer(int theTime);
	int setYHGate(unsigned char theGate);

	int yhPPI();
	//int yhUpdate();

private:
	VIDEODATATYPE decayData(VIDEODATATYPE data, VIDEODATATYPE decay);
};






#endif

