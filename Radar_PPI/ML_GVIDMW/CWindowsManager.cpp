// Name:   CWindowsManager.cpp
// Author: Yan Guo
// Date:   Feb 27, 2013 in the canteen of Rohm Building


#include <windows.h>

#include "CWindowsManager.h"

extern BMPImageRec* pBackMap;


static LPCSTR winclsName[WINDOW_NUM] = {"PPI_1 Wincls", "MINI_1 Wincls", "AR_1 Wincls", 
                              "PPI_2 Wincls", "MINI_2 Wincls", "AR_2 Wincls", 
							  "TV_1 Wincls", "TV_2 Wincls"};

static LPCSTR windowName[WINDOW_NUM] = {"PPI_1 Window", "MINI_1 Window", "AR_1 Window", 
                              "PPI_2 Window", "MINI_2 Window", "AR_2 Window", 
							  "TV_1 Window", "TV_2 Window"};

int winPosX[WINDOW_NUM] =   {0,   400, 400, 700, 1100, 1100, 0,  700};
int winPosY[WINDOW_NUM] =   {0,   0,   200, 0,   0,    200, 400, 400};
int winWidth[WINDOW_NUM] =  {400, 200, 200, 400, 200,  200, 600, 600};
int winHeight[WINDOW_NUM] = {400, 200, 200, 400, 200,  200, 300, 300};


LRESULT CALLBACK WinProc_PPI1( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_MINI1( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_AR1( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_PPI2( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_MINI2( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_AR2( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_TV1( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc_TV2( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CWindowsManager::CWindowsManager()
{
	for(int i = 0; i < WINDOW_NUM; i ++)
	{
		isOpen[i] = false;
	}
	
	wndcls[0].lpfnWndProc = WinProc_PPI1;        // ������Ĵ��ڹ��̺���
	wndcls[1].lpfnWndProc = WinProc_MINI1;
	wndcls[2].lpfnWndProc = WinProc_AR1;
	wndcls[3].lpfnWndProc = WinProc_PPI2;
	wndcls[4].lpfnWndProc = WinProc_MINI2;
	wndcls[5].lpfnWndProc = WinProc_AR2;
	wndcls[6].lpfnWndProc = WinProc_TV1;
	wndcls[7].lpfnWndProc = WinProc_TV2;

	for(int i = 1; i <= WINDOW_NUM; i ++)
	{
		createWindow(i);
		hDC[i-1] = GetDC(hWnd[i-1]);                 // A window has only one DC.
		setDCPixelFormat(hDC[i-1]);            // The pixel format must be set before the RC is created.
	}
}



// �������ܣ� ��ȡ��winth�����ڵĿ���
int CWindowsManager::getWH(int winth, int* pWidth, int* pHeight)
{
	*pWidth = winWidth[winth];
	*pHeight = winHeight[winth];

	return 0;
}


//�������ܣ���ȡ���ڵ�HWND
HWND* CWindowsManager::getHWND()
{
	return hWnd;
}


//�������ܣ���ȡ���ڵ�DC
HDC* CWindowsManager::getHDC()
{
	return hDC;
}


//�������ܣ�������winth������
int CWindowsManager::createWindow(int winth)
{

		//Step1: Design PPI Window Class

		wndcls[winth - 1].cbClsExtra = 0;        // ������׷���ڴ�
		wndcls[winth - 1].cbWndExtra = 0;        // ����׷���ڴ�
		wndcls[winth - 1].hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);      // ������ˢ���
		wndcls[winth - 1].hCursor = LoadCursor(NULL, IDC_CROSS);               // �����
		wndcls[winth - 1].hIcon = LoadIcon(NULL, IDI_ERROR);                   // ͼ����
		wndcls[winth - 1].hInstance = NULL;                                    // ʵ�����
		wndcls[winth - 1].lpszClassName = winclsName[winth - 1];                                 // ��������
		wndcls[winth - 1].lpszMenuName = NULL;                                      // ->�˵���Դ��
		wndcls[winth - 1].style = CS_HREDRAW | CS_VREDRAW;                          // ����ʽ��

		RegisterClass(&wndcls[winth - 1]);         //Step2: Register Window Class
	
		hWnd[winth - 1] =  CreateWindow(           //Step3: Create Window	
			winclsName[winth - 1], 
			(windowName[winth - 1]), 
			WS_OVERLAPPEDWINDOW, 
			winPosX[winth - 1],          // ����ˮƽλ��
			winPosY[winth - 1],          // ���ڴ�ֱλ��
			winWidth[winth - 1],        // ���ڿ��
			winHeight[winth - 1],        // ���ڸ߶�
			NULL,       // -> �����ھ��
			NULL,       // -> �˵����Ӵ��ڱ�ʾ��
			NULL,  // -> Ӧ�ó���ʵ��
			NULL        // -> ���ڴ�������
			);

	return 0;
}


//�������ܣ�Ϊ�������ڵ�DC�����ض������ظ�ʽ
int CWindowsManager::setDCPixelFormat(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd  
		1,                     // version number  
       PFD_DRAW_TO_WINDOW |   // support window  
       PFD_SUPPORT_OPENGL, 
       PFD_TYPE_RGBA,         // RGBA type  
       24,                    // 24-bit color depth  
       0, 0, 0, 0, 0, 0,      // color bits ignored  
       0,                     // no alpha buffer  
       0,                     // shift bit ignored  
       0,                     // no accumulation buffer  
       0, 0, 0, 0,            // accum bits ignored  
       32,                    // 32-bit z-buffer  
       0,                     // no stencil buffer  
       0,                     // no auxiliary buffer  
       PFD_MAIN_PLANE,        // main layer  
       0,                     // reserved  
       0, 0, 0                // layer masks ignored  
	}; 


	//static PIXELFORMATDESCRIPTOR pfd = 
	//{
	//	sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd  
	//	1,                     // version number  
 //      PFD_DRAW_TO_WINDOW |   // support window  
 //      PFD_SUPPORT_OPENGL |   // support OpenGL  
 //      PFD_DOUBLEBUFFER,      // double buffered  
 //      PFD_TYPE_RGBA,         // RGBA type  
 //      24,                    // 24-bit color depth  
 //      0, 0, 0, 0, 0, 0,      // color bits ignored  
 //      0,                     // no alpha buffer  
 //      0,                     // shift bit ignored  
 //      0,                     // no accumulation buffer  
 //      0, 0, 0, 0,            // accum bits ignored  
 //      32,                    // 32-bit z-buffer  
 //      0,                     // no stencil buffer  
 //      0,                     // no auxiliary buffer  
 //      PFD_MAIN_PLANE,        // main layer  
 //      0,                     // reserved  
 //      0, 0, 0                // layer masks ignored  
	//}; 


   int iPixelFormat = ChoosePixelFormat(hDC, &pfd); 
 
   SetPixelFormat(hDC, iPixelFormat, &pfd);

   return 0;
}



// �������ܣ��򿪵�winth�����ڣ� winth: 1 ~ 8
int CWindowsManager::openWindow(int winth)
{
	if ( (winth <= 0) || (winth > WINDOW_NUM) ) 
	{
		return -1;
	}

	if(!isOpen[winth - 1])
	{
		ShowWindow(hWnd[winth - 1], SW_SHOWNORMAL);
		UpdateWindow(hWnd[winth - 1]);

		isOpen[winth - 1] = true;
	}

	return 0;
}


// �������ܣ��رյ�winth�����ڣ� winth: 1 ~ 8
int CWindowsManager::closeWindow(int winth)
{
	if ( (winth <= 0) || (winth > WINDOW_NUM) ) 
	{
		return -1;
	}

	if(isOpen[winth - 1])
	{
		ShowWindow(hWnd[winth - 1], SW_HIDE);
		isOpen[winth - 1] = false;
	}

	return 0;
}



//�������ܣ����õ�winth�����ڵ�λ��
int CWindowsManager::setWindowXY(int winth, short theX ,short theY)
{
	if ( (winth <= 0) || (winth > WINDOW_NUM) ) 
	{
		return -1;
	}

	winPosX[winth - 1] = theX;
	winPosY[winth - 1] = theY;

	SetWindowPos(hWnd[winth - 1], HWND_TOP, theX, theY, 0, 0, SWP_NOSIZE);
	UpdateWindow(hWnd[winth - 1]);

	return 0;
}


//�������ܣ����õ�winth�����ڵĴ�С
int CWindowsManager::setWindowWH(int winth, short theWidth, short theHeight)
{
	if ( (winth <= 0) || (winth > WINDOW_NUM) ) 
	{
		return -1;
	}

	winWidth[winth - 1] = theWidth;
	winHeight[winth - 1] = theHeight;

	SetWindowPos(hWnd[winth - 1], HWND_TOP, 0, 0, theWidth, theHeight, SWP_NOMOVE);

	return 0;
}

int CWindowsManager::updateWindow(int winth)
{
	if ( (winth <= 0) || (winth > WINDOW_NUM) ) 
	{
		return -1;
	}

	UpdateWindow(hWnd[winth - 1]);

	return 0;
}



CWindowsManager::~CWindowsManager()
{
	delete pGraphicsManager;
}


//---------------Window Process Function: Process Message--------------//

LRESULT CALLBACK WinProc_PPI1(
						 HWND    hwnd,
						 UINT    uMsg,
						 WPARAM  wParam,
						 LPARAM  lParam
						 )
{
	switch(uMsg)
	{
	case WM_SIZE:
		winWidth[0] = LOWORD(lParam);
		winHeight[0] = HIWORD(lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}


LRESULT CALLBACK WinProc_MINI1(
						 HWND    hwnd,
						 UINT    uMsg,
						 WPARAM  wParam,
						 LPARAM  lParam
						 )
{
	switch(uMsg)
	{
	case WM_SIZE:
		winWidth[1] = LOWORD(lParam);
		winHeight[1] = HIWORD(lParam);
		break;

	case WM_DESTROY:

		PostQuitMessage(NULL);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}





LRESULT CALLBACK WinProc_AR1(
						 HWND    hwnd,
						 UINT    uMsg,
						 WPARAM  wParam,
						 LPARAM  lParam
						 )
{
	switch(uMsg)
	{
	case WM_SIZE:
		winWidth[2] = LOWORD(lParam);
		winHeight[2] = HIWORD(lParam);
		break;

	case WM_DESTROY:

		PostQuitMessage(NULL);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}


LRESULT CALLBACK WinProc_PPI2(
						 HWND    hwnd,
						 UINT    uMsg,
						 WPARAM  wParam,
						 LPARAM  lParam
						 )
{
	switch(uMsg)
	{
	case WM_SIZE:
		winWidth[3] = LOWORD(lParam);
		winHeight[3] = HIWORD(lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}


LRESULT CALLBACK WinProc_MINI2(
						 HWND    hwnd,
						 UINT    uMsg,
						 WPARAM  wParam,
						 LPARAM  lParam
						 )
{
	switch(uMsg)
	{
	case WM_SIZE:
		winWidth[4] = LOWORD(lParam);
		winHeight[4] = HIWORD(lParam);
		break;

	case WM_DESTROY:

		PostQuitMessage(NULL);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}



LRESULT CALLBACK WinProc_AR2(
						 HWND    hwnd,
						 UINT    uMsg,
						 WPARAM  wParam,
						 LPARAM  lParam
						 )
{
	switch(uMsg)
	{
	case WM_SIZE:
		winWidth[5] = LOWORD(lParam);
		winHeight[5] = HIWORD(lParam);
		break;

	case WM_DESTROY:

		PostQuitMessage(NULL);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}
 

LRESULT CALLBACK WinProc_TV1(
						 HWND    hwnd,
						 UINT    uMsg,
						 WPARAM  wParam,
						 LPARAM  lParam
						 )
{
	switch(uMsg)
	{
	case WM_SIZE:
		winWidth[6] = LOWORD(lParam);
		winHeight[6] = HIWORD(lParam);
		break;

	case WM_DESTROY:

		PostQuitMessage(NULL);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}
 


LRESULT CALLBACK WinProc_TV2(
						 HWND    hwnd,
						 UINT    uMsg,
						 WPARAM  wParam,
						 LPARAM  lParam
						 )
{
	switch(uMsg)
	{
	case WM_SIZE:
		winWidth[7] = LOWORD(lParam);
		winHeight[7] = HIWORD(lParam);
		break;

	case WM_DESTROY:

		PostQuitMessage(NULL);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}




