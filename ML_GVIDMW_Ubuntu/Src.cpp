//Name:   Scheduler.h
//Date:   Feb 28, 2012
//Author: GuoYan in the Canteen Rohm Building, Tsinghua University

#include <time.h>

#include <windows.h>
#include <stdio.h>

#include "bmp.h"
#include "CRadarDisplay.h"
#include "CWindowsManager.h"
#include "CGraphicsManager.h"
#include "CPPI.h"
#include "CDataPool.h"

s_ppi_video_for_disp * dispbuf1;      //全局变量，这几个变量之所以放在外面，是因为不止个函数需要用到它们
s_ppi_video_for_disp* dispdat1;

s_ppi_video_for_disp * dispbuf2;
s_ppi_video_for_disp* dispdat2;

BMPImageRec* pBackMap = NULL;

//#define _TREADBUFNUM 3600*7
#define _TREADBUFNUM 3600 * 3

bool loadPPIData();
void CALLBACK radarTimerProc(HWND hwnd,UINT uMsg, UINT idEvent, DWORD dwTime);


int SrcData()
{
	// 准备数据：包括海图数据以及雷达数据
	pBackMap = loadBMP("Images/backmap1.bmp");       // 上载背景海图，为之后的显示做好准备
	if(pBackMap == NULL)
	{
		return -1;
	}

	clock_t start = clock();

	loadPPIData();        // 上载两路雷达数据，为之后的显示做好准备

	clock_t  end = clock();
	double d = (double)(end - start) ;    /// CLOCKS_PER_SEC


	dispdat1 = new s_ppi_video_for_disp;            // 指示器，指示数据读取的当前位置
	dispdat1 = dispbuf1;

	dispdat2 = new s_ppi_video_for_disp;            // 指示器，指示数据读取的当前位置
	dispdat2 = dispbuf2;


	//CRadarDisplay* pRadarDisplay = new CRadarDisplay();   //


	return 0;
}


bool loadPPIData()
{
	FILE * rpkgfp;
	FILE * hpkgfp;
	bool isOK;

	// 打开文件
	if( (rpkgfp=fopen("./save/radar1","rb"))==NULL )
	{
		printf("error open pkg file\n");
		isOK=false;
	}
	else
		isOK=true;


	if( (hpkgfp=fopen("./save/radar2","rb"))==NULL )
	{
		printf("error open pkg file\n");
		isOK=false;
	}
	else
		isOK=true;

	if(isOK==false)return false;


	// 读取两路雷达数据，一路读取_TREADBUFNUM个包，存放在内存中
	dispbuf1 = new s_ppi_video_for_disp[_TREADBUFNUM]; 
	if(dispbuf1==NULL)isOK=false;
	fread(dispbuf1,sizeof(s_ppi_video_for_disp),_TREADBUFNUM,rpkgfp);

	dispbuf2 = new s_ppi_video_for_disp[_TREADBUFNUM];
	if(dispbuf2==NULL)isOK=false;
	fread(dispbuf2,sizeof(s_ppi_video_for_disp),_TREADBUFNUM,hpkgfp);


	return isOK;

}