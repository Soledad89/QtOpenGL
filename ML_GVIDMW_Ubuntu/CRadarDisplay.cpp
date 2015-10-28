// Name:   CRadarDisplay.cpp
// Author: Yan Guo
// Date:   Feb 27, 2013 in the canteen of Rohm Building
#include <time.h>

#include "netRadar.h"

//#include <windows.h>

#include "CRadarDisplay.h"
#include "bmp.h"
#include "ppidef.h"
#include "ML_GVIDMW.h"
#include "CAR.h"
#include "CPPI.h"
#include "CYH.h"

#pragma comment(lib, "glew32.lib")

#include "tv.h"
#include "ML_GVIDMW_sys.h"

extern BMPImageRec* pBackMap;

BMPImageRec* layerForUser[WINDOW_NUM];
int startX[WINDOW_NUM];
int startY[WINDOW_NUM];


CPPI* pPPI[2];
CAR* pAR[2];
CYH* pYH[2];
CDataPool* pDataPool;

TVPlayer *pTVlayer;

CNetRadar* pNetRadar;

CRITICAL_SECTION csTex;
HANDLE thread;

unsigned char ScanMode = 0;

char VideoChannelTmp;
unsigned int ColorTmp;
bool SetPPIColor = 0;;

// �������ܣ�ÿ��һ��ʱ�䣬��pRadarData1��pRadarData2�е�������ʾ����
void CALLBACK radarTimerProc(HWND hwnd,UINT uMsg, UINT idEvent, DWORD dwTime)
{
	EnterCriticalSection(&csTex);
	if (SetPPIColor)
	{
		pDataPool -> setColorTable(pDataPool -> colortablePPI[VideoChannelTmp - 1], ColorTmp);
		SetPPIColor = 0;
	}

	pTVlayer ->TVUpdate();
	pPPI[0] -> PPIUpdate();
	pPPI[0] -> WinUpdate();
	pAR[0] -> ARUpdate();              // ����ҳFBO����ͼ����Windows�����е�����ʾ
	pPPI[1] -> PPIUpdate();
	pPPI[1] -> WinUpdate();
	pAR[1] -> ARUpdate();

	LeaveCriticalSection(&csTex);

}

//int nn = 0;

void CALLBACK yhTimerProc(HWND hwnd,UINT uMsg, UINT idEvent, DWORD dwTime)
{

	//EnterCriticalSection(&csTex);
	//ML_GVID_SetPPIColor(1, 0x00ff00ff);
	//LeaveCriticalSection(&csTex);

	EnterCriticalSection(&csTex);
	pYH[0] -> yhPPI();
	pPPI[0] -> PPIUpdate();
	pYH[1] -> yhPPI();
	pPPI[1] -> PPIUpdate();
	LeaveCriticalSection(&csTex);

	//if(nn == 100)
	//{
	//	//EnterCriticalSection(&csTex);
		//ML_GVID_SetPPIColor(1, 0x00ff00ff);
	//	//LeaveCriticalSection(&csTex);

	//	nn = 0;
	//}
	//else
	//{
	//	nn ++;
	//}

	//ML_GVID_SetPPIColor(1, 0x00ff00ff);

	//EnterCriticalSection(&csTex);
	//ML_GVID_SetPPIColor(1, 0xffff00ff);
	//LeaveCriticalSection(&csTex);


}


DWORD WINAPI Loop(void* pParam)         //LPTHREAD_START_ROUTINE  WINAPI
{
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))         // �����￪ʼ������ѭ��
	{ 
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


CRadarDisplay::CRadarDisplay()
{
	ML_GVID_Init();
}


//�������ܣ�����8�����ڣ�Ϊ��8�����ڴ�����OpenGL��Ⱦ���������ڴ˻����´���8����Ӧ��FBO
int CRadarDisplay::ML_GVID_Init()
{
	InitializeCriticalSection(&csTex);

	pDataPool = new CDataPool();	
	pWindowsManager = new CWindowsManager();        // �������ڣ�pGraphicsManager�Ѿ����������
	HDC* hDC = pWindowsManager -> getHDC();
	pGraphicsManager = new CGraphicsManager(hDC, 8);

	HWND* hWnd = pWindowsManager -> getHWND();	//No usage???????????????????
	pTVlayer = new TVPlayer(pGraphicsManager, 1);
	

	for(int i = 0; i < 2; i ++)
	{
		pAR[i] = new CAR(pGraphicsManager, i + 1);
		pPPI[i] = new CPPI(pDataPool, pGraphicsManager, i + 1);       //����PPI
		pYH[i] = new CYH(pDataPool, pGraphicsManager, i + 1); 
	}

	CNetRadar* pNetRadar = new CNetRadar;
	pNetRadar->init();




	BMPImageRec*  pBackMap = loadBMP("Images/backmap1.bmp");       // ���ر�����ͼ��Ϊ֮�����ʾ����׼��
	if(pBackMap == NULL)
		return -1;
	pPPI[0] -> PPIDrawBackMap(pBackMap);
	pPPI[1] -> PPIDrawBackMap(pBackMap);



	//-------PPI����--------
	ML_GVID_OpenPPI(1, 0, 0, 600, 600);   
	ML_GVID_OpenPPI(2, 600, 0, 300, 300);    

	ML_GVID_ClosePPI(1);
	ML_GVID_SetPPIXY(1 , 100, 0);
	ML_GVID_SetPPIWH(1 , 600, 600);
	ML_GVID_SetPPIMode(1, 2);
	ML_GVID_SetPPICenter(1, 100, 600);
	ML_GVID_SetPPIShipDir(1, 500);
	ML_GVID_SetPPIColor(1, 0x00ff00ff);     // �Ӹ����ͣ� RGBA
	ML_GVID_SetPPIScan(1, 1);
	ML_GVID_SetPPILineColor(1, 0x0000ffff);
	ML_GVID_SetPPIDist(1, 500);
	ML_GVID_SetPPISwitch(1, 0);
	ML_GVID_SetPPIGate(1, 250);
	ML_GVID_SetPPIBackgroundColor(1, 0xff0000ff);
	ML_GVID_ClearPPI(1);

	ML_GVID_SetPPITail(1,100);
	ML_GVID_SetPPITailSwitch(1,1);
	ML_GVID_SetPPITailColor(1,0x0000ffff);      
	ML_GVID_SetPPITailGate(1,200);
	ML_GVID_SetPPITailTimer(1,30);

	//-------MiniWin����--------
	ML_GVID_OpenMiniWin(1, 300, 0, 200, 200);
	ML_GVID_OpenMiniWin(2, 900, 0, 200, 200);
	ML_GVID_CloseMiniWin(1);
	ML_GVID_SetMiniWinXY(1 , 100, 0);
	ML_GVID_SetMiniWinWH(1 , 600, 600);
	ML_GVID_SetMiniWinColor(1, 0xff0000ff);
	ML_GVID_SetMiniWinScale(1, 3);
	ML_GVID_SetMiniWinBackgroundColor(1, 0xff00007f);

	//-------AR����--------
	ML_GVID_OpenAr(1, 300, 300, 200, 200);
	ML_GVID_SetArTail(1, 65535);
	ML_GVID_OpenAr(2, 900, 300, 200, 200);
	ML_GVID_CloseAr(1);
	ML_GVID_SetArXY(1 , 100, 0);
	ML_GVID_SetArWH(1 , 600, 600);
	ML_GVID_SetArColor(1, 0x0000ff00);
	ML_GVID_SetMiniWinScale(1, 3);
	ML_GVID_SetArBackgroundColor(1, 1114112);


	ML_GVID_OpenPPI(2, 600, 0, 300, 300);   
	ML_GVID_OpenMiniWin(2, 900, 0, 200, 200);

	ML_GVID_OpenTV(1, 0, 0, 500, 500); 
	ML_GVID_OpenTV(2, 600, 0, 500, 500); 

	ML_GVID_CloseTV (1);
	ML_GVID_SetTVXY(1,200, 200);
	ML_GVID_SetTVWH(1,300, 300);
	ML_GVID_FrostTV(1,0);
	ML_GVID_SetTVBright(1,100);
	ML_GVID_SetTVContrast(1,100);
	ML_GVID_SetTVColor(1,100);
	//ML_GVID_SetPMLayerColors(1, 0, 0,colorTable );
	ML_GVIDMW_RadarPkg_t *pkgBuf = new ML_GVIDMW_RadarPkg_t;
	pkgBuf->azi =0;
	pkgBuf ->elev =0;
	//pkgBuf ->video = NULL;
	ML_GVID_SendRadarPkg(1, 32, pkgBuf);
	BMPImageRec *boat = loadBMP("Images/boat.bmp"); 
	tshGetBmp(boat, 1, 550, 550) ;
	unsigned int colors[2];
	colors[0] = 0;
	colors[1] = 0x0000ff00;
	ML_GVID_SetPMLayerColors(1,0, 0, colors);

	BMPImageRec *Paint = loadBMP("Images/forUser.bmp"); 
	tshGetBmp(Paint, 3, 50, 50) ;
	tshGetBmp(Paint, 7, 50, 50) ;
	unsigned int color;
	color = 0x00000000;
	ML_GVID_SetATLayerColorKey(1,color);

	ML_GVID_EraseBackMap(1);
	ML_GVID_EraseBmp(3);

	SetTimer(NULL, 0, 40, radarTimerProc);          // ����ʱ�����ú�
	SetTimer(NULL, 0, 24, yhTimerProc);


	DWORD threadID;
	thread = CreateThread(NULL,0,Loop,NULL,0,&threadID);
	if(thread==NULL) return -1;

	//MSG msg;
	//while(GetMessage(&msg, NULL, 0, 0))         // �����￪ʼ������ѭ��
	//{ 
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}

	return 0;
}


//------------------------------------------------------------PPI----------------------------------------------------------//

//�������ܣ���PPI����
int CRadarDisplay::ML_GVID_OpenPPI(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	char winth = 3 * (VideoChannel - 1) + 1;

	pWindowsManager -> setWindowXY(winth, theX, theY);
	pWindowsManager -> setWindowWH(winth, theWidth, theHeight);
	pWindowsManager -> updateWindow(winth);

	pWindowsManager -> openWindow(winth);

	return 0;
}

//�������ܣ��ر�PPI����
int CRadarDisplay::ML_GVID_ClosePPI(char VideoChannel)
{
	char winth = 3 * (VideoChannel - 1) + 1;
	pWindowsManager -> closeWindow(winth);

	return 0;
}


//�������ܣ����ô���λ�ã����Ͻǣ�
int CRadarDisplay::ML_GVID_SetPPIXY(char VideoChannel ,short theX ,short theY)
{
	char winth = 3 * (VideoChannel - 1) + 1;

	pWindowsManager -> setWindowXY(winth, theX, theY);
	pWindowsManager -> updateWindow(winth);

	return 0;
}


//�������ܣ����ô��ڴ�С
int CRadarDisplay::ML_GVID_SetPPIWH(char VideoChannel ,short theWidth, short theHeight)
{
	char winth = 3 * (VideoChannel - 1) + 1;

	pWindowsManager -> setWindowWH(winth, theWidth, theHeight);
	pWindowsManager -> updateWindow(winth);


	return 0;
}


//�������ܣ�ֻ���Ƶ��ӡ��ر�ʱ���������������ڹر�״̬ʱ��PPI��ҳ��������ƣ������в����ӡ�
int CRadarDisplay::ML_GVID_SetLayerSwitch(char VideoChannel,int Layer,short theSwitch)
{
	switch(Layer)
	{
	case SURF_PPI_TOP:
		pPPI[VideoChannel - 1] -> PPISetVisible(1, 1, theSwitch);
		break;

	case SURF_PPI_MIDDLE:
		pPPI[VideoChannel - 1] -> PPISetVisible(1, 2, theSwitch);
		break;

	case SURF_PPI_BOTTOM:
		pPPI[VideoChannel - 1] -> PPISetVisible(1, 3, theSwitch);
		break;

	case SURF_MINI_TOP:
		pPPI[VideoChannel - 1] -> PPISetVisible(2, 1, theSwitch);
		break;

	case SURF_MINI_BOTTOM:
		pPPI[VideoChannel - 1] -> PPISetVisible(2, 2, theSwitch);		
		break;

	case SURF_AR_TOP:
		pAR[VideoChannel - 1] -> ARSetVisible(1, theSwitch);
		break;

	case SURF_AR_BOTTOM:
		pAR[VideoChannel - 1] -> ARSetVisible(2, theSwitch);
		break;

	case SURF_TV_TOP:
		break;

	case SURF_TV_BOTTOM:
		break;
	}

	return 0;
}



//�������ܣ�����PPI��ʾģʽ
int CRadarDisplay::ML_GVID_SetPPIMode(char VideoChannel, short theMode)
{
	pPPI[VideoChannel - 1] -> PPISetDispMode(theMode);

	return 0;	
}

//�������ܣ�����P�Ե�����λ��
int CRadarDisplay::ML_GVID_SetPPICenter(char VideoChannel ,short theX ,short theY)
{
	pPPI[VideoChannel - 1] -> PPISetDispCenter(theX, theY);

	return 0;
}

//�������ܣ����ý�����λ
int CRadarDisplay::ML_GVID_SetPPIShipDir(char VideoChannel,float theShipDir)
{
	pPPI[VideoChannel - 1] -> PPISetShipDir(theShipDir);

	return 0;
}


//�������ܣ�����PPI��ʾ����ɫ
int CRadarDisplay::ML_GVID_SetPPIColor(char VideoChannel,unsigned int theColor)
{
	pPPI[VideoChannel - 1] -> PPIClearLayer();
	pPPI[VideoChannel - 1] -> PPIClear(2);
	pPPI[VideoChannel - 1] -> WinClear(2);

	VideoChannelTmp = VideoChannel;
	ColorTmp = theColor;

	SetPPIColor = 1;

	/*EnterCriticalSection(&csTex);
	pDataPool -> setColorTable(pDataPool -> colortablePPI[VideoChannel - 1], theColor);
	LeaveCriticalSection(&csTex);*/
	
	//pDataPool -> setColorTable(pDataPool -> colortablePPI[VideoChannel - 1], theColor);

	return 0;
}


//�������ܣ�����PPI��ʾ����ɫ
int CRadarDisplay::ML_GVID_SetPPIColorTable(char VideoChannel,unsigned int* colorTable)
{
	pDataPool -> setColorTable(pDataPool -> colortablePPI[VideoChannel - 1], colorTable);

	return 0;
}


//�������ܣ������Ƿ���ʾPPIɨ����
int CRadarDisplay::ML_GVID_SetPPIScan(char VideoChannel,int theScan)
{
	pPPI[VideoChannel - 1] -> PPISetScan(theScan);

	return 0;
}

//�������ܣ�����PPIɨ���ߵ���ɫ
int CRadarDisplay::ML_GVID_SetPPILineColor(char VideoChannel,unsigned int theColor)
{
	pPPI[VideoChannel - 1] -> PPISetScanLineColor(theColor);

	return 0;
}

//�������ܣ������Ƿ񶳽�PPI��Ƶ�������ʱ�����ݻ���
int CRadarDisplay::ML_GVID_SetPPIFrost(char VideoChannel,char theFrost)
{
	if(!theFrost)          // �ⶳʱ��������
	{
		pPPI[VideoChannel - 1] -> PPIClearLayer();
		pPPI[VideoChannel - 1] -> PPIClear(2);
		pPPI[VideoChannel - 1] -> WinClear(2);
		pAR[VideoChannel - 1] -> ARClear(2); 
	}

	pPPI[VideoChannel - 1] -> PPISetFrost(theFrost);
	pAR[VideoChannel - 1] -> ARSetFrost(theFrost);

	return 0;
}


//�������ܣ�����PPI����
int CRadarDisplay::ML_GVID_SetPPIDist(char VideoChannel,short theDist)
{
	//rangePPI[VideoChannel] = theDist;
	pPPI[VideoChannel - 1] -> PPISetRange(theDist);
	pAR[VideoChannel - 1] -> ARsetPPIRange(theDist);

	return 0;
}


//�������ܣ���һ·ͨ����3���״����ݲ㣬���ƺ�ҳ���ƣ��ر�ʱ�������������ڹر�״̬ʱ��PPI��ҳ�㲻���ƣ����ӡ�
int CRadarDisplay::ML_GVID_SetPPISwitch(char VideoChannel,short theSwitch)
{
	if(!theSwitch)          // �ر�ʱ��������
	{
		pPPI[VideoChannel - 1] -> PPIClearLayer();
		pPPI[VideoChannel - 1] -> PPIClear(2);
		pPPI[VideoChannel - 1] -> WinClear(2);
		pAR[VideoChannel - 1] -> ARClear(2); 
	}

	pPPI[VideoChannel - 1] -> PPISetSwitch(theSwitch);
	pAR[VideoChannel - 1] -> ARSetSwitch(theSwitch);

	return 0;
}

//�������ܣ�����PPI����ֵ��ֻ�д���һ������ֵ�����ݲŻ���ʾ
int CRadarDisplay::ML_GVID_SetPPIGate(char VideoChannel,unsigned char theGate)
{
	pPPI[VideoChannel - 1] -> PPISetGate(theGate);
	pYH[VideoChannel - 1] -> setYHGate(theGate);

	return 0;
}

//�������ܣ�����PPI���ͳ̶ȣ���theTail = 0ʱ������������˥�� ��ΪԽ��˥��Խ��
int CRadarDisplay::ML_GVID_SetPPITail(char VideoChannel,int theTail)
{
	pYH[VideoChannel - 1] -> setPPIDecay(255/(theTail + 0.01) * 24);

	return 0;
}


//�������ܣ�����PPIβ������
int CRadarDisplay::ML_GVID_SetPPITailSwitch(char VideoChannel,short theSwitch)      //β�������״�ز�����ֵ����һ������ʱ���Ż���ʾ
{
	pYH[VideoChannel - 1] -> setPPITailSwitch(theSwitch);

	return 0;
}


//�������ܣ�����PPIβ����ɫֵ
int CRadarDisplay::ML_GVID_SetPPITailColor(char VideoChannel,unsigned int theColor)
{
	pDataPool -> setColorTable(pDataPool -> colortableTail[VideoChannel - 1], theColor);

	return 0;
}


//�������ܣ�����PPIβ������
int CRadarDisplay::ML_GVID_SetPPITailGate(char VideoChannel,unsigned char theGate)
{
	pYH[VideoChannel - 1] -> setPPITailGate(theGate);

	return 0;
}

//�������ܣ�����PPIβ��ʱ��   theTime��ʾβ������ʱ�䣬Խ��˥��Խ��
int CRadarDisplay::ML_GVID_SetPPITailTimer(char VideoChannel,int theTime)
{
	pYH[VideoChannel - 1] -> setPPITailTimer(255/(theTime + 0.01) * 24);

	return 0;
}


//�������ܣ�����PPI�ı�����ɫ
int CRadarDisplay::ML_GVID_SetPPIBackgroundColor(char VideoChannel,unsigned int theColor)
{
	pPPI[VideoChannel - 1] -> PPISetBackgroundColor(theColor);

	return 0;
}

//int CRadarDisplay::ML_GVID_SetPPIColorTable();


// �������ܣ����PPI��ʾ��������
int CRadarDisplay::ML_GVID_ClearPPI(char VideoChannel)
{
	pPPI[VideoChannel - 1] -> PPIClear(2);        // PPI�״����ݲ�    ��1�����û����㣬��2�㣺�״����ݲ�,��3�㣺������ͼ�� 

	return 0;
}


// �������ܣ���·�״���Ƶͬ������ʾ
int CRadarDisplay::ML_GVID_SetPPITogether(short theSwitch)
{
	pPPI[0] -> PPISetTogether(theSwitch);
	return 0;
}


int CRadarDisplay::ML_GVID_SetMoveMode(char VideoChannel, short theMode)
{
	pPPI[0] -> PPISetMoveMode(theMode);
	return 0;
}

// �������ܣ�ʹ����·�״���ͬһ��������ʾ
//int ML_GVID_PPITogether(char VideoChannel, short theSwitch);





//------------------------------------------------------------MiniWin----------------------------------------------------------//

//------------------------------------------------------------MiniWin----------------------------------------------------------//

// �������ܣ���С����
int CRadarDisplay::ML_GVID_OpenMiniWin(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	char winth = 3 * (VideoChannel - 1) + 2;

	pWindowsManager -> setWindowXY(winth, theX, theY);
	pWindowsManager -> setWindowWH(winth, theWidth, theHeight);
	pWindowsManager -> updateWindow(winth);

	pWindowsManager -> openWindow(winth);      // ShowWindow  UpdateWindow

	return 0;
}


// �������ܣ��ر�С���ڣ�ֻ�ǹرգ�����̨��Ȼ�ڸ���
int CRadarDisplay::ML_GVID_CloseMiniWin(char VideoChannel)
{
	char winth = 3 * (VideoChannel - 1) + 2;
	pWindowsManager -> closeWindow(winth);    //ShowWindow Hide

	return 0;
}


// �������ܣ�����С����λ�ã����Ͻǣ�
int CRadarDisplay::ML_GVID_SetMiniWinXY(char VideoChannel ,short theX ,short theY)
{
	char winth = 3 * (VideoChannel - 1) + 2;

	pWindowsManager -> setWindowXY(winth, theX, theY);
	pWindowsManager -> updateWindow(winth);

	return 0;
}


// �������ܣ����ô��ڴ�С�ߴ�
int CRadarDisplay::ML_GVID_SetMiniWinWH(char VideoChannel ,short theWidth, short theHeight)
{
	char winth = 3 * (VideoChannel - 1) + 2;

	pWindowsManager -> setWindowWH(winth, theWidth, theHeight);
	pWindowsManager -> updateWindow(winth);

	return 0;
}


// �������ܣ�
//int CRadarDisplay::ML_GVID_SetMiniWinMode(char VideoChannel, short theMode)
//{
//	pPPI[VideoChannel - 1] -> WinSetMode(theMode);
//	return 0;
//}


// �������ܣ�����С�������״���Ƶ��ʾ����ɫ
int CRadarDisplay::ML_GVID_SetMiniWinColor(char VideoChannel,unsigned int theColor)
{
	pDataPool -> setColorTable(pDataPool -> colortableWin[VideoChannel - 1], theColor);

	return 0;
}


//�������ܣ�����PPI��ʾ����ɫ
int CRadarDisplay::ML_GVID_SetMiniWinColorTable(char VideoChannel,unsigned int* colorTable)
{
	//for(int i=0;i<256;i++)
	//{
	//	int r, g, b, a;              // OpenGL�ڽ���ʱ���ӵ͵�ַ���ߵ�ַ��˳���ǰ�RGBA���н��룬��ˣ��ڵ�ַ�� int -> char ��ǿ��ת����Ҫע��
 //                                // ������洢�������ݵ�˳���ǣ����ֽ��ڵ͵�ַ�����ֽ��ڸߵ�ַ������ڼ�����У��ӵ͵�ַ���ߵ�ַ��˳��洢���ǣ�ABGR
	//	r = colorTable[i] >> 24;
	//	g = (colorTable[i] >> 16) & 0xff;
	//	b = (colorTable[i] >> 8) & 0xff;
	//	a = 0xff;

	//	colorTable[i] = (unsigned int)( ((a&0xff)<<24) | ((b&0xff)<<16) | ((g&0xff)<<8) | (r&0xff) );
	//}
	
	pDataPool -> setColorTable(pDataPool -> colortableWin[VideoChannel - 1], colorTable);

	return 0;
}


// �������ܣ������״���ƵС���ھ���ͷ�λ�����ݾ���ͷ�λ����ȷ��Ҫ���Ĵ��ڵ�����
int CRadarDisplay::ML_GVID_SetMiniWinDistDir(char VideoChannel ,short theDistance ,float theDirection)
{
	pPPI[VideoChannel - 1] -> WinSetDistDir(theDistance, theDirection);

	return 0;
}


// �������ܣ�������ƵС���ڵķŴ���
int CRadarDisplay::ML_GVID_SetMiniWinScale(char VideoChannel,int theScale)
{
	pPPI[VideoChannel - 1] -> WinSetScale(theScale);

	return 0;
}


//�������ܣ�����С�������״����ݲ�ı�����ɫ
int CRadarDisplay::ML_GVID_SetMiniWinBackgroundColor(char VideoChannel,unsigned int theColor)
{
	pPPI[VideoChannel - 1] -> WinSetBackgroundColor(theColor);

	return 0;
}





//------------------------------------------------------------AR----------------------------------------------------------//

//------------------------------------------------------------AR----------------------------------------------------------//

int CRadarDisplay::ML_GVID_OpenAr(char VideoChannel,short theX, short theY , short theWidth, short theHeight)
{
	char winth = 3 * VideoChannel;      // ��Ӧ��3�͵�6������

	pWindowsManager -> setWindowXY(winth, theX, theY);
	pWindowsManager -> setWindowWH(winth, theWidth, theHeight);
	pWindowsManager -> updateWindow(winth);

	pWindowsManager -> openWindow(winth);
	return 0;
}

int CRadarDisplay::ML_GVID_CloseAr(char VideoChannel)
{
	char winth = 3 * VideoChannel;

	pWindowsManager -> closeWindow(winth);

	return 0;
}

int CRadarDisplay::ML_GVID_SetArXY(char VideoChannel ,short theX ,short theY)
{
	char winth = 3 * VideoChannel;

	pWindowsManager -> setWindowXY(winth, theX, theY);
	pWindowsManager -> updateWindow(winth);

	return 0;
}

int CRadarDisplay::ML_GVID_SetArWH(char VideoChannel ,short theWidth, short theHeight)
{
	char winth = 3 * VideoChannel;

	pWindowsManager -> setWindowWH(winth, theWidth, theHeight);
	pWindowsManager -> updateWindow(winth);

	return 0;
}


//�������ܣ� ����ֵС��theGate�����ݲ���ʾ
int CRadarDisplay::ML_GVID_SetArGate (char VideoChannel ,unsigned char theGate)          //
{
	pAR[VideoChannel - 1] -> ARSetGate(theGate);

	return 0;
}


//�������ܣ� ��������
int CRadarDisplay::ML_GVID_SetArDist (char VideoChannel ,short theDist)
{
	pAR[VideoChannel - 1] -> ARSetDist(theDist);

	return 0;
}


 // �������ܣ�����AR����˥���̶ȣ�theTailֵԽ��˥��Խǿ ��ΪtheTail��ʾ��Ա���ʱ�䣬Խ��˥��Խ��
int CRadarDisplay::ML_GVID_SetARTail(char VideoChannel,int theTail)
{
	pAR[VideoChannel - 1] -> ARSetTail(theTail);

	return 0;
}



//�������ܣ� ����AR�ߵ���ɫ
int CRadarDisplay::ML_GVID_SetArColor(char VideoChannel,unsigned int theColor)
{
	pAR[VideoChannel - 1] -> ARSetColor(theColor);

	return 0;
}


//�������ܣ� ����AR�״����ݲ�ı�����ɫ
int CRadarDisplay::ML_GVID_SetArBackgroundColor(char VideoChannel,unsigned int theColor)
{
	pAR[VideoChannel - 1] -> ARSetBackgroundColor(theColor);

	return 0;
}

int CRadarDisplay::ML_GVID_SetScanMode(char VideoChannel, short theScanMode)
{
	pNetRadar -> SetScanMode(theScanMode);
	return 0;
}




// �������ܣ� ��������ڲ�Ҫ���Ե��״����ݻ��Ƶ���ҳFBO��
int CRadarDisplay::ML_GVID_SendRadarPkg(char VideoChannel, int pkgNum, ML_GVIDMW_RadarPkg_t *pkgBuf)
{
	s_ppi_video_for_disp* dispbuf = new s_ppi_video_for_disp;

	for(int i = 0; i < pkgNum; i ++)
	{
		convertRadarPkg(VideoChannel, pkgBuf + i, dispbuf);

		pPPI[VideoChannel - 1] -> PPIDraw(dispbuf); 
		pAR[VideoChannel - 1] -> ARDraw(dispbuf);
		pYH[VideoChannel - 1] -> yhPPI();
	}

	return 0;
}



// �������ܣ� �������ϴ��������ݰ����в���
 int CRadarDisplay::convertRadarPkg(char VideoChannel, ML_GVIDMW_RadarPkg_t* dat, s_ppi_video_for_disp* dispbuf)
{
	static int lastAzi1 = 0;
	static int lastAzi2 = 0;

	int lastAzi;
	int curAzi;

	curAzi = dat->azi;

	if(VideoChannel == 1)
		lastAzi = lastAzi1;
	else if(VideoChannel==2)
		lastAzi = lastAzi2;
	else
		return -1;

	if(curAzi>lastAzi && curAzi<lastAzi+20)                        // 0 ~ 20 ֮��
	{
		dispbuf->znum=curAzi-lastAzi;
		for(int i=0;i<dispbuf->znum && i<max_azi_num_per_video;i++)
			dispbuf->azicnt[i]=lastAzi+i;
	}
	else if(curAzi<lastAzi && curAzi<20 && lastAzi > 4096-20)     // ��һȦ
	{
		dispbuf->znum=4096+curAzi-lastAzi;
		for(int i=0;i<dispbuf->znum && i<max_azi_num_per_video;i++)
			dispbuf->azicnt[i]=(lastAzi+i)%4096;
	}
	else
	{
		dispbuf->znum=1;
		dispbuf->azicnt[0]=curAzi;
	}

	if(VideoChannel==1)
		lastAzi1 = curAzi;
	else
		lastAzi2 = curAzi;

	memcpy(dispbuf->videodata,dat->video,1200);        //pData��ǰ�ĸ��ֽ�Ϊ������Ϣ

	return 0;
}



	//------------------------------------TV----------------------------------------------
int CRadarDisplay::ML_GVID_OpenTV (char VideoChannel ,short theX, short theY,short theWidth,short theHeight)
{
	int winth = 6 + VideoChannel;
	pWindowsManager -> setWindowXY(winth, theX, theY);
	pWindowsManager -> setWindowWH(winth, theWidth, theHeight);
	pWindowsManager -> updateWindow(winth);	
	pWindowsManager -> openWindow(winth);


	pTVlayer -> ML_GVID_OpenTv(-1, VideoChannel, theX, theY, theWidth, theHeight);
	return 0;
}


int CRadarDisplay::ML_GVID_CloseTV (char VideoChannel)
{
	pTVlayer -> ML_GVID_CloseTv(-1, VideoChannel);

	int winth = 6 + VideoChannel;
	pWindowsManager -> closeWindow(winth);

	return 0;
}


int CRadarDisplay::ML_GVID_SetTVXY(char VideoChannel ,short theX, short theY)
{
	int winth = 6 + VideoChannel;

	pWindowsManager -> setWindowXY(winth, theX, theY);
	pWindowsManager -> updateWindow(winth);

	pTVlayer -> ML_GVID_SetTvXY(-1, VideoChannel, theX, theY);
	return 0;
}


int CRadarDisplay::ML_GVID_SetTVWH(char VideoChannel ,short theWidth, short theHeight)
{
	int winth = 6 + VideoChannel;

	pWindowsManager -> setWindowWH(winth, theWidth, theHeight);
	pWindowsManager -> updateWindow(winth);

	pTVlayer -> ML_GVID_SetTvWH(-1, VideoChannel, theWidth, theHeight);
	return 0;
}


int CRadarDisplay::ML_GVID_FrostTV(char VideoChannel,char theFrost)
{
	if(theFrost==0)
	{
		pTVlayer -> ML_GVID_FrostTV(-1, VideoChannel);
	}
	else if(theFrost==1)
	{
		pTVlayer -> ML_GVID_UnFrostTV(-1,VideoChannel);
	}
	else
	{
		return -1;
	}
	
	return 0;
}


int CRadarDisplay::ML_GVID_SetTVBright(char VideoChannel,unsigned char theBright)
{
	pTVlayer -> ML_GVID_ChangTvBright(-1, VideoChannel, theBright);
	return 0;
}


int CRadarDisplay::ML_GVID_SetTVContrast(char VideoChannel,unsigned char theContrast)
{
	pTVlayer -> ML_GVID_ChangTvContrast(-1, VideoChannel, theContrast);
	return 0;
}


int CRadarDisplay::ML_GVID_SetTVColor(char VideoChannel,unsigned char theColor)
{
	pTVlayer -> ML_GVID_ChangTvColor(-1, VideoChannel, theColor);
	return 0;
}

int CRadarDisplay::ML_GVID_SetPMLayerColors(char VideoChannel, char theMode, int theNum, unsigned int *colors)
{
	pPPI[VideoChannel - 1] -> PPISetBackgroundColor(colors[0]);
                 //�����м�㱳��ɫ��͸����Ϊ0
	pPPI[VideoChannel-1] ->WinSetBackgroundColor(colors[0]);

	pPPI[VideoChannel - 1] -> PPIsetColorKey(colors[1],layerForUser[3 * (VideoChannel - 1)],startX[3 * (VideoChannel - 1)], startY[3 * (VideoChannel - 1)]);
	pPPI[VideoChannel - 1] -> WinSetColorKey(colors[1],layerForUser[3 * (VideoChannel - 1) + 1], startX[3 * (VideoChannel - 1) + 1], startY[3 * (VideoChannel - 1) + 1]);
	return 0;
}


int CRadarDisplay::ML_GVID_SetATLayerColorKey(char VideoChannel, unsigned int colorKey)
{
	pAR[VideoChannel - 1] -> ARsetColorKey(colorKey,layerForUser[3 * (VideoChannel - 1) + 2],startX[3 * (VideoChannel - 1) + 2],startY[3 * (VideoChannel - 1) + 2]);
	pTVlayer -> TVsetColorKey(VideoChannel+6,colorKey,layerForUser[VideoChannel+5],startX[VideoChannel+5],startY[VideoChannel+5]);
	return 0;
}


CRadarDisplay::~CRadarDisplay()
{
	delete pWindowsManager;
	delete pDataPool;
	for(int i = 0; i < 2; i ++)
	{
		delete []pPPI[i];
		delete []pAR[i];
		//delete []pYH[i];

		//delete []pRadarDataPkg[i];
	}

	delete dispbuf;
}

int CRadarDisplay::ML_GVID_GetBmp(char winth, BMPImageRec *bmp, int x, int y)     //������㱣��
{
	layerForUser[winth - 1] = bmp;
	startX[winth - 1] = x;
	startY[winth - 1] = y;
	return 0;
}

int CRadarDisplay::ML_GVID_EraseBmp(char winth)     //�������
{
	pGraphicsManager->clear(winth,1,0x00000000);
	return 0;
}

int CRadarDisplay::ML_GVID_GetBackMap(BMPImageRec *bmp, char VideoChannel)
{
	pPPI[VideoChannel - 1] -> PPIDrawBackMap(bmp);
	return 0;
}

int CRadarDisplay::ML_GVID_EraseBackMap(char VideoChannel)
{
	pGraphicsManager->clear(3*(VideoChannel - 1) + 1,1,0x000000ff);
	return 0;
}