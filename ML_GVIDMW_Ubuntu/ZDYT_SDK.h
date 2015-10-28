#pragma once

#include "VilarEventHandler.h"
#include "VilarVideoClientExport.h"
#include "CGraphicsManager.h"
#include "bmp.h"
#include "NewDataRecver.h"

typedef HRESULT(*InitFunc)();
typedef HRESULT(*FreeFunc)();
typedef HRESULT(*AddCameraFunc)(long, const _CAMERA_CONNECT_INFO *, long, VilarEventHandler*, bool);
typedef HRESULT(*AddCameraExFunc)(long, const _CAMERA_CONNECT_INFO *, long, VilarEventHandler*, DataRecver*, bool);
typedef HRESULT(*RemoveItemFunc)(long);
typedef HRESULT(*PlayFunc)(long, HWND, bool, bool, bool, DataRecver *);
typedef HRESULT(*StopFunc)(long);
typedef HRESULT(*SetShowWndFunc)(long, HWND);
typedef HRESULT(*CapPicFunc)(long, const wchar_t*, unsigned int);
typedef HRESULT(*SetDecryptKeyFunc)(long, int, const unsigned char *);
typedef HRESULT(*SetXvidDecodeQualityFunc)(long, int);
typedef HRESULT(*GetXvidDecodeQualityFunc)(long, int&);
typedef HRESULT(*ZoomFunc)(long, unsigned int, unsigned int, unsigned int);
typedef HRESULT(*GetVideoInfoFunc)(long, _CAMERA_VIDEO_INFO&);
typedef HRESULT(*SetImageSharpnessFunc)(long, bool, int, int);
typedef HRESULT(*RecordFunc)(long, const wchar_t*);
typedef HRESULT(*StopRecordFunc)(long);
typedef HRESULT(*SetLocalOsdInfoFunc)(long, long, const wchar_t *, int, int, unsigned char, int, COLORREF);
typedef HRESULT(*ShowLocalOsdFunc)(long);
typedef HRESULT(*HideLocalOsdFunc)(long);
typedef HRESULT(*SetBufTimeFunc)(long, unsigned int); 
typedef HRESULT(*ReconnectCameraFunc)(long, bool); 
typedef HRESULT(*SetConnectTimeoutFunc)(long, unsigned int); 
typedef HRESULT(*SetRecvTimeoutFunc)(long, unsigned int); 
typedef HRESULT (*ExecCmdFunc)(long, long);
typedef HRESULT (*ExecCmdExFunc)(long, long, void*, int);
typedef HRESULT(*SetBrightnessFunc)(long, int); 
typedef HRESULT (*SetContrastFunc)(long, int);
typedef HRESULT (*SetSaturationFunc)(long, int);

/*typedef struct __OUTDATAINFO                            //BMPmap等同
{
	 unsigned int nWidth;
	 unsigned int nHeight;
	  unsigned char* pBuffer;
}OUTDATAINFO;*/


typedef void (*RecvDataCallBack)(BMPImageRec);                  //回调函数的意义？
class ZDYT_SDK
{
//public:
protected:
	HINSTANCE m_hDll;
public:
	BMPImageRec *od[2];
	ZDYT_SDK(CGraphicsManager* pGM,BMPImageRec *bmp1,BMPImageRec *bmp2);
	~ZDYT_SDK(void);
	CGraphicsManager* pGraphicsManager;
	//接收数据帧回调函数
	RecvDataCallBack RecRGBCallBack;

	void play(char ind);
	SetBrightnessFunc DLLSetBrightnessFunc; 
	SetContrastFunc DLLSetContrastFunc;
	SetSaturationFunc DLLSetSaturationFunc;
	FreeFunc DllFreeFunc ;
	StopFunc DllStopFunc ;
//private:
//	virtual void DoInputData(const char * buf, unsigned int size );//这里面执行需要做的工作
//	virtual void __stdcall DoDealWithEvent(int notifyEvent, void * param, bool isBlockEvent);
	NewDataRecver* TVDataRecver[2];

	InitFunc DllInitFunc;
//	FreeFunc DllFreeFunc ;
	AddCameraFunc DllAddCameraFunc ;
	AddCameraExFunc DllAddCameraExFunc ;
	RemoveItemFunc DllRemoveCameraFunc ;
	PlayFunc DllPlayFunc ;
//	StopFunc DllStopFunc ;
	SetShowWndFunc DllSetShowWndFunc ;
	CapPicFunc DllCapPicFunc ;
	SetDecryptKeyFunc DllSetDecryptKeyFunc ;
	ZoomFunc DllZoomFunc ;
	GetVideoInfoFunc DllGetVideoInfoFunc ;
	SetImageSharpnessFunc DllSetImageSharpnessFunc ;
	RecordFunc DllRecordFunc ;
	StopRecordFunc DllStopRecordFunc ;
	SetLocalOsdInfoFunc DllSetLocalOsdInfoFunc ;
	ShowLocalOsdFunc DllShowLocalOsdFunc ;
	HideLocalOsdFunc DllHideLocalOsdFunc ;
	SetBufTimeFunc DllSetBufTimeFunc ;
	ReconnectCameraFunc DllReconnectCameraFunc ;
	SetConnectTimeoutFunc DllSetConnectTimeoutFunc ;
	SetRecvTimeoutFunc DllSetRecvTimeoutFunc ;
	ExecCmdFunc DllExecCmdFunc ;
	ExecCmdExFunc DllExecCmdExFunc ;

	_CAMERA_CONNECT_INFO camera;
	
};
