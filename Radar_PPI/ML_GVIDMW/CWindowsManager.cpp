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
	
	wndcls[0].lpfnWndProc = WinProc_PPI1;        // 窗口类的窗口过程函数
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



// 函数功能： 获取第winth个窗口的宽、高
int CWindowsManager::getWH(int winth, int* pWidth, int* pHeight)
{
	*pWidth = winWidth[winth];
	*pHeight = winHeight[winth];

	return 0;
}


//函数功能：获取窗口的HWND
HWND* CWindowsManager::getHWND()
{
	return hWnd;
}


//函数功能：获取窗口的DC
HDC* CWindowsManager::getHDC()
{
	return hDC;
}


//函数功能：创建第winth个窗口
int CWindowsManager::createWindow(int winth)
{

		//Step1: Design PPI Window Class

		wndcls[winth - 1].cbClsExtra = 0;        // 窗口类追加内存
		wndcls[winth - 1].cbWndExtra = 0;        // 窗口追加内存
		wndcls[winth - 1].hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);      // 背景画刷句柄
		wndcls[winth - 1].hCursor = LoadCursor(NULL, IDC_CROSS);               // 光标句柄
		wndcls[winth - 1].hIcon = LoadIcon(NULL, IDI_ERROR);                   // 图标句柄
		wndcls[winth - 1].hInstance = NULL;                                    // 实例句柄
		wndcls[winth - 1].lpszClassName = winclsName[winth - 1];                                 // 窗口类名
		wndcls[winth - 1].lpszMenuName = NULL;                                      // ->菜单资源名
		wndcls[winth - 1].style = CS_HREDRAW | CS_VREDRAW;                          // 窗口式样

		RegisterClass(&wndcls[winth - 1]);         //Step2: Register Window Class
	
		hWnd[winth - 1] =  CreateWindow(           //Step3: Create Window	
			winclsName[winth - 1], 
			(windowName[winth - 1]), 
			WS_OVERLAPPEDWINDOW, 
			winPosX[winth - 1],          // 窗口水平位置
			winPosY[winth - 1],          // 窗口垂直位置
			winWidth[winth - 1],        // 窗口宽度
			winHeight[winth - 1],        // 窗口高度
			NULL,       // -> 父窗口句柄
			NULL,       // -> 菜单或子窗口标示符
			NULL,  // -> 应用程序实例
			NULL        // -> 窗口创建数据
			);

	return 0;
}


//函数功能：为各个窗口的DC创建特定的像素格式
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



// 函数功能：打开第winth个窗口， winth: 1 ~ 8
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


// 函数功能：关闭第winth个窗口， winth: 1 ~ 8
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



//函数功能：设置第winth个窗口的位置
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


//函数功能：设置第winth个窗口的大小
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




