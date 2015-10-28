// Name:   CRadarDisplay.h
// Author: Yan Guo
// Date:   Feb 27, 2013 in the canteen of Rohm Building


#include "ppidef.h"
#include "CWindowsManager.h"
#include "CGraphicsManager.h"
#include "CAR.h"
#include "CPPI.h"

#ifndef _C_RADAR_DISPLAY_H_
#define _C_RADAR_DISPLAY_H_

#define QUEUE_LEN    64
#define PKG_NUM      16

#define ID_TIMER_PPI  1
#define ID_TIMER_YH   2 

class CRadarDisplay
{
private:
	s_ppi_video_for_disp* dispbuf;          // ָ��Ҫ���Ե��ڲ��״�����

	CGraphicsManager* pGraphicsManager;
	CWindowsManager* pWindowsManager;
	//CDataPool* pDataPool;


public:
	CRadarDisplay();
	~CRadarDisplay();


	//--------------------------PPI����-----------------------------------------
	
	// ���ڿ���
	int ML_GVID_OpenPPI(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
	int ML_GVID_ClosePPI(char VideoChannel);
	int ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY);
	int ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight);

	// ���ؿ���
	int ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch);                   // ֻ���ƺ�ҳ���ƣ��ر�ʱ�������������ڹر�״̬ʱ��PPI��ҳ�㲻���ƣ����ӡ�
	int ML_GVID_SetPPIFrost(char VideoChannel,char theFrost);                      // ֻ���ƺ�ҳ���ƣ�����ʱ���������������ڶ���״̬ʱ��PPI��ҳ�㲻���ƣ����ӡ��ⶳʱ����������
	int ML_GVID_SetPPIScan(char VideoChannel,int theScan);                         // �����Ƿ���ʾɨ����
	int ML_GVID_SetPPITailSwitch(char VideoChannel,short theSwitch);               // �����Ƿ��β������

	// ��ɫ����
	int ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor);               
	int ML_GVID_SetPPIColorTable(char VideoChannel,unsigned int* colorTable);
	int ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor);
	int ML_GVID_SetPPIBackgroundColor(char VideoChannel,unsigned int theColor);
	int ML_GVID_SetPPITailColor(char VideoChannel,unsigned int theColor);

	int ML_GVID_SetPPIMode(char VideoChannel, short theMode);                       // PPI������ʾģʽ    0��P��   1��B��   2��E��
	int ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY);            // P������λ��
	int ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir);                  // ������λ
	int ML_GVID_SetPPIDist(char VideoChannel,short theDist);                        // ����
	int ML_GVID_SetMoveMode(char VideoChannel, short theMode);

	int ML_GVID_SetPPIGate(char VideoChannel,unsigned char theGate);                // ���ޣ�ֻ�г��������޵��״����ݲŻ���ʾ��̫С���״����ݲ�������ʾ
	int ML_GVID_SetPPITailGate(char VideoChannel,unsigned char theGate);            // β�����ޣ�����������Ŀ�꣬������˥����������ֵʱ��������β����־���Ժ��˥����β��˥�����������У�ֱ����һ���ϴ��Ŀ�꽫�串��

	int ML_GVID_SetPPITail(char VideoChannel,int theTail);                          // ����˥���̶ȣ� theTailֵԽ��˥��Խǿ
	int ML_GVID_SetPPITailTimer(char VideoChannel,int theTime);                     // β��˥���̶ȣ� theTimeֵԽ��˥��Խǿ

	int ML_GVID_ClearPPI(char VideoChannel);                                        // ��PPI�״����ݲ�

	int ML_GVID_SetPPITogether(short theSwitch);                                    // ��·�״���Ƶͬ������ʾ


	//---------------------------------MiniWin����-------------------------------------------
	int ML_GVID_OpenMiniWin(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
	int ML_GVID_CloseMiniWin(char VideoChannel);
	int ML_GVID_SetMiniWinXY(char VideoChannel ,short theX ,short theY);
	int ML_GVID_SetMiniWinWH(char VideoChannel ,short theWidth, short theHeight);

	// ��ɫ����
	int ML_GVID_SetMiniWinColor(char VideoChannel,unsigned int theColor);                          // ����С������PPI�״����ݵ���ɫ���û�ֻ������һ����ɫֵ���ڲ����������������ɫ��
	int ML_GVID_SetMiniWinColorTable(char VideoChannel,unsigned int* colorTable);                  // ����С������PPI�״����ݵ���ɫ���û�������һ�ű�
	int ML_GVID_SetMiniWinBackgroundColor(char VideoChannel,unsigned int theColor);                // ���ñ�����ɫ������ʱ�� 

	// �����Ŵ�
	int ML_GVID_SetMiniWinDistDir(char VideoChannel ,short theDistance ,float theDirection);       // ���ÿ���������ģ� ͨ����λ�;��룬�Լ������̣����Եõ�
	int ML_GVID_SetMiniWinScale(char VideoChannel,int theScale);                                   // ���÷Ŵ���

	int ML_GVID_SetMiniWinMode(char VideoChannel, short theMode);



	//------------------------------------AR----------------------------------------------
	int ML_GVID_OpenAr(char VideoChannel,short theX, short theY , short theWidth, short theHeight);  
	int ML_GVID_CloseAr(char VideoChannel);
	int ML_GVID_SetArXY(char VideoChannel ,short theX ,short theY);
	int ML_GVID_SetArWH(char VideoChannel ,short theWidth, short theHeight);

	// ��ɫ����
	int ML_GVID_SetArColor(char VideoChannel,unsigned int theColor);
	int ML_GVID_SetArBackgroundColor(char VideoChannel,unsigned int theColor);

	int ML_GVID_SetArGate(char VideoChannel ,unsigned char theGate);                    // AR���ޣ�С�ڴ������״����ݲ���AR���ڽ�����ʾ
	int ML_GVID_SetArDist(char VideoChannel ,short theDist);                            // �������ұߵ�Ĵ�С
	int ML_GVID_SetARTail(char VideoChannel,int theTail);                               // AR����˥���̶ȣ�theTailֵԽ��˥��Խǿ


	int ML_GVID_SetLayerSwitch(char VideoChannel,int Layer,short theSwitch);            // ֻ���Ƶ��ӣ��ر�ʱ���������������ڹر�״̬ʱ��PPI��ҳ��������ƣ������в����ӡ�
	int ML_GVID_SendRadarPkg(char VideoChannel, int pkgNum, ML_GVIDMW_RadarPkg_t *pkgBuf);
	int ML_GVID_SetScanMode(char VideoChannel, short theScanMode);


	//------------------------------------TV----------------------------------------------
	int ML_GVID_OpenTV (char VideoChannel ,short theX, short theY,short theWidth,short theHeight);
	int ML_GVID_CloseTV (char VideoChannel );
	int ML_GVID_SetTVXY(char VideoChannel ,short theX, short theY);
	int ML_GVID_SetTVWH(char VideoChannel ,short theWidth, short theHeight);
	int ML_GVID_FrostTV(char VideoChannel,char theFrost);
	int ML_GVID_SetTVBright(char VideoChannel,unsigned char theBright);
	int ML_GVID_SetTVContrast(char VideoChannel,unsigned char theContrast);
	int ML_GVID_SetTVColor(char VideoChannel,unsigned char theColor);

	int ML_GVID_SetPMLayerColors(char VideoChannel, char theMode, int theNum, unsigned int *colors);
	int ML_GVID_SetATLayerColorKey(char VideoChannel, unsigned int colorKey);

	int ML_GVID_GetBmp(char winth, BMPImageRec *bmp, int x, int y);
	int ML_GVID_EraseBmp(char winth);
	int ML_GVID_GetBackMap(BMPImageRec *bmp, char VideoChannel);
	int ML_GVID_EraseBackMap(char VideoChannel); 


private:
	int convertRadarPkg(char VideoChannel, ML_GVIDMW_RadarPkg_t* dat, s_ppi_video_for_disp* dispbuf);                   // �������ϴ��������ݰ����в���
	int ML_GVID_Init();
};


#endif
