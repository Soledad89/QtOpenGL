#ifndef _TVPLAYER_H
#define _TVPLAYER_H

#include <stdio.h>
#include <windows.h>
#include "ZDYT_SDK.h"


//定义显示的电视通道数
#define TVChannelNum 2

typedef struct  TV_CFG
{
	bool play_flag;
	bool Frost_flag;
	short startX;
	short startY;
	short Width;
	short Height;
}TVcfg,*ptvcfg;

//视频

typedef struct USER_INFO
{
	char* pUrl;      // 访问硬件的IP地址
	char* pUserName; // 用户名
	char* pPwd;  // 密码  
}UserInfo,pUserInfo;
 
class TVPlayer
{

private:

	char VideoChannel;
	int winth;

	TVcfg   tv_cfg[TVChannelNum];                        //状态

	bool IsAudioPlay[TVChannelNum];

	UserInfo  userinfo[TVChannelNum];                 //用户信息

	CGraphicsManager* pGraphicsManager;               //显示窗口

	BMPImageRec *od[2];

	//ZDYT_SDK *zs[TVChannelNum];

public:
	ZDYT_SDK *zs;

public:
	TVPlayer(CGraphicsManager* pGM, char videoChannel);
	~TVPlayer();

	void TVUpdate();
	
	//---------------------------------------电视对外接口-----------------------------------------------------------
	//打开电视
	int ML_GVID_OpenTv (short theDisplay,short VideoChannel ,short theX, short theY,short theWidth,short theHeight);
	//关闭电视
	int ML_GVID_CloseTv (short theDisplay,short VideoChannel );
	//设置窗口位置
	int ML_GVID_SetTvXY(short theDisplay,short VideoChannel ,short theX, short theY);
	//设置窗口大小
	int ML_GVID_SetTvWH(short theDisplay,short VideoChannel ,short theWidth, short theHeight);
	//冻结电视
	void ML_GVID_FrostTV(short theDisplay,short VideoChannel);
	//解冻电视
	void ML_GVID_UnFrostTV(short theDisplay,short VideoChannel);
	//改变电视亮度
	void ML_GVID_ChangTvBright(short theDisplay,short VideoChannel,int theBright);
	//改变电视对比度
	void ML_GVID_ChangTvContrast(short theDisplay,short VideoChannel,int theContrast);
	//调整电视色彩
	void ML_GVID_ChangTvColor(short theDisplay,short VideoChannel,int theColor);

	
	int TVsetColorKey(char winth,unsigned int colorKey,BMPImageRec* pBackMap1, int x, int y);
	//---------------------------------------------结束---------------------------------------------------------------
	//记录音视频文件
	void tshStartRecordAVFile(short VideoChannel,char *FileName = NULL);
	void tshCloseRecordAVFile(short VideoChannel);
	void tshPlayAudio(short VideoChannel);
	void tshCloseAudio(short VideoChannel);
	bool tshSetOSDInfo(short VideoChannel,LPBYTE pOSDData,RECT OSD_Rect,DWORD dwCOlorSpcae);
	void tshSetOSDStr(short VideoChannel,char* pstr,short x,short y);
	void tshSetOSDTime(short VideoChannel,short x,short y);

	// 	* 功能：发送透明数据* pData 数据* nDataLen 数据长度 * nChnNum ---- 0 is 485,1 is 232
	int tshSendTransparentData(short VideoChannel,char* pData,int nLen,int nChnNum);
	int tshVerifyTime(short VideoChannel);// 功能：校时
private:
	BOOL tshStartWindowMoveHook();
	BOOL tshCloseWindowMoveHook();

//protected:
//// 	typedef int (*pFunc[TVChannelNum])(LPVOID pContext,HANDLE hIpCam,int nAct,
//// 									   DWORD dwParam1,DWORD dwParam2 );
//	//回调函数
//	static int  __stdcall  CommonIpcamCallback( 
//												LPVOID pContext,
//												HANDLE hIpCam,
//												int nAct,
//												DWORD dwParam1,
//												DWORD dwParam2 );
//	int IpCamCallback(HANDLE hIpCam,
//						int nAct,
//						DWORD dwParam1,
//						DWORD dwParam2 );
//

};

#endif