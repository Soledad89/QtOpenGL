// Name:   CAR.h
// Author: Yan Guo
// Date:   Feb 4, 2013 in the canteen of Rohm Building

//�ļ����ܣ�

#include "ppidef.h"
#include "CGraphicsManager.h"

#ifndef _C_AR_H_
#define _C_AR_H_

class CAR
{
private:
	char VideoChannel;
	int winth;

	VIDEODATATYPE *video_data;          //������ʾ���״�ز�����
	int dispRadius;

	unsigned int colorAR;
	unsigned int backColor;

	unsigned char base; // ֻ�д��ڴ����޵��״����ݲŻᱻ��ʾ
	short range;        // �������ұߵ�ֵ�Ĵ�С
	int decayAR;        // ����˥���̶�

	short PPIRange;

	CGraphicsManager* pGraphicsManager;
	int* vertices;
	unsigned int* colors;

	short isOpen;       //�Ƿ���״����ݲ�
	char frost;
	short visibleAR[2];

	bool changeBackgroundColor;

public:
	CAR(CGraphicsManager* pGM, char videoChannel);
	~CAR();

	int ARSetGate(unsigned char theGate);
	int ARSetDist(short theDist);
	int ARSetSwitch(short theSwitch);
	int ARSetFrost(char theFrost);
	int ARSetColor(unsigned int theColor);
	int ARSetBackgroundColor(unsigned int theColor);
	int ARSetVisible(int surfth, short theSwitch);
	int ARSetTail(int theTail);

	int ARDraw(s_ppi_video_for_disp* dispdat);
	int ARClear(int surfth);
	int ARUpdate();

	int ARsetColorKey(unsigned int colorKey,BMPImageRec* pBackMap1,int x, int y);
	int ARsetPPIRange(short theDist);
};


#endif