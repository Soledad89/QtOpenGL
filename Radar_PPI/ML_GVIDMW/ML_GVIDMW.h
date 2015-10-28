#include <windows.h>
#include "ppidef.h"
#include "bmp.h"

/****************************************************
�м�����ƣ��ۺ�ͼ��ͼ����ʾ�м��������棩
�汾��1.0
��Ʒ��λ���廪��ѧ���ӹ���ϵ���źŲɼ����������Կ�ʵ����
�ļ��������м������ӿڶ���
****************************************************/

#ifndef __THU_6BE7D83F_50C1_4007_804D_525118CDCC34__
#define __THU_6BE7D83F_50C1_4007_804D_525118CDCC34__


#ifdef _WINDLL
#define GVIDMW_API  extern "C" __declspec(dllexport) 
#else 
#define GVIDMW_API  extern "C" __declspec(dllimport) 
#endif


//======��׼�й涨�Ķ���ӿ�======
// VideoChannel ȡֵ1��2
// theColor�ĸ�ʽΪ���ɸߵ��ͣ�R��G��B������  ( 0xRRGGBB00 )

//------init------
GVIDMW_API int ML_GVID_Init();
//��ʼ����������ֵ������Ϣ��
//  -1  --  �ظ����ó�ʼ������
//  -2  --  ��־�ļ�������������û��save�ļ���
//  -3  --  �����ļ�����
//  -4  --  ͼ��ϵͳ������������û����DirectDraw����
//  -5  --  ϵͳ���ݳ������dat�ļ������ļ�
//  -6  --  �״���ϢԴ��Ϊlocal��ʽ��save�ļ�����ȱ����Ϊradar1��radar2����Ƶ�ļ�

//------PPI------
GVIDMW_API int ML_GVID_OpenPPI(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
GVIDMW_API int ML_GVID_ClosePPI(char VideoChannel);
GVIDMW_API int ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY);
GVIDMW_API int ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight);
GVIDMW_API int ML_GVID_SetPPIMode(char VideoChannel, short theMode);
GVIDMW_API int ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY);
GVIDMW_API int ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir);
GVIDMW_API int ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor);
GVIDMW_API int ML_GVID_SetPPIScan(char VideoChannel,int theScan);
GVIDMW_API int ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor);
GVIDMW_API int ML_GVID_FrostPPI(char VideoChannel,char theFrost);
GVIDMW_API int ML_GVID_SetPPIDist(char VideoChannel,short theDist);
GVIDMW_API int ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch);
GVIDMW_API int ML_GVID_SetPPIGate(char VideoChannel,char theGate);
GVIDMW_API int ML_GVID_SetPPITail(char VideoChannel,int theTail);

GVIDMW_API int ML_GVID_SetTailSwitch(char VideoChannel,short theSwitch);
GVIDMW_API int ML_GVID_SetTailColor(char VideoChannel,unsigned int theColor);
GVIDMW_API int ML_GVID_SetTailGate(char VideoChannel,unsigned char theGate); 
GVIDMW_API int ML_GVID_SetTailTimer(char VideoChannel,int theTime); 
GVIDMW_API int ML_GVID_SetPPITogether(short theSwitch);
GVIDMW_API int ML_GVID_SetPPIColorTable(char VideoChannel,unsigned int* colorTable);
GVIDMW_API int ML_GVID_ClearPPI(char VideoChannel); 
GVIDMW_API int ML_GVID_SetPPIMoveMode(char VideoChannel, short theMode); 




//------AR------
GVIDMW_API int ML_GVID_OpenAr(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
GVIDMW_API int ML_GVID_CloseAr(char VideoChannel);
GVIDMW_API int ML_GVID_SetArXY(char VideoChannel ,short theX ,short theY);
GVIDMW_API int ML_GVID_SetArWH(char VideoChannel ,short theWidth, short theHeight);
GVIDMW_API int ML_GVID_SetArGate(char VideoChannel ,char theGate);
GVIDMW_API int ML_GVID_SetArDist(char VideoChannel ,short theDist);

GVIDMW_API int ML_GVID_SetArColor(char VideoChannel,unsigned int theColor);
GVIDMW_API int ML_GVID_SetArTail(char VideoChannel,int theTail);
GVIDMW_API int ML_GVID_SetArBackgroundColor(char VideoChannel,unsigned int theColor);




//------MiniWin------
GVIDMW_API int ML_GVID_OpenMiniWin(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
GVIDMW_API int ML_GVID_CloseMiniWin(char VideoChannel);
GVIDMW_API int ML_GVID_SetMiniWinXY(char VideoChannel ,short theX ,short theY);
GVIDMW_API int ML_GVID_SetMiniWinWH(char VideoChannel ,short theWidth, short theHeight);
GVIDMW_API int ML_GVID_SetMiniWinMode(char VideoChannel, short theMode);
GVIDMW_API int ML_GVID_SetMiniWinColor(char VideoChannel,unsigned int theColor);
GVIDMW_API int ML_GVID_SetMiniWinDistDir(char VideoChannel ,short theDistance ,float theDirection);
GVIDMW_API int ML_GVID_SetMiniWinScale(char VideoChannel,int theScale);

GVIDMW_API int ML_GVID_SetMiniWinColorTable(char VideoChannel,unsigned int* colorTable);
GVIDMW_API int ML_GVID_SetScanMode(char VideoChannel, short theScanMode);
GVIDMW_API int ML_GVID_SendRadarPkg(char VideoChannel, int pkgNum, ML_GVIDMW_RadarPkg_t *pkgBuf);




//------TV------
/*
GVIDMW_API int ML_GVID_OpenTV (char VideoChannel ,short theX, short theY,short theWidth,short theHeight);
GVIDMW_API int ML_GVID_CloseTV (char VideoChannel );
GVIDMW_API int ML_GVID_SetTVXY(char VideoChannel ,short theX, short theY);
GVIDMW_API int ML_GVID_SetTVWH(char VideoChannel ,short theWidth, short theHeight);
GVIDMW_API int ML_GVID_FrostTV(char VideoChannel,char theFrost);
GVIDMW_API int ML_GVID_SetTVBright(char VideoChannel,unsigned char theBright);
GVIDMW_API int ML_GVID_SetTVContrast(char VideoChannel,unsigned char theContrast);
GVIDMW_API int ML_GVID_SetTVColor(char VideoChannel,unsigned char theColor);


GVIDMW_API int ML_GVID_SetPMLayerColors(char VideoChannel, char theMode, int theNum, unsigned int *colors);
GVIDMW_API int ML_GVID_SetATLayerColorKey(char VideoChannel, unsigned int colorKey);

*/


//========== ��׼�����չ�ӿ�==========
// VideoChannel ȡֵ1��2

// ����PPI��AR�ı����Ƿ���ʾ��disp��true��ʾ��false����ʾ
GVIDMW_API int tshSetArMarkDisplay(char VideoChannel,bool disp);
GVIDMW_API int tshSetRadarMarkDisplay(char VideoChannel,bool disp);

// ========== ��ͼ��չ�ӿ� ==========

 // ��ͼ�����Ͷ���(surfType)
#define SURF_PPI_TOP 0             // �û���
#define SURF_PPI_MIDDLE 1          // �״����ݲ�
#define SURF_PPI_BOTTOM 2          // ������ͼ��

#define SURF_MINI_TOP 3            // �û���
#define SURF_MINI_BOTTOM 4         // �״����ݲ�

#define SURF_AR_TOP 5
#define SURF_AR_BOTTOM 6

#define SURF_TV_TOP 7
#define SURF_TV_BOTTOM 8

GVIDMW_API int tshGetBackMap(BMPImageRec *bmp, char VideoChannel);
GVIDMW_API int tshEraseBackMap(char VideoChannel);
GVIDMW_API int tshGetBmp(BMPImageRec *bmp, char winth, int x, int y);                                   //����ӿ�
GVIDMW_API int tshEraseBmp(char winth);                                      //��������


GVIDMW_API int tshGetSurfaceDC(HDC* dc, char VideoChannel,int surfType);
GVIDMW_API int tshReleaseSurfaceDC(HDC dc, char VideoChannel,int surfType);

// !! ע�⣬colorKey��������COLORREF����ͬ�ڱ�׼�ӿ��е���ɫ��ʽ��
// ʹ�ú�RGB(r,g,b)���ɣ���ʽΪ���ɸߵ��ͣ�������B��G��R   ( 0x00BBGGRR )
GVIDMW_API int tshSetSurfaceColorKey(char VideoChannel,int surfType,COLORREF colorkey);
GVIDMW_API int tshSetSurfaceVisible(char VideoChannel,int surfType,BOOL visible);
GVIDMW_API int tshEraseSurface(char VideoChannel,int surfType);

// ========== ���������չ�ӿ� ==========
/*
// ����OSD����ʾ�ַ���ʱ��
// XY����Ϊ�ַ��������Ͻǡ������Ƶ�����Ͻǡ�������,�ַ���󳤶�32
GVIDMW_API int tshSetOSDStr(char VideoChannel,char* pstr,short x,short y);
GVIDMW_API int tshSetOSDTime(char VideoChannel,short x,short y);

// ����͸���������豸�˴��ڣ�pData ����; nLen ���ݳ���; nChnNum ---- 0Ϊ485�ӿ�,1Ϊ232�ӿ�
GVIDMW_API int tshSendTransparentData(char VideoChannel,char* pData,int nLen,int nChnNum);
// Уʱ,��ϵͳʱ�䷢�͵��豸��
GVIDMW_API int tshVerifyTime(char VideoChannel);
*/

#endif