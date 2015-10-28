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
	int yhLen;                  // ÿ�θ���ʱ������Ҫ��������˥���ĵ���   0: PPI1   1:PPI2
	int yhIndex;                // ����֮���λ��
	int yhDecay;                // ÿ�θ���ʱ������˥���ĳ̶�
	unsigned char tailGate;     // β�����ޣ�����˥����������ʱ�������ǳ�β����ɫ�����͵�˥������β��˥�����ɽ���
	short tailSwitch;           // β������
	int tailDecay;              // β������������ʱ��˥���ĳ̶�

	DISPMEMTYPE* colortablePPI;    // PPI��ʾ��ɫ��
	DISPMEMTYPE* colortableWin;    // PPI��ʾ��ɫ��
	DISPMEMTYPE* colortableTail;   // PPI��ʾ��ɫ��


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

