// Name:   CPPI.h
// Author: Yan Guo
// Date:   Feb 27, 2013 in the canteen of Rohm Building

//文件功能：

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

	VIDEODATATYPE *video_data;          //所需显示的雷达回波数据
	int dispRadius;      // 显示半径
	int dispMode;        // 显示模式    0：B显   1：P显   2：E显
    int prowIndex;       // 舰艏方位
	int hollowNum;       // 空心的像素值
	short moveMode;      // 真运动模式

//	int WinMode;

	short xorg;
	short yorg;

	s_layer_unit ** layerPPI;
	s_layer_unit ** layerWin;

	DISPMEMTYPE *colortablePPI;    // PPI显示颜色表
	DISPMEMTYPE *colortableWin;    // 小窗口显示颜色表

	int scanLine;
	unsigned int *scanLineColor;
	unsigned int backColor;
	unsigned int backColorWin;

	unsigned char base;  // 门限，只有超过此门限的雷达数据才会显示


	// 根据量程range和距离distance，可以计算出开窗位置（direction方向上第几个点）
	short range;        // PPI量程
	short distanceWin;     // 开窗距离
	float directionWin;    // 开窗方位
	int scaleWin;          // 开窗的放大系数，根据这个参数和窗口的大小尺寸，可以确定将后页FBO中多大的区域贴到小窗口中


    int ** addr_near_x;
    int ** addr_near_y;
    int ** addr_far_x;
    int ** addr_far_y;

	CGraphicsManager* pGraphicsManager;
	int* vertices;
	int* scanLineVertices;
	unsigned int* colorsPPI;
	unsigned int* colorsWin;

	int numPack;   // 每隔多少个包渲染一次
	int np;  // 待更新的点数
	int line;

	short isOpen;            // 控制后页绘制：关闭时，清屏。当处于关闭状态时，PPI后页层不绘制，叠加。
	char frost;              // 控制后页绘制：冻结时，不清屏。当处于冻结状态时，PPI后页层不绘制，叠加。解冻时，先清屏。
	short visiblePPI[3];     // 控制窗口叠加：关闭时，不清屏。当处于关闭状态时，PPI后页层继续绘制，不叠加。
	short visibleWin[2];

	short comdisp;


public:
	CPPI(CDataPool *dp, CGraphicsManager* pGM, char videoChannel);
	~CPPI();

	//-----------------------------PPI---------------------------
	// 状态控制
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

	// 图形绘制
	int PPIDraw(s_ppi_video_for_disp* dispdat);
	int PPIClear(int surfth);
	int PPIUpdate();
	int PPIDrawBackMap(BMPImageRec* pBackMap);

	int PPIClearLayer();
	int PPISetMoveMode(short theMode);

	//--------------------------MiniWin---------------------------
	// 状态控制
	int WinSetDistDir(short theDistance ,float theDirection);
	int WinSetScale(int theScale);
	int WinSetBackgroundColor(unsigned int theColor);
	
	// 图形绘制
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