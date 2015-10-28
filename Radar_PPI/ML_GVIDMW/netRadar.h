

#ifndef __NETRADAR_BB64F08B_F358_4f13_8509_598DF91965BE
#define __NETRADAR_BB64F08B_F358_4f13_8509_598DF91965BE



#include "Communication.h"
#include "ppidef.h"

#include <windows.h>

typedef std::deque<s_ppi_video_for_disp*> DeqDispPointer;        // 空队列
typedef DeqDispPointer::iterator DeqDispPointerIte;

#define DISP_BUF_NUM 1000

extern unsigned char ScanMode;

class CNetRadar : public CCommunication
{
public:
	CNetRadar();
	~CNetRadar();

	bool init();
	int SetScanMode(char theScanMode);

private:

//	unsigned char scanMode;
	HWND hMyWnd;

	__declspec(align(16)) s_ppi_video_for_disp dispPkg1[DISP_BUF_NUM];
	__declspec(align(16)) s_ppi_video_for_disp dispPkg2[DISP_BUF_NUM];
	
	s_ppi_video_for_disp * dispbuf;

	DeqDispPointer deqDP1;//维护缓冲区，空闲区段的指针队列
	DeqDispPointer deqDP2;

	DeqDispPointer deqDispP1;//维护缓冲区，已存数据区段的指针队列
	DeqDispPointer deqDispP2;

	CRITICAL_SECTION cs1;
	CRITICAL_SECTION cs2;
	HANDLE sem1;
	HANDLE sem2;

	HANDLE thread1;
	HANDLE thread2;

private:
	void OnReceive(unsigned char id,short len,char* pData,int pos);// pos=0 is begin;pos=-i is end;other positive is position

	static DWORD WINAPI RadarThread_1(void* pParam);
	static DWORD WINAPI RadarThread_2(void* pParam);

	void RadarLoop_1();
	void RadarLoop_2();



};




#endif