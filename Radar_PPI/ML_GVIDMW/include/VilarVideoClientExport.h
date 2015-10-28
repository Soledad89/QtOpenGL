#pragma once

#include "VilarEventHandler.h"
#include "DataRecver.h"
#include "StreamTypeDefine.h"
#include "ImageTypes.h"
#include "VilarCameraDefine.h"
#include "CameraCmdDefine.h"
#include "VilarOuterEventDefine.h"

extern "C"
{
	__declspec( dllexport ) HRESULT Initial();
	__declspec( dllexport ) HRESULT Free();
	__declspec( dllexport ) HRESULT AddCamera( long index, const _CAMERA_CONNECT_INFO * cameraInfo, int nChannelID, VilarEventHandler* eventHandler, bool isWaitforDataRecved );
	__declspec( dllexport ) HRESULT AddCameraEx( long index, const _CAMERA_CONNECT_INFO * cameraInfo, int nChannelID, VilarEventHandler* eventHandler, DataRecver * streamDataRecver, bool isWaitforDataRecved );
	__declspec( dllexport ) HRESULT ReconnectCamera( long index, bool isWaitforDataRecved );
	__declspec( dllexport ) HRESULT Play( long index, HWND hWnd, bool isShowRemoteOsd, bool isShowLocalOsd, bool isMute, DataRecver * recver );
	__declspec( dllexport ) HRESULT AddStream( long index );
	__declspec( dllexport ) HRESULT InputStreamData( long index, const char * buf, long size, bool isOnlyVideoData, STREAM_TYPE type, unsigned __int64 timestamp );
	__declspec( dllexport ) HRESULT RemoveItem( long index );
	__declspec( dllexport ) HRESULT ChangeItemIndex( long oldIndex, long newIndex );
	__declspec( dllexport ) HRESULT SwapItem( long index1, long index2 );
	__declspec( dllexport ) HRESULT SetShowWnd( long index, HWND showWnd );
	__declspec( dllexport ) HRESULT SetDecryptKey( long index, int keySize, const unsigned char *key );
	__declspec( dllexport ) HRESULT Stop( long index );
	__declspec( dllexport ) HRESULT CapPic( long index, const wchar_t * fileName, unsigned int imageType );
	__declspec( dllexport ) HRESULT Record( long index, const wchar_t * fileName );
	__declspec( dllexport ) HRESULT StopRecord( long index );
	__declspec( dllexport ) HRESULT ExecCmd( long index, long cmd );
	__declspec( dllexport ) HRESULT Zoom( long index, unsigned int x, unsigned int y, unsigned int zoomRate );
	__declspec( dllexport ) HRESULT ZoomRect( long index, unsigned int left, unsigned int top, unsigned int right, unsigned int bottom );
	__declspec( dllexport ) HRESULT GetLastValidDataTime( long index, unsigned long & time );
	__declspec( dllexport ) HRESULT GetVideoInfo( long index, _CAMERA_VIDEO_INFO & videoInfo );
	__declspec( dllexport ) HRESULT SetImageSharpness( long index, bool isSharpness, int adaptTH, int edgeStrength );
	__declspec( dllexport ) HRESULT Refresh( long index );
	__declspec( dllexport ) HRESULT SetLocalOsdInfo( long index, long osdIndex, const wchar_t * osd, LOGFONT font, int x, int y, unsigned char alpha, COLORREF color );
	__declspec( dllexport ) HRESULT ModifyLocalOsdText( long index, long osdIndex, const wchar_t * osd );
	__declspec( dllexport ) HRESULT RemoveLocalOsdInfo( long index, long osdIndex );
	__declspec( dllexport ) HRESULT ClearLocalOsdInfo( long index );
	__declspec( dllexport ) HRESULT ShowLocalOsd( long index );
	__declspec( dllexport ) HRESULT HideLocalOsd( long index );
	__declspec( dllexport ) HRESULT SetDrawRect( long index, long rectIndex, int x, int y, int width, int height, int lineWidth, COLORREF color );
	__declspec( dllexport ) HRESULT RemoveDrawRect( long index, long rectIndex );
	__declspec( dllexport ) HRESULT ClearDrawRect( long index );
	__declspec( dllexport ) HRESULT SetBufTime( long index, unsigned int time );
	__declspec( dllexport ) HRESULT OpenSound( long index );
	__declspec( dllexport ) HRESULT CloseSound( long index );
	__declspec( dllexport ) HRESULT SetVolume( long index, unsigned int volume );
	__declspec( dllexport ) HRESULT GetVolume( long index, unsigned int & volume );
	__declspec( dllexport ) HRESULT Resume( long index );
	__declspec( dllexport ) HRESULT SetBrightness( long index, int brightness );
	__declspec( dllexport ) HRESULT GetBrightness( long index, int & brightness );
	__declspec( dllexport ) HRESULT SetContrast( long index, int contrast );
	__declspec( dllexport ) HRESULT GetContrast( long index, int & contrast );
	__declspec( dllexport ) HRESULT SetHue( long index, int hue );
	__declspec( dllexport ) HRESULT GetHue( long index, int & hue );
	__declspec( dllexport ) HRESULT SetSaturation( long index, int saturation );
	__declspec( dllexport ) HRESULT GetSaturation( long index, int & saturation );
	__declspec( dllexport ) HRESULT SetRecvTimeout( long index, unsigned int timeout );
	__declspec( dllexport ) HRESULT SetConnectTimeout( long index, unsigned int timeout );
	__declspec( dllexport ) HRESULT SetFpsCountInterval( long index, unsigned int interval );
	__declspec( dllexport ) HRESULT RegisterEventHandler( long index, VilarEventHandler * eventHandler );
	__declspec( dllexport ) HRESULT GetReceivedFrameCount( long index, unsigned int & receivedFrameCount );
	__declspec( dllexport ) HRESULT GetCorrectFrameCount( long index, unsigned int & correctFrameCount );
	__declspec( dllexport ) HRESULT GetCameraFeature( long index, unsigned int & feature );
	__declspec( dllexport ) HRESULT SetVSyncState( long index, bool isEnabled );
}
