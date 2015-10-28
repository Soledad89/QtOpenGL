#include "StdAfx.h"
#include "ZDYT_SDK.h"
#include "FrameInfoDefine.h"
#include "yuvrgb24.h"
#include "bmp.h"
//#include "CGraphicsManager.h"

HINSTANCE m_hDll;
//bool unFrost0;

ZDYT_SDK::ZDYT_SDK(CGraphicsManager* pGM,BMPImageRec *bmp1,BMPImageRec *bmp2)
{
	pGraphicsManager = pGM;
	od[0] = bmp1;
	od[1] = bmp2;


	init_dither_tab();
		m_hDll = LoadLibrary(_T("VilarClientCoreDll.dll"));
		if (NULL == m_hDll)
		{
			int i = GetLastError();//���׳��쳣
			printf("error");
		}
	
	DllInitFunc = (InitFunc)GetProcAddress(m_hDll, "Initial");
	DllFreeFunc = (FreeFunc)GetProcAddress(m_hDll, "Free");
	DllAddCameraFunc = (AddCameraFunc)GetProcAddress(m_hDll, "AddCamera");
	DllAddCameraExFunc = (AddCameraExFunc)GetProcAddress(m_hDll, "AddCameraEx");
	DllPlayFunc = (PlayFunc)GetProcAddress(m_hDll, "Play");
	DllStopFunc = (StopFunc)GetProcAddress(m_hDll, "Stop");
	DllSetShowWndFunc = (SetShowWndFunc)GetProcAddress(m_hDll, "SetShowWnd");
	DllRemoveCameraFunc = (RemoveItemFunc)GetProcAddress(m_hDll, "RemoveItem");
	DllCapPicFunc = (CapPicFunc)GetProcAddress(m_hDll, "CapPic");
	DllSetDecryptKeyFunc = (SetDecryptKeyFunc)GetProcAddress(m_hDll, "SetDecryptKey");
	DllZoomFunc = (ZoomFunc)GetProcAddress(m_hDll, "Zoom");
	DllGetVideoInfoFunc = (GetVideoInfoFunc)GetProcAddress(m_hDll, "GetVideoInfo");
	DllSetImageSharpnessFunc = (SetImageSharpnessFunc)GetProcAddress(m_hDll, "SetImageSharpness");
	DllRecordFunc = (RecordFunc)GetProcAddress(m_hDll, "Record");
	DllStopRecordFunc = (StopRecordFunc)GetProcAddress(m_hDll, "StopRecord");
	DllSetLocalOsdInfoFunc = (SetLocalOsdInfoFunc)GetProcAddress(m_hDll, "SetLocalOsdInfo");
	DllShowLocalOsdFunc = (ShowLocalOsdFunc)GetProcAddress(m_hDll, "ShowLocalOsd");
	DllHideLocalOsdFunc = (HideLocalOsdFunc)GetProcAddress(m_hDll, "HideLocalOsd");
	DllSetBufTimeFunc = (SetBufTimeFunc)GetProcAddress(m_hDll, "SetBufTime");
	DllReconnectCameraFunc = (ReconnectCameraFunc)GetProcAddress(m_hDll, "ReconnectCamera");
	DllSetConnectTimeoutFunc = (SetConnectTimeoutFunc)GetProcAddress(m_hDll, "SetConnectTimeout");
	DllSetRecvTimeoutFunc = (SetRecvTimeoutFunc)GetProcAddress(m_hDll, "SetRecvTimeout");
	DllExecCmdFunc = (ExecCmdFunc)GetProcAddress(m_hDll, "ExecCmd");
	DllExecCmdExFunc = (ExecCmdExFunc)GetProcAddress(m_hDll, "ExecCmdEx");
	DLLSetBrightnessFunc = (SetBrightnessFunc)GetProcAddress(m_hDll, "SetBrightness");
	DLLSetContrastFunc = (SetContrastFunc)GetProcAddress(m_hDll, "SetContrast");
	DLLSetSaturationFunc = (SetSaturationFunc)GetProcAddress(m_hDll, "SetSaturation");


	HRESULT hr = DllInitFunc();
//	_CAMERA_CONNECT_INFO camera;
	memset(&camera, 0, sizeof(_CAMERA_CONNECT_INFO));
	camera.port = 37777;
	char * ss="192.168.1.234";
	memccpy( camera.url,ss,0,strlen(ss));//Ӳ��IP��ַ
	ss="admin";
	memccpy( camera.name,ss,0,strlen(ss));//Ӳ���û���
	ss="admin";
	memccpy( camera.pwd,ss,0,strlen(ss));//Ӳ������
	/*WideCharToMultiByte(CP_ACP, NULL, (LPCTSTR)"192.168.1.234", -1, camera.url, 1024, NULL, NULL);
	WideCharToMultiByte(CP_ACP, NULL, (LPCTSTR)"admin", -1, camera.name, 48, NULL, NULL);
	WideCharToMultiByte(CP_ACP, NULL, (LPCTSTR)"123456", -1, camera.pwd, 48, NULL, NULL);*/
	_HTTP_ADDITIONAL_INFO additionalInfo;
	additionalInfo.type = "Digest";
	additionalInfo.text = "";
	camera.additionalInfo = &additionalInfo;

	TVDataRecver[0] = new NewDataRecver(pGM,0,od[0]);

	TVDataRecver[1] = new NewDataRecver(pGM,1,od[1]);

	//DllRemoveCameraFunc(g_nIndex);
	////�������ͷ
	//hr = DllAddCameraExFunc(g_nIndex, &camera,g_nIndex, NULL, NULL, false);
	//if (S_OK == hr)
	//{
	//	DllSetConnectTimeoutFunc(g_nIndex, 30000);
	//}

	//int a = -1;
	//	DllPlayFunc(g_nIndex, (HWND)a, false, true, false, (DataRecver *)TVDataRecver[g_nIndex]);//��ʼ����
	//	DllSetBufTimeFunc(g_nIndex, 0);//���û����¼�ms
}

void ZDYT_SDK::play(char g_nIndex)
{
	DllRemoveCameraFunc(g_nIndex);
	//�������ͷ
	HRESULT hr = DllAddCameraExFunc(g_nIndex, &camera,g_nIndex, NULL, NULL, false);
	if (S_OK == hr)
	{
		DllSetConnectTimeoutFunc(g_nIndex, 30000);
	}

	int a = -1;
		DllPlayFunc(g_nIndex, (HWND)a, false,false, false, (DataRecver *)TVDataRecver[g_nIndex]);//��ʼ����
		DllSetBufTimeFunc(g_nIndex, 0);//���û����¼�ms
}



ZDYT_SDK::~ZDYT_SDK(void)
{
	DllFreeFunc();
}

//void ZDYT_SDK::DoInputData(const char * buf, unsigned int size )
//{
//	const char * p = buf;
//		
//	unsigned int len = size;	
//	FRAME_INFO_VIMICRO* frameInfo = (FRAME_INFO_VIMICRO*)p;	
//	long type = frameInfo->nType;	
//	unsigned long long nTimestamp = frameInfo->nTimestamp;	
//	long nFrameRate = frameInfo->nFrameRate;	
//	unsigned int nFrameId = frameInfo->nFrameId;	
//	long nWidth = frameInfo->nWidth;	
//	long nHeight = frameInfo->nHeight;
//	char* pBuffer = frameInfo->pBuffer;	// ͼ�����ݵ�ַ	
//	long nSize = frameInfo->nSize;		// ͼ�����ݳ���	
//
//	
//	if(RecRGBCallBack!=NULL)//�ж��Ƿ��лص�����
//	{
//		unsigned char * BGRBuffer=new unsigned char [nWidth*nHeight*3];//����BGRͼ���ڴ�
//		//ͼ��ת��ΪRGB
//		//YUV420toRGB((unsigned char *)pBuffer,BGRBuffer,nWidth,nHeight,true);//ͼ�������·�ת
//		//ͼ��ת��ΪBGR
//		YUV420toRGB((unsigned char *)pBuffer,BGRBuffer,nWidth,nHeight,true);//ͼ�������·�ת
//		BMPImageRec *od = new BMPImageRec;
//		od->sizeX=(GLuint)nWidth;
//		od->sizeY=(GLuint)nHeight;
//		od->data=(GLubyte *)BGRBuffer;
//		if (unFrost0)
//		{
//			pGraphicsManager -> drawBackMap(g_nIndex+7, od);
//			pGraphicsManager -> copyTexFBO(g_nIndex+7, 3, g_nIndex+7, 2);
//			pGraphicsManager -> copyTex(g_nIndex+7, g_nIndex+7, 3);
//			pGraphicsManager -> update(g_nIndex+7);
//		}
//		delete (od);
//		delete(BGRBuffer);//����RGB�ڴ�
////		RecRGBCallBack( *od);//ִ�лص�����
//
//	}	
//	//ת���������ɶ������ڴ˴����
//}

//void ZDYT_SDK::DoDealWithEvent(int notifyEvent, void * param, bool isBlockEvent)
//{
//	VILAR_EVENT_PARAM * eventParam = (VILAR_EVENT_PARAM *)param;
//	if (notifyEvent == CONNECTION_ESTABLISHED)
//	{
//		int a = -1;
//		DllPlayFunc(g_nIndex, (HWND)a, false, true, false, this);//��ʼ����
//		DllSetBufTimeFunc(g_nIndex, 0);//���û����¼�ms
//	//	DllExecCmdFunc(g_nIndex, CMD_SET_STREAM_TYPE_JPEG);
//	}
//	else if (notifyEvent == CONNECTION_CLOSED)
//	{
//		//���ӶϿ�
//	}
//	else if (notifyEvent == RESOLUTION_CHANGED)
//	{
//	}
//	else if (notifyEvent == NEED_DECRYPT_KEY)
//	{
//
//	}
//	else if (notifyEvent == FRAME_RENDERED)
//	{
//	}
//}