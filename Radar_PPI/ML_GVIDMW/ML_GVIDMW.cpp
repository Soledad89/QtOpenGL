#include "ML_GVIDMW.h"
#include "CRadarDisplay.h"

CRadarDisplay* pRadarDisplay = NULL;

bool bInit = false;
bool bInitPPI[2] = {false,false};
bool bInitAR[2] = {false,false};
bool bInitMiniWin[2] = {false,false};
bool bInitTV[2] = {false,false};

#define INIT_CHECK if(!bInit)return -5;//-3
#define INIT_PPI_CHECK if(!bInitPPI[VideoChannel-1])return -5;
#define INIT_AR_CHECK if(!bInitAR[VideoChannel-1])return -5;
#define INIT_MiniWin_CHECK if(!bInitMiniWin[VideoChannel-1])return -5;
#define INIT_TV_CHECK if(!bInitTV[VideoChannel-1])return -5;

#define VC_CHECK   if(VideoChannel!=1 && VideoChannel!=2)return -4;
#define XY_CHECK   if(!checkXY(theX,theY))return -3;
#define VR_CHECK(n,l,u) if(n<l || n>u)return -6;

int screenMode = 0;// 0--双屏，横排； 1--双屏，竖排； 2--单屏

bool checkXY(int x,int y)
{
	switch(screenMode)
	{
	case 0:
		if(x<0 || x>=3200 || y<0 || y>=1200)
			return false;
		break;
	case 1:
		if(x<0 || x>=1600 || y<0 || y>=2400)
			return false;
		break;
	case 2:
		if(x<0 || x>=1600 || y<0 || y>=1200)
			return false;
		break;
	}

	return true;
}



GVIDMW_API int ML_GVID_Init()
{
	if(bInit == true) return -1;

	pRadarDisplay = new CRadarDisplay();
	bInit=true;

	return 1;
}


//------PPI------
GVIDMW_API int ML_GVID_OpenPPI(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	INIT_CHECK
	VC_CHECK
	XY_CHECK
	VR_CHECK(theWidth,1,1200)
	VR_CHECK(theHeight,1,1200)

	pRadarDisplay -> ML_GVID_OpenPPI(VideoChannel, theX, theY, theWidth, theHeight);
	bInitPPI[VideoChannel - 1]= true;

	return 1;
}


GVIDMW_API int ML_GVID_ClosePPI(char VideoChannel)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_ClosePPI(VideoChannel);
	bInitPPI[VideoChannel - 1] = false;

	return 1;
}


GVIDMW_API int ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	XY_CHECK

	pRadarDisplay -> ML_GVID_SetPPIXY(VideoChannel, theX, theY);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theWidth,1,1200)
	VR_CHECK(theHeight,1,1200)

	pRadarDisplay -> ML_GVID_SetPPIWH(VideoChannel, theWidth, theHeight);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIMode(char VideoChannel, short theMode)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theMode,0,3)

	pRadarDisplay -> ML_GVID_SetPPIMode(VideoChannel, theMode);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theX,0,1199)
	VR_CHECK(theY,0,1199)

	pRadarDisplay -> ML_GVID_SetPPICenter(VideoChannel, theX, 1200 - theY);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK

	while(theShipDir<0)
			theShipDir+=360.0;
	while(theShipDir>=360)
		theShipDir -= 360.0;

	pRadarDisplay -> ML_GVID_SetPPIShipDir(VideoChannel, theShipDir);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetPPIColor(VideoChannel, theColor);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIScan(char VideoChannel,int theScan)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theScan,0,1)

	pRadarDisplay -> ML_GVID_SetPPIScan(VideoChannel, theScan);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetPPILineColor(VideoChannel, theColor);
	return 1;
}


GVIDMW_API int ML_GVID_FrostPPI(char VideoChannel,char theFrost)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theFrost,0,1)

	pRadarDisplay -> ML_GVID_SetPPIFrost(VideoChannel, theFrost);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIDist(char VideoChannel,short theDist)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theDist,1,32767)

	pRadarDisplay -> ML_GVID_SetPPIDist(VideoChannel, theDist);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theSwitch,0,1)

	pRadarDisplay -> ML_GVID_SetPPISwitch(VideoChannel, theSwitch);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIGate(char VideoChannel,char theGate)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theGate,0,7)

	pRadarDisplay -> ML_GVID_SetPPIGate(VideoChannel, theGate*36);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPITail(char VideoChannel,int theTail)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theTail,0,65535)

	pRadarDisplay -> ML_GVID_SetPPITail(VideoChannel, theTail);
	return 1;
}


// 新增
GVIDMW_API int ML_GVID_SetTailSwitch(char VideoChannel,short theSwitch)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theSwitch,0,1)

	pRadarDisplay -> ML_GVID_SetPPITailSwitch(VideoChannel, theSwitch);
	return 1;
}


GVIDMW_API int ML_GVID_SetTailColor(char VideoChannel,unsigned int theColor)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetPPITailColor(VideoChannel, theColor);
	return 1;
}


GVIDMW_API int ML_GVID_SetTailGate(char VideoChannel,unsigned char theGate)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theGate,0,255)

	pRadarDisplay -> ML_GVID_SetPPITailGate(VideoChannel, theGate);
	return 1;
}


GVIDMW_API int ML_GVID_SetTailTimer(char VideoChannel,int theTime)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theTime,0,65535)

	pRadarDisplay -> ML_GVID_SetPPITailTimer(VideoChannel, theTime);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPITogether(short theSwitch)
{
	INIT_CHECK
	VR_CHECK(theSwitch,0,1)

	pRadarDisplay -> ML_GVID_SetPPITogether(theSwitch);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIColorTable(char VideoChannel,unsigned int* colorTable)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetPPIColorTable(VideoChannel, colorTable);
	return 1;
}


GVIDMW_API int ML_GVID_ClearPPI(char VideoChannel)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_ClearPPI(VideoChannel);
	return 1;
}


GVIDMW_API int ML_GVID_SetPPIMoveMode(char VideoChannel, short theMode)
{
	INIT_CHECK
	INIT_PPI_CHECK
	VC_CHECK
	VR_CHECK(theMode,0,1)

	pRadarDisplay -> ML_GVID_SetMoveMode(VideoChannel, theMode);
	return 1;
}



//------AR------
GVIDMW_API int ML_GVID_OpenAr(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	INIT_CHECK
	VC_CHECK
	XY_CHECK
	VR_CHECK(theWidth,1,512)
	VR_CHECK(theHeight,1,512)

	pRadarDisplay -> ML_GVID_OpenAr(VideoChannel, theX, theY, theWidth, theHeight);
	bInitAR[VideoChannel - 1]  = true;

	return 1;
}


GVIDMW_API int ML_GVID_CloseAr(char VideoChannel)
{
	INIT_CHECK
	INIT_AR_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_CloseAr(VideoChannel);
	bInitAR[VideoChannel - 1]  = false;

	return 1;
}


GVIDMW_API int ML_GVID_SetArXY(char VideoChannel ,short theX ,short theY)
{
	INIT_CHECK
	INIT_AR_CHECK
	VC_CHECK
	XY_CHECK

	pRadarDisplay -> ML_GVID_SetArXY(VideoChannel, theX, theY);
	return 1;
}


GVIDMW_API int ML_GVID_SetArWH(char VideoChannel ,short theWidth, short theHeight)
{
	INIT_CHECK
	INIT_AR_CHECK
	VC_CHECK
	VR_CHECK(theWidth,1,512)
	VR_CHECK(theHeight,1,512)

	pRadarDisplay -> ML_GVID_SetArWH(VideoChannel, theWidth, theHeight);
	return 1;
}


GVIDMW_API int ML_GVID_SetArGate (char VideoChannel ,char theGate)
{
	INIT_CHECK
	INIT_AR_CHECK
	VC_CHECK
	VR_CHECK(theGate,0,7)

	pRadarDisplay -> ML_GVID_SetArGate(VideoChannel, theGate*36);
	return 1;
}


GVIDMW_API int ML_GVID_SetArDist (char VideoChannel ,short theDist)
{
	INIT_CHECK
	INIT_AR_CHECK
	VC_CHECK
	VR_CHECK(theDist,1,32767)

	pRadarDisplay -> ML_GVID_SetArDist(VideoChannel, theDist);
	return 1;
}


// 新增
GVIDMW_API int ML_GVID_SetArColor(char VideoChannel,unsigned int theColor)
{
	INIT_CHECK
	INIT_AR_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetArColor(VideoChannel, theColor);
	return 1;
}


GVIDMW_API int ML_GVID_SetArTail(char VideoChannel,int theTail)
{
	//INIT_CHECK
	//INIT_AR_CHECK
	//VC_CHECK
	//VR_CHECK(theTail,1,65535)

	pRadarDisplay -> ML_GVID_SetARTail(VideoChannel, theTail);
	return 1;
}


GVIDMW_API int ML_GVID_SetArBackgroundColor(char VideoChannel,unsigned int theColor)
{
	INIT_CHECK
	INIT_AR_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetArBackgroundColor(VideoChannel, theColor);
	return 1;
}



//------MiniWin------
GVIDMW_API int ML_GVID_OpenMiniWin(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	INIT_CHECK
	VC_CHECK 
	XY_CHECK
	VR_CHECK(theWidth,1,1200)
	VR_CHECK(theHeight,1,1200)

	pRadarDisplay -> ML_GVID_OpenMiniWin(VideoChannel, theX, theY, theWidth, theHeight);
	bInitMiniWin[VideoChannel - 1]  = true;

	return 1;
}


GVIDMW_API int ML_GVID_CloseMiniWin(char VideoChannel)
{
	INIT_CHECK
	INIT_MiniWin_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_CloseMiniWin(VideoChannel);
	bInitMiniWin[VideoChannel - 1]  = false;
	return 1;
}


GVIDMW_API int ML_GVID_SetMiniWinXY(char VideoChannel ,short theX ,short theY)
{
	INIT_CHECK
	INIT_MiniWin_CHECK
	VC_CHECK
	XY_CHECK

	pRadarDisplay -> ML_GVID_SetMiniWinXY(VideoChannel, theX, theY);
	return 1;
}


GVIDMW_API int ML_GVID_SetMiniWinWH(char VideoChannel ,short theWidth, short theHeight)
{
	INIT_CHECK
	INIT_MiniWin_CHECK
	VC_CHECK
	VR_CHECK(theWidth,1,1200)
	VR_CHECK(theHeight,1,1200)

	pRadarDisplay -> ML_GVID_SetMiniWinWH(VideoChannel, theWidth, theHeight);
	return 1;
}


GVIDMW_API int ML_GVID_SetMiniWinMode(char VideoChannel, short theMode)
{
	INIT_CHECK
	INIT_PPI_CHECK
	INIT_MiniWin_CHECK
	VC_CHECK
	VR_CHECK(theMode,0,1)

	pRadarDisplay -> ML_GVID_SetPPIMode(VideoChannel, theMode);
	return 1;
}


GVIDMW_API int ML_GVID_SetMiniWinColor(char VideoChannel,unsigned int theColor)
{
	INIT_CHECK
	INIT_PPI_CHECK
	INIT_MiniWin_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetMiniWinColor(VideoChannel, theColor);
	return 1;
}


GVIDMW_API int ML_GVID_SetMiniWinDistDir(char VideoChannel ,short theDistance ,float theDirection)
{
	INIT_CHECK
	INIT_PPI_CHECK
	INIT_MiniWin_CHECK
	VC_CHECK
	VR_CHECK(theDistance,0,32767)
	while(theDirection<0)
	theDirection += 360.0;
	while(theDirection>=360)
	theDirection -= 360.0;

	pRadarDisplay -> ML_GVID_SetMiniWinDistDir(VideoChannel, theDistance, theDirection);
	return 1;
}


GVIDMW_API int ML_GVID_SetMiniWinScale(char VideoChannel,int theScale)
{
	INIT_CHECK
	INIT_PPI_CHECK
	INIT_MiniWin_CHECK
	VR_CHECK(theScale,1,8)
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetMiniWinScale(VideoChannel, theScale);
	return 1;
}


// 新增
GVIDMW_API int ML_GVID_SetMiniWinColorTable(char VideoChannel,unsigned int* colorTable)
{
	INIT_CHECK
	INIT_PPI_CHECK
	INIT_MiniWin_CHECK

	pRadarDisplay -> ML_GVID_SetMiniWinColorTable(VideoChannel, colorTable);
	return 1;
}


GVIDMW_API int ML_GVID_SetScanMode(char VideoChannel, short theScanMode)
{
	INIT_CHECK
	INIT_PPI_CHECK
	INIT_MiniWin_CHECK

	VR_CHECK(theScanMode,0,4)

	pRadarDisplay -> ML_GVID_SetScanMode(VideoChannel, theScanMode);
	return 1;
}


GVIDMW_API int ML_GVID_SendRadarPkg(char VideoChannel, int pkgNum, ML_GVIDMW_RadarPkg_t *pkgBuf)
{
	INIT_CHECK
	INIT_PPI_CHECK
	INIT_MiniWin_CHECK

	pRadarDisplay -> ML_GVID_SendRadarPkg(VideoChannel, pkgNum, pkgBuf);
	return 1;
}



//------TV------
GVIDMW_API int ML_GVID_OpenTV (char VideoChannel ,short theX, short theY,short theWidth,short theHeight)
{
	INIT_CHECK
	VC_CHECK
	XY_CHECK
	VR_CHECK(theWidth,1,720);
	VR_CHECK(theHeight,1,576);

	pRadarDisplay -> ML_GVID_OpenTV(VideoChannel, theX, theY, theWidth, theHeight);
	bInitTV[VideoChannel-1]  = true;

	return 1;
}


GVIDMW_API int ML_GVID_CloseTV (char VideoChannel )
{
	INIT_CHECK
	INIT_TV_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_CloseTV (VideoChannel);
	bInitTV[VideoChannel-1]  = false;

	return 1;
}


GVIDMW_API int ML_GVID_SetTVXY(char VideoChannel ,short theX, short theY)
{
	INIT_CHECK
	INIT_TV_CHECK
	VC_CHECK
	XY_CHECK

	pRadarDisplay -> ML_GVID_SetTVXY(VideoChannel, theX, theY);
	return 1;
}


GVIDMW_API int ML_GVID_SetTVWH(char VideoChannel ,short theWidth, short theHeight)
{
	INIT_CHECK
	INIT_TV_CHECK
	VC_CHECK
	VR_CHECK(theWidth,1,720)
	VR_CHECK(theHeight,1,576)

	pRadarDisplay -> ML_GVID_SetTVWH(VideoChannel, theWidth, theHeight);
	return 1;
}


GVIDMW_API int ML_GVID_FrostTV(char VideoChannel,char theFrost)
{
	INIT_CHECK
	INIT_TV_CHECK
	VC_CHECK
	VR_CHECK(theFrost,0,1)

	pRadarDisplay -> ML_GVID_FrostTV(VideoChannel, theFrost);
	return 1;
}


GVIDMW_API int ML_GVID_SetTVBright(char VideoChannel,unsigned char theBright)
{
	INIT_CHECK
	INIT_TV_CHECK
	VC_CHECK
	VR_CHECK(theBright,0,255)

	pRadarDisplay -> ML_GVID_SetTVBright(VideoChannel, theBright);
	return 1;
}


GVIDMW_API int ML_GVID_SetTVContrast(char VideoChannel,unsigned char theContrast)
{
	INIT_CHECK
	INIT_TV_CHECK
	VC_CHECK
	VR_CHECK(theContrast,0,255)

	pRadarDisplay -> ML_GVID_SetTVContrast(VideoChannel, theContrast);
	return 1;
}

GVIDMW_API int ML_GVID_SetTVColor(char VideoChannel,unsigned char theColor)
{
	INIT_CHECK
	INIT_TV_CHECK
	VC_CHECK
	VR_CHECK(theColor,0,255)

	pRadarDisplay -> ML_GVID_SetTVColor(VideoChannel, theColor);
	return 1;
}


// 新增
GVIDMW_API int ML_GVID_SetPMLayerColors(char VideoChannel, char theMode, int theNum, unsigned int *colors)
{
	INIT_CHECK
	VC_CHECK
	VR_CHECK(theMode,0,1)
	VR_CHECK(theNum,0,256)

	pRadarDisplay -> ML_GVID_SetPMLayerColors(VideoChannel, theMode, theNum, colors);
	return 1;
}


GVIDMW_API int ML_GVID_SetATLayerColorKey(char VideoChannel, unsigned int colorKey)
{
	INIT_CHECK
	INIT_TV_CHECK
	VC_CHECK

	pRadarDisplay -> ML_GVID_SetATLayerColorKey(VideoChannel, colorKey);
	return 1;
}



//========== 标准外的扩展接口==========
// VideoChannel 取值1、2

// 设置PPI和AR的标绘层是否显示，disp：true显示；false不显示
GVIDMW_API int tshSetArMarkDisplay(char VideoChannel,bool disp)
{
	return 0;
}
GVIDMW_API int tshSetRadarMarkDisplay(char VideoChannel,bool disp)
{
	return 0;
}

// ========== 绘图扩展接口 ==========

 // 绘图层类型定义(surfType)
#define SURF_PPI_BOTTOM 0
#define SURF_PPI_TOP 1
#define SURF_AR_TOP 2
#define SURF_MINI_TOP 3

GVIDMW_API int tshGetBackMap(BMPImageRec *bmp, char VideoChannel)
{
	pRadarDisplay -> ML_GVID_GetBackMap(bmp, VideoChannel);
	return 1;
}

GVIDMW_API int tshEraseBackMap(char VideoChannel)
{
	pRadarDisplay -> ML_GVID_EraseBackMap(VideoChannel);
	return 1;
}

GVIDMW_API int tshGetBmp(BMPImageRec *bmp, char winth, int x, int y)                                   //标绘层接口
{
	pRadarDisplay -> ML_GVID_GetBmp(winth, bmp, x, y);
	return 1;
}

GVIDMW_API int tshEraseBmp(char winth)
{
	pRadarDisplay -> ML_GVID_EraseBmp(winth);
	return 1;
}



GVIDMW_API int tshGetSurfaceDC(HDC* dc, char VideoChannel,int surfType)
{
	return 0;
}
GVIDMW_API int tshReleaseSurfaceDC(HDC dc, char VideoChannel,int surfType)
{
	return 0;
}

// !! 注意，colorKey的类型是COLORREF，不同于标准接口中的颜色格式，
// 使用宏RGB(r,g,b)生成，格式为：由高到低，保留、B、G、R   ( 0x00BBGGRR )
GVIDMW_API int tshSetSurfaceColorKey(char VideoChannel,int surfType,COLORREF colorkey)
{
	return 0;
}
GVIDMW_API int tshSetSurfaceVisible(char VideoChannel,int surfType,BOOL visible)
{
	return 0;
}
GVIDMW_API int tshEraseSurface(char VideoChannel,int surfType)
{
	return 0;
}

// ========== 电视相关扩展接口 ==========

// 设置OSD层显示字符或时间
// XY坐标为字符串“左上角”相对视频“左上角”的坐标,字符最大长度32
GVIDMW_API int tshSetOSDStr(char VideoChannel,char* pstr,short x,short y)
{
	return 0;
}
GVIDMW_API int tshSetOSDTime(char VideoChannel,short x,short y)
{
	return 0;
}

// 发送透明数据至设备端串口，pData 数据; nLen 数据长度; nChnNum ---- 0为485接口,1为232接口
GVIDMW_API int tshSendTransparentData(char VideoChannel,char* pData,int nLen,int nChnNum)
{
	return 0;
}
// 校时,将系统时间发送到设备端
GVIDMW_API int tshVerifyTime(char VideoChannel)
{
	return 0;
}







