#ifndef _TVPLAYER_H
#define _TVPLAYER_H

#include <stdio.h>
#include <windows.h>
#include "ZDYT_SDK.h"


//������ʾ�ĵ���ͨ����
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

//��Ƶ

typedef struct USER_INFO
{
	char* pUrl;      // ����Ӳ����IP��ַ
	char* pUserName; // �û���
	char* pPwd;  // ����  
}UserInfo,pUserInfo;
 
class TVPlayer
{

private:

	char VideoChannel;
	int winth;

	TVcfg   tv_cfg[TVChannelNum];                        //״̬

	bool IsAudioPlay[TVChannelNum];

	UserInfo  userinfo[TVChannelNum];                 //�û���Ϣ

	CGraphicsManager* pGraphicsManager;               //��ʾ����

	BMPImageRec *od[2];

	//ZDYT_SDK *zs[TVChannelNum];

public:
	ZDYT_SDK *zs;

public:
	TVPlayer(CGraphicsManager* pGM, char videoChannel);
	~TVPlayer();

	void TVUpdate();
	
	//---------------------------------------���Ӷ���ӿ�-----------------------------------------------------------
	//�򿪵���
	int ML_GVID_OpenTv (short theDisplay,short VideoChannel ,short theX, short theY,short theWidth,short theHeight);
	//�رյ���
	int ML_GVID_CloseTv (short theDisplay,short VideoChannel );
	//���ô���λ��
	int ML_GVID_SetTvXY(short theDisplay,short VideoChannel ,short theX, short theY);
	//���ô��ڴ�С
	int ML_GVID_SetTvWH(short theDisplay,short VideoChannel ,short theWidth, short theHeight);
	//�������
	void ML_GVID_FrostTV(short theDisplay,short VideoChannel);
	//�ⶳ����
	void ML_GVID_UnFrostTV(short theDisplay,short VideoChannel);
	//�ı��������
	void ML_GVID_ChangTvBright(short theDisplay,short VideoChannel,int theBright);
	//�ı���ӶԱȶ�
	void ML_GVID_ChangTvContrast(short theDisplay,short VideoChannel,int theContrast);
	//��������ɫ��
	void ML_GVID_ChangTvColor(short theDisplay,short VideoChannel,int theColor);

	
	int TVsetColorKey(char winth,unsigned int colorKey,BMPImageRec* pBackMap1, int x, int y);
	//---------------------------------------------����---------------------------------------------------------------
	//��¼����Ƶ�ļ�
	void tshStartRecordAVFile(short VideoChannel,char *FileName = NULL);
	void tshCloseRecordAVFile(short VideoChannel);
	void tshPlayAudio(short VideoChannel);
	void tshCloseAudio(short VideoChannel);
	bool tshSetOSDInfo(short VideoChannel,LPBYTE pOSDData,RECT OSD_Rect,DWORD dwCOlorSpcae);
	void tshSetOSDStr(short VideoChannel,char* pstr,short x,short y);
	void tshSetOSDTime(short VideoChannel,short x,short y);

	// 	* ���ܣ�����͸������* pData ����* nDataLen ���ݳ��� * nChnNum ---- 0 is 485,1 is 232
	int tshSendTransparentData(short VideoChannel,char* pData,int nLen,int nChnNum);
	int tshVerifyTime(short VideoChannel);// ���ܣ�Уʱ
private:
	BOOL tshStartWindowMoveHook();
	BOOL tshCloseWindowMoveHook();

//protected:
//// 	typedef int (*pFunc[TVChannelNum])(LPVOID pContext,HANDLE hIpCam,int nAct,
//// 									   DWORD dwParam1,DWORD dwParam2 );
//	//�ص�����
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