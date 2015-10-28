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
	s_ppi_video_for_disp* dispbuf;          // 指向要送显的内部雷达数据

	CGraphicsManager* pGraphicsManager;
	CWindowsManager* pWindowsManager;
	//CDataPool* pDataPool;


public:
	CRadarDisplay();
	~CRadarDisplay();


	//--------------------------PPI控制-----------------------------------------
	
	// 窗口控制
	int ML_GVID_OpenPPI(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
	int ML_GVID_ClosePPI(char VideoChannel);
	int ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY);
	int ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight);

	// 开关控制
	int ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch);                   // 只控制后页绘制：关闭时，清屏。当处于关闭状态时，PPI后页层不绘制，叠加。
	int ML_GVID_SetPPIFrost(char VideoChannel,char theFrost);                      // 只控制后页绘制：冻结时，不清屏。当处于冻结状态时，PPI后页层不绘制，叠加。解冻时，先清屏。
	int ML_GVID_SetPPIScan(char VideoChannel,int theScan);                         // 控制是否显示扫描线
	int ML_GVID_SetPPITailSwitch(char VideoChannel,short theSwitch);               // 控制是否打开尾迹开关

	// 颜色设置
	int ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor);               
	int ML_GVID_SetPPIColorTable(char VideoChannel,unsigned int* colorTable);
	int ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor);
	int ML_GVID_SetPPIBackgroundColor(char VideoChannel,unsigned int theColor);
	int ML_GVID_SetPPITailColor(char VideoChannel,unsigned int theColor);

	int ML_GVID_SetPPIMode(char VideoChannel, short theMode);                       // PPI窗口显示模式    0：P显   1：B显   2：E显
	int ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY);            // P显中心位置
	int ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir);                  // 舰艏方位
	int ML_GVID_SetPPIDist(char VideoChannel,short theDist);                        // 量程
	int ML_GVID_SetMoveMode(char VideoChannel, short theMode);

	int ML_GVID_SetPPIGate(char VideoChannel,unsigned char theGate);                // 门限，只有超过此门限的雷达数据才会显示，太小的雷达数据不进行显示
	int ML_GVID_SetPPITailGate(char VideoChannel,unsigned char theGate);            // 尾迹门限：尽量保留大目标，当余晖衰减到此门限值时，对其做尾迹标志，以后的衰减按尾迹衰减规律来进行，直到下一个较大的目标将其覆盖

	int ML_GVID_SetPPITail(char VideoChannel,int theTail);                          // 余晖衰减程度： theTail值越大，衰减越强
	int ML_GVID_SetPPITailTimer(char VideoChannel,int theTime);                     // 尾迹衰减程度： theTime值越大，衰减越强

	int ML_GVID_ClearPPI(char VideoChannel);                                        // 对PPI雷达数据层

	int ML_GVID_SetPPITogether(short theSwitch);                                    // 两路雷达视频同窗口显示


	//---------------------------------MiniWin控制-------------------------------------------
	int ML_GVID_OpenMiniWin(char VideoChannel,short theX, short theY , short theWidth, short theHeight);
	int ML_GVID_CloseMiniWin(char VideoChannel);
	int ML_GVID_SetMiniWinXY(char VideoChannel ,short theX ,short theY);
	int ML_GVID_SetMiniWinWH(char VideoChannel ,short theWidth, short theHeight);

	// 颜色设置
	int ML_GVID_SetMiniWinColor(char VideoChannel,unsigned int theColor);                          // 设置小窗口中PPI雷达数据的颜色，用户只需输入一个颜色值，内部函数会算出整张颜色表
	int ML_GVID_SetMiniWinColorTable(char VideoChannel,unsigned int* colorTable);                  // 设置小窗口中PPI雷达数据的颜色，用户可上载一张表
	int ML_GVID_SetMiniWinBackgroundColor(char VideoChannel,unsigned int theColor);                // 设置背景颜色，清屏时用 

	// 开窗放大
	int ML_GVID_SetMiniWinDistDir(char VideoChannel ,short theDistance ,float theDirection);       // 设置开窗点的中心： 通过方位和距离，以及总量程，可以得到
	int ML_GVID_SetMiniWinScale(char VideoChannel,int theScale);                                   // 设置放大倍数

	int ML_GVID_SetMiniWinMode(char VideoChannel, short theMode);



	//------------------------------------AR----------------------------------------------
	int ML_GVID_OpenAr(char VideoChannel,short theX, short theY , short theWidth, short theHeight);  
	int ML_GVID_CloseAr(char VideoChannel);
	int ML_GVID_SetArXY(char VideoChannel ,short theX ,short theY);
	int ML_GVID_SetArWH(char VideoChannel ,short theWidth, short theHeight);

	// 颜色设置
	int ML_GVID_SetArColor(char VideoChannel,unsigned int theColor);
	int ML_GVID_SetArBackgroundColor(char VideoChannel,unsigned int theColor);

	int ML_GVID_SetArGate(char VideoChannel ,unsigned char theGate);                    // AR门限：小于此门限雷达数据不在AR窗口进行显示
	int ML_GVID_SetArDist(char VideoChannel ,short theDist);                            // 坐标最右边点的大小
	int ML_GVID_SetARTail(char VideoChannel,int theTail);                               // AR余晖衰减程度：theTail值越大，衰减越强


	int ML_GVID_SetLayerSwitch(char VideoChannel,int Layer,short theSwitch);            // 只控制叠加：关闭时，不清屏。当处于关闭状态时，PPI后页层继续绘制，窗口中不叠加。
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
	int convertRadarPkg(char VideoChannel, ML_GVIDMW_RadarPkg_t* dat, s_ppi_video_for_disp* dispbuf);                   // 将网络上传来的数据包进行补线
	int ML_GVID_Init();
};


#endif
