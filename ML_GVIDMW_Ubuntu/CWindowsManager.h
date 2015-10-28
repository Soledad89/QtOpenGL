// Name:   CWindowsManager.h
// Author: Yan Guo
// Date:   Feb 27, 2013 in the canteen of Rohm Building

#include <windows.h>
#include "CGraphicsManager.h" 


#ifndef _C_WINDOWS_MANAGER_H_
#define _C_WINDOWS_MANAGER_H_

#define WINDOW_NUM    8

class CWindowsManager
{
private:
	WNDCLASS wndcls[WINDOW_NUM];     //窗口类
	HWND   hWnd[WINDOW_NUM];         //窗口句柄，这里，每个窗口都有一个与之对应的窗口类，因为每个窗口类的过程函数都不一样
	HDC    hDC[WINDOW_NUM];

	bool isOpen[WINDOW_NUM];        // 记录各个窗口是否已经被打开
	CGraphicsManager* pGraphicsManager;

public:
	CWindowsManager();
	~CWindowsManager();

	int openWindow(int winth);
	int closeWindow(int winth);
	int setWindowXY(int winth, short theX ,short theY);
	int setWindowWH(int winth, short theWidth, short theHeight);
	int updateWindow(int winth);
	int getWH(int winth, int* pWidth, int* pHeight);
	HDC* getHDC();
	HWND* getHWND();

private:
	int createWindow(int winth);
	int setDCPixelFormat(HDC hDC);
};

#endif


