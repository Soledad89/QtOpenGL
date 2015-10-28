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
	WNDCLASS wndcls[WINDOW_NUM];     //������
	HWND   hWnd[WINDOW_NUM];         //���ھ�������ÿ�����ڶ���һ����֮��Ӧ�Ĵ����࣬��Ϊÿ��������Ĺ��̺�������һ��
	HDC    hDC[WINDOW_NUM];

	bool isOpen[WINDOW_NUM];        // ��¼���������Ƿ��Ѿ�����
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


