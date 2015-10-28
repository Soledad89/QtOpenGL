//#include "StdAfx.h"
#include "tv.h"
#include <stddef.h>
#include <winsock.h>
#include "ZDYT_SDK0.h"
#include "ZDYT_SDK.h"
#include "CGraphicsManager.h"


//#pragma comment(lib,"ws2_32.lib")
//#pragma comment(lib,"IpCamSdk.lib")
//#pragma comment(lib,"HxPlayerSDK.lib")
//#pragma comment(lib,"GeVideoLib.lib")

#define VIDEO_WIDTH  704
#define VIDEO_HEIGHT 576


#define PIC_SIZE  704*576

//安装窗口移动钩子
/*static HHOOK WindowMoveHook=NULL; 

static HWND  hwnd_Window[TVChannelNum];
static HANDLE display_Window[TVChannelNum];
LRESULT CALLBACK  CallWndProc(int nCode,WPARAM wParam,LPARAM lParam);*/

TVPlayer::TVPlayer(CGraphicsManager* pGM, char videoChannel)
{		
	VideoChannel = videoChannel;
	//设置视频
	for(int i=0;i < TVChannelNum;i++)
	{
		tv_cfg[i].play_flag  = false;
		tv_cfg[i].Frost_flag = false;

	}
	//创建本地音频播放器
	pGraphicsManager = pGM;
	
	od[0] = new BMPImageRec;

	od[1] = new BMPImageRec;

		zs=new ZDYT_SDK(pGraphicsManager,od[0],od[1]);

}
TVPlayer::~TVPlayer()
{
	zs->DllFreeFunc();
	delete(zs);

}
/*BOOL TVPlayer::tshStartWindowMoveHook()
{
	WindowMoveHook = ::SetWindowsHookEx(WH_CALLWNDPROC,CallWndProc,NULL,::GetCurrentThreadId());
	if(WindowMoveHook != NULL) return  TRUE;
	return FALSE;
}
//卸载钩子
BOOL TVPlayer::tshCloseWindowMoveHook() 
{
	if(WindowMoveHook) return ::UnhookWindowsHookEx(WindowMoveHook);
	else 
		return FALSE;
}*/


int TVPlayer::ML_GVID_OpenTv (short theDisplay,short VideoChannel ,short theX, short theY,short theWidth,short theHeight)
{
	RECT rect;
	if(!tv_cfg[VideoChannel - 1].play_flag)
	{
		tv_cfg[VideoChannel - 1].play_flag=TRUE;

		tv_cfg[VideoChannel - 1].startX = theX;
		tv_cfg[VideoChannel - 1].startY = theY;
		tv_cfg[VideoChannel - 1].Width = theWidth;
		tv_cfg[VideoChannel - 1].Height = theHeight;
		
		rect.left =  theX;
		rect.top = theY;
		rect.right = theWidth+rect.left;
		rect.bottom = theHeight+rect.top;

//		unFrost = TRUE;
		
//		zs=new ZDYT_SDK(pGraphicsManager, VideoChannel - 1);
		zs->play(VideoChannel - 1);

		return 0;
	}
	return -1;
}
//关闭电视
int TVPlayer::ML_GVID_CloseTv (short theDisplay,short VideoChannel )
{

	zs ->DllStopFunc(VideoChannel - 1);
//	zs ->DllFreeFunc();

	if(tv_cfg[VideoChannel - 1].play_flag == TRUE)
	{
		tv_cfg[VideoChannel - 1].play_flag=FALSE;


		return 0;
	}
	return 0;
}

//设置窗口位置
int TVPlayer::ML_GVID_SetTvXY(short theDisplay,short VideoChannel ,short theX, short theY)
{ 
		if(tv_cfg[VideoChannel - 1].play_flag==TRUE)
			{
				tv_cfg[VideoChannel - 1].startX = theX;
				tv_cfg[VideoChannel - 1].startY = theY;

				//rect.left = theX;
				//rect.right = tv_cfg[VideoChannel].Width+theX;

				//rect.top = theY;
				//rect.bottom = tv_cfg[VideoChannel].Height+theY;
				//gevdMoveVideo(video_cfg[VideoChannel].m_hDisplay,rect);
				return 0;
			}
				return -1;
}
//设置窗口大小
int TVPlayer::ML_GVID_SetTvWH(short theDisplay,short VideoChannel ,short theWidth, short theHeight)
{
		if(tv_cfg[VideoChannel - 1].play_flag==TRUE)
		{	
				tv_cfg[VideoChannel - 1].Width = theWidth;
				tv_cfg[VideoChannel - 1].Height = theHeight;

				//rect.left = tv_cfg[VideoChannel].startX ;
				//rect.right =theWidth+rect.left;
				//
				//rect.top = tv_cfg[VideoChannel].startY ;
				//rect.bottom = theHeight+rect.top;
				//gevdMoveVideo(video_cfg[VideoChannel].m_hDisplay,rect);
				return 0;
			}
			return -1;
}
//冻结电视
void TVPlayer::ML_GVID_FrostTV(short theDisplay,short VideoChannel)
{
	if((tv_cfg[VideoChannel - 1].play_flag==TRUE))			
	{
		zs->TVDataRecver[VideoChannel - 1]->unFrost = FALSE;
	}
}
//解冻电视
void TVPlayer::ML_GVID_UnFrostTV(short theDisplay,short VideoChannel)
{
	if(tv_cfg[VideoChannel - 1].play_flag)				
	{
		zs->TVDataRecver[VideoChannel - 1]->unFrost =TRUE;
	}
}
//改变电视亮度
void TVPlayer::ML_GVID_ChangTvBright(short theDisplay,short VideoChannel,int theBright)
{
	if(tv_cfg[VideoChannel - 1].play_flag==TRUE)			
	{
		zs->DLLSetBrightnessFunc(VideoChannel - 1,theBright );
	}
	
}
//改变电视对比度
void TVPlayer::ML_GVID_ChangTvContrast(short theDisplay,short VideoChannel,int theContrast)
{	
	if(tv_cfg[VideoChannel - 1].play_flag==TRUE)			
	{
		zs->DLLSetContrastFunc(VideoChannel - 1,theContrast );
	}
	

}
//调整电视色彩
void TVPlayer::ML_GVID_ChangTvColor(short theDisplay,short VideoChannel,int theColor)
{	
	if(tv_cfg[VideoChannel - 1].play_flag==TRUE)			
	{
		zs->DLLSetSaturationFunc(VideoChannel - 1,theColor );
	}
	

}


int TVPlayer::TVsetColorKey(char winth,unsigned int colorKey,BMPImageRec* pBackMap1, int x, int y)
{
	pGraphicsManager -> setColorKey(winth, colorKey, pBackMap1,x,y);
	return 1;
}

void TVPlayer::TVUpdate()
{
	/*for(int i = 0; i < 2; i ++)
	{
		if(zs ->TVDataRecver[i]->unFrost)
		{
			pGraphicsManager -> drawBackMap(i+7, zs->TVDataRecver[i]->od);
			delete(zs->TVDataRecver[i]->od->data);
			pGraphicsManager -> copyTexFBO(i+7, 3, i+7, 2);
			pGraphicsManager -> copyTexFBO(i+7, 3, i+7, 1);
			pGraphicsManager -> copyTex(i+7, i + 7 , 3);
			pGraphicsManager -> update(i+7);
		}
	}*/
}


//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

/*
bool TVPlayer::tshSetOSDInfo(short VideoChannel,LPBYTE pOSDData,RECT OSD_Rect,DWORD dwCOlorSpcae)
{
	if((tv_cfg[VideoChannel].play_flag==TRUE)&&(tv_cfg[VideoChannel].Frost_flag==FALSE))
	{
		gevdSetOSDInfo(video_cfg[VideoChannel].m_hDisplay,pOSDData,OSD_Rect,dwCOlorSpcae);
		return true;
	}
	return false;
}
void TVPlayer::tshSetOSDStr(short VideoChannel,char* pstr,short x,short y)
{
	CQ_OSD_INFO stuOsdInfo;
	memset(&stuOsdInfo,0,sizeof(stuOsdInfo));

	stuOsdInfo.uValidArea = CQ_OSDVALID_CHARDATA|CQ_OSDVALID_OSDPOS|CQ_OSDVALID_STATE;

	stuOsdInfo.uOSDState = CQ_OSD_CHAR;
	strncpy(stuOsdInfo.szCharData,pstr,32);
	stuOsdInfo.sOSDPosX = x;
	stuOsdInfo.sOSDPosY = y;

	cxSetOSDInfo(video_cfg[VideoChannel].m_hCarama,(LPBYTE)&stuOsdInfo,offsetof(CQ_OSD_INFO,pCharMask));
}
void TVPlayer::tshSetOSDTime(short VideoChannel,short x,short y)
{
	CQ_OSD_INFO stuOsdInfo;
	memset(&stuOsdInfo,0,sizeof(stuOsdInfo));
	
	stuOsdInfo.uValidArea = CQ_OSDVALID_TIMEPOS|CQ_OSDVALID_STATE;

	stuOsdInfo.uOSDState = CQ_OSD_TIME;
	stuOsdInfo.sTimePosX = x;
	stuOsdInfo.sTimePosY = y;
	
	cxSetOSDInfo(video_cfg[VideoChannel].m_hCarama,(LPBYTE)&stuOsdInfo,offsetof(CQ_OSD_INFO,pCharMask));
}
int TVPlayer::tshVerifyTime(short VideoChannel)
{
	return cxVerifyTime(video_cfg[VideoChannel].m_hCarama);
}
void TVPlayer::tshStartRecordAVFile(short VideoChannel,char *FileName)
{
	if(!FileName) 
	{
		char str[25];
		sprintf(str,"AVRecordFile_%d.mpg",VideoChannel);
		FileName = str;
	}
	AVRecordFile[VideoChannel] = cxCreateAVFile(FileName,VIDEO_WIDTH,VIDEO_HEIGHT,0);
}

void TVPlayer::tshCloseRecordAVFile(short VideoChannel)
{
	//关闭录像文件
	if(AVRecordFile[VideoChannel] != NULL)
	{
		cxCloseAVFile(AVRecordFile[VideoChannel]);
		AVRecordFile[VideoChannel] = NULL;
	}
}
int  TVPlayer::tshSendTransparentData(short VideoChannel,char* pData,int nLen,int nChnNum)
{
	return cxSendTransparentData(video_cfg[VideoChannel].m_hCarama,pData,nLen,nChnNum);
}
void TVPlayer::tshPlayAudio(short VideoChannel)
{
	IsAudioPlay[VideoChannel] = true;
}
void TVPlayer::tshCloseAudio(short VideoChannel)
{
	IsAudioPlay[VideoChannel] = false;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int  TVPlayer::CommonIpcamCallback( LPVOID pContext,
						HANDLE hIpCam,
						int nAct,
						DWORD dwParam1,
						DWORD dwParam2 )
{
	return ((TVPlayer*)pContext)->IpCamCallback(hIpCam,nAct,dwParam1,dwParam2);
}

int TVPlayer::IpCamCallback(HANDLE hIpCam,
						int nAct,
						DWORD dwParam1,
						DWORD dwParam2 )
{
	int VideoChannel = (hIpCam == video_cfg[0].m_hCarama) ? 0 : 1;
	switch(nAct)
	{

	case ACT_CONNECT_FAILED:
		{
			//TRACE("Connect Fail\n");
		}
		break;

	case ACT_CONNECT_OK:
		{
			cxStartRecvVideo(hIpCam,0,0);//本地开始接收视频
			cxStartVideo(hIpCam,0,0);//远端开始发送视频
			
			cxStartRecvAudio(hIpCam,0,0);//本地开始接收音频
			cxStartAudio(hIpCam,0,0);//远端开始发送音频

			cxForceIFrame(hIpCam);
			//获得开关量信息
			//cxGetAlarmIn(hIpCam);
			//::MessageBox(NULL,"ok","OK!",NULL);
		}
		break;
	case ACT_RECV_VIDEO_FRAME:
		{
			MEDIA_UNIT* pVideoUnit = (MEDIA_UNIT*)dwParam1;
			//解码一帧图像
			cxDecodeFrame(video_cfg[VideoChannel].m_hDecoder,pVideoUnit->pBuf,pVideoUnit->nLength,(char*)video_cfg[VideoChannel].m_pDecoderBuffer,-1,0);		
			//显示一帧图像	
			gevdPushVideo(video_cfg[VideoChannel].m_hDisplay,video_cfg[VideoChannel].m_pDecoderBuffer,DecodeBufferLen,MAKEFOURCC('I','4','2','0'));
			//视频帧存盘
			if(AVRecordFile[VideoChannel]!= NULL)
			{
				cxWriteVideo(AVRecordFile[VideoChannel],pVideoUnit);
			}
		}
		break;
	case ACT_RECV_AUDIO_FRAME:
		{
			MEDIA_UNIT* pAudioUnit = (MEDIA_UNIT*)dwParam1;
			//音频帧存盘
			if(AVRecordFile[VideoChannel]!= NULL)
			{
				cxWriteAudio(AVRecordFile[VideoChannel],pAudioUnit);
			}
			if(IsAudioPlay[VideoChannel])//播放声音
			{
				cxPlayAudio(m_LocalAudioPlayer,(short*)pAudioUnit->pBuf,pAudioUnit->nLength/2,pAudioUnit->dwTimeStamp);
			}
		}
		break;
		case ACT_ALARM_IN: //告警信息处理
			break;
	}

	return 0;
}

LRESULT CALLBACK  CallWndProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	
	if (nCode < 0)  // do not process message 
		return CallNextHookEx(WindowMoveHook, nCode, wParam, lParam); 
	
	PCWPSTRUCT ptr = (PCWPSTRUCT)lParam;
	RECT rect;
	
	switch (nCode)
	{
	case HC_ACTION:			
		if(ptr->message == WM_MOVE||ptr->message == WM_SIZE)
		{
			if(ptr->hwnd == hwnd_Window[0])
			{	
				::GetClientRect(hwnd_Window[0],&rect);
				::ClientToScreen(hwnd_Window[0],(LPPOINT) &rect);
				::ClientToScreen(hwnd_Window[0],(LPPOINT)&rect+1);
				gevdMoveVideo(display_Window[0],rect);
			}
			else if(ptr->hwnd == hwnd_Window[1])
			{
				::GetClientRect(hwnd_Window[1],&rect);
				::ClientToScreen(hwnd_Window[1],(LPPOINT) &rect);
				::ClientToScreen(hwnd_Window[1],(LPPOINT) &rect+1);
				gevdMoveVideo(display_Window[1],rect);	
			}
		}
		break;
	default:
		break;
	}
	
    return CallNextHookEx(WindowMoveHook, nCode, wParam,lParam); 
}

*/

