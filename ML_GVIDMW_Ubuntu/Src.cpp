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

s_ppi_video_for_disp * dispbuf1;      //ȫ�ֱ������⼸������֮���Է������棬����Ϊ��ֹ��������Ҫ�õ�����
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
	// ׼�����ݣ�������ͼ�����Լ��״�����
	pBackMap = loadBMP("Images/backmap1.bmp");       // ���ر�����ͼ��Ϊ֮�����ʾ����׼��
	if(pBackMap == NULL)
	{
		return -1;
	}

	clock_t start = clock();

	loadPPIData();        // ������·�״����ݣ�Ϊ֮�����ʾ����׼��

	clock_t  end = clock();
	double d = (double)(end - start) ;    /// CLOCKS_PER_SEC


	dispdat1 = new s_ppi_video_for_disp;            // ָʾ����ָʾ���ݶ�ȡ�ĵ�ǰλ��
	dispdat1 = dispbuf1;

	dispdat2 = new s_ppi_video_for_disp;            // ָʾ����ָʾ���ݶ�ȡ�ĵ�ǰλ��
	dispdat2 = dispbuf2;


	//CRadarDisplay* pRadarDisplay = new CRadarDisplay();   //


	return 0;
}


bool loadPPIData()
{
	FILE * rpkgfp;
	FILE * hpkgfp;
	bool isOK;

	// ���ļ�
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


	// ��ȡ��·�״����ݣ�һ·��ȡ_TREADBUFNUM������������ڴ���
	dispbuf1 = new s_ppi_video_for_disp[_TREADBUFNUM]; 
	if(dispbuf1==NULL)isOK=false;
	fread(dispbuf1,sizeof(s_ppi_video_for_disp),_TREADBUFNUM,rpkgfp);

	dispbuf2 = new s_ppi_video_for_disp[_TREADBUFNUM];
	if(dispbuf2==NULL)isOK=false;
	fread(dispbuf2,sizeof(s_ppi_video_for_disp),_TREADBUFNUM,hpkgfp);


	return isOK;

}