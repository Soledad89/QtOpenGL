// Name:   CPPI.h
// Author: Yan Guo
// Date:   Feb 27, 2013 in the canteen of Rohm Building

//�ļ����ܣ�

#include "ppidef.h"
//#include "CWindowsManager.h"
#include "CGraphicsManager.h"
#include "CDataPool.h"
#include "bmp.h"

#ifndef _C_PPI_
#define _C_PPI_

#define NUM_PACK 10


class CPPI
{
private:
	char VideoChannel;
	int winth;

	VIDEODATATYPE *video_data;          //������ʾ���״�ز�����
	int dispRadius;      // ��ʾ�뾶
	int dispMode;        // ��ʾģʽ    0��B��   1��P��   2��E��
    int prowIndex;       // ������λ
	int hollowNum;       // ���ĵ�����ֵ
	short moveMode;      // ���˶�ģʽ

//	int WinMode;

	short xorg;
	short yorg;

	s_layer_unit ** layerPPI;
	s_layer_unit ** layerWin;

	DISPMEMTYPE *colortablePPI;    // PPI��ʾ��ɫ��
	DISPMEMTYPE *colortableWin;    // С������ʾ��ɫ��

	int scanLine;
	unsigned int *scanLineColor;
	unsigned int backColor;
	unsigned int backColorWin;

	unsigned char base;  // ���ޣ�ֻ�г��������޵��״����ݲŻ���ʾ


	// ��������range�;���distance�����Լ��������λ�ã�direction�����ϵڼ����㣩
	short range;        // PPI����
	short distanceWin;     // ��������
	float directionWin;    // ������λ
	int scaleWin;          // �����ķŴ�ϵ����������������ʹ��ڵĴ�С�ߴ磬����ȷ������ҳFBO�ж�����������С������


    int ** addr_near_x;
    int ** addr_near_y;
    int ** addr_far_x;
    int ** addr_far_y;

	CGraphicsManager* pGraphicsManager;
	int* vertices;
	int* scanLineVertices;
	unsigned int* colorsPPI;
	unsigned int* colorsWin;

	int numPack;   // ÿ�����ٸ�����Ⱦһ��
	int np;  // �����µĵ���
	int line;

	short isOpen;            // ���ƺ�ҳ���ƣ��ر�ʱ�������������ڹر�״̬ʱ��PPI��ҳ�㲻���ƣ����ӡ�
	char frost;              // ���ƺ�ҳ���ƣ�����ʱ���������������ڶ���״̬ʱ��PPI��ҳ�㲻���ƣ����ӡ��ⶳʱ����������
	short visiblePPI[3];     // ���ƴ��ڵ��ӣ��ر�ʱ���������������ڹر�״̬ʱ��PPI��ҳ��������ƣ������ӡ�
	short visibleWin[2];

	short comdisp;


public:
	CPPI(CDataPool *dp, CGraphicsManager* pGM, char videoChannel);
	~CPPI();

	//-----------------------------PPI---------------------------
	// ״̬����
	int PPISetDispMode(int mode);
	int PPISetDispCenter(short theX ,short theY);
	int	PPISetShipDir(float theShipDir);
	int PPISetScan(int theScan);
	int PPISetScanLineColor(unsigned int theColor);
	int PPISetFrost(char theFrost);
	int PPISetRange(short theDist);
	int PPISetSwitch(short theSwitch);
	int PPISetBackgroundColor(unsigned int theColor);
	int PPISetGate(unsigned char theGate);
	int PPISetVisible(int winth, int surfth, short theSwitch);
	int PPISetTogether(short theSwitch);

	// ͼ�λ���
	int PPIDraw(s_ppi_video_for_disp* dispdat);
	int PPIClear(int surfth);
	int PPIUpdate();
	int PPIDrawBackMap(BMPImageRec* pBackMap);

	int PPIClearLayer();
	int PPISetMoveMode(short theMode);

	//--------------------------MiniWin---------------------------
	// ״̬����
	int WinSetDistDir(short theDistance ,float theDirection);
	int WinSetScale(int theScale);
	int WinSetBackgroundColor(unsigned int theColor);
	
	// ͼ�λ���
	int WinClear(int surfth);
	int WinUpdate();

	int PPIsetColorKey(unsigned int theColor,BMPImageRec* pBackMap1, int x, int y);
	int WinSetColorKey(unsigned int theColor, BMPImageRec* pBackMap1, int x, int y);

private:
	int PPIPackProc(s_ppi_video_for_disp* dispdat);
	int PPILineProc(int azicnt);
	int BScopeLineProc( int azicnt );
	int EScopeLineProc( int azicnt );
	int WinPPILineProc(int azicnt);
	int WinBScopeLineProc( int azicnt );
};


#endif