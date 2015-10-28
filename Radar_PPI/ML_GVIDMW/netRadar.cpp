
//  Version: 1.0

#include "netRadar.h"
#include "CAR.h"
#include "CPPI.h"

extern CPPI* pPPI[2];
extern CAR* pAR[2];
extern CRITICAL_SECTION csTex;


CNetRadar::CNetRadar()
{
	sem1 = NULL;
	sem2 = NULL;
	thread1 = NULL;
	thread2 = NULL;
}

CNetRadar::~CNetRadar()
{

}

bool CNetRadar::init()
{
	for(int i=0;i<DISP_BUF_NUM;i++)
	{
		deqDP1.push_back(&dispPkg1[i]);        // 装进1000个空的数据包 s_ppi_video_for_disp
		deqDP2.push_back(&dispPkg2[i]);
	}

	sem1 = CreateSemaphore(NULL,0,DISP_BUF_NUM*2,NULL);
	sem2 = CreateSemaphore(NULL,0,DISP_BUF_NUM*2,NULL);
	InitializeCriticalSection(&cs1);
	InitializeCriticalSection(&cs2);


	// create thread
	DWORD threadID;
	thread1 = CreateThread(NULL,0,RadarThread_1,this,0,&threadID);
	if(thread1==NULL)return false;

	//fprintf(debugfp,"radar thread 1 created!\n");

	thread2 = CreateThread(NULL,0,RadarThread_2,this,0,&threadID);
	if(thread2==NULL)return false;

	//fprintf(debugfp,"radar thread 2 created!\n");


	CreateServer();
	//CreateServer(port);

	return true;
}

int CNetRadar::SetScanMode( char theScanMode)
{
	ScanMode = theScanMode;
	return 0;
}

/************************************************************************/
/* OnReceive当期望的数据体(编号为ID，长度为len)到达后的发生，pData为发来的数据的起始地址(不包含报文头)，可以拷贝直接使用
/*此函数为虚函数，由使用方重写                                          */
/************************************************************************/
void CNetRadar::OnReceive(unsigned char id, short len,char* pData,int pos)
{
//	scanMode = 0;
	static int lastAzi1=0;
	static int lastAzi2=0;

	int lastAzi;
	int curAzi;
	int i;

	static bool post1=false;
	static bool post2=false;

	//fprintf(debugfp,"recvpkg,id=%d\n",id);
	//printf("recvpkg,id=%d\n",id);

	if(id==10)
	{
		EnterCriticalSection(&cs1);
		if(deqDP1.size()>0)
		{
			dispbuf=*deqDP1.begin();         //头部第一个元素：起始空包的地址
			deqDP1.pop_front();              //删除头部第一个元素
			LeaveCriticalSection(&cs1); 
		}
		else
		{
			LeaveCriticalSection(&cs1);
			//fprintf(debugfp,"buff 1 is full!\n");
			return;
		}
		lastAzi = lastAzi1;
	}
	else if(id==11)
	{
		EnterCriticalSection(&cs2);
		if(deqDP2.size()>0)
		{
			dispbuf=*deqDP2.begin();
			deqDP2.pop_front();
			LeaveCriticalSection(&cs2);
		}
		else
		{
			LeaveCriticalSection(&cs2);
			return;
		}
		lastAzi = lastAzi2;
	}
	else
		return;


	if(pos==0){  //大报文(XSMXP)起始
		post1=false;
		post2=false;
	}

	curAzi = *(unsigned short*)(pData+2);        // 当前方位

	//fprintf(debugfp,"curAzi=%d\n",curAzi);

	// 补线
	if(ScanMode ==0|| ScanMode == 2 || ScanMode == 4 )                              //正向扫描时
	{
		if(curAzi>lastAzi && curAzi<lastAzi+20)                           //连续补线：  < 20             
		{
			dispbuf->znum=curAzi-lastAzi;                                      //多少条线 
			for(i=0;i<dispbuf->znum && i<max_azi_num_per_video;i++)
				dispbuf->azicnt[i]=lastAzi+i;                                  //线的方位
		}
		else if(curAzi<lastAzi && curAzi<20 && lastAzi > 4096-20)         //首尾补线：  < 40  24 
		{
			dispbuf->znum=4096+curAzi-lastAzi;
			for(i=0;i<dispbuf->znum && i<max_azi_num_per_video;i++)            //可能没有补全？
				dispbuf->azicnt[i]=(lastAzi+i)%4096;
		}
		else                                                              //不补线                           
		{
			dispbuf->znum=1;
			dispbuf->azicnt[0]=curAzi;
			//return;
		}
	}
	else                                                                 //反向扫描时
	{
		if(curAzi< lastAzi && curAzi>lastAzi-20)                           //连续补线：  < 20             
		{
			dispbuf->znum=lastAzi-curAzi;                                      //多少条线 
			for(i=0;i<dispbuf->znum && i<max_azi_num_per_video;i++)
				dispbuf->azicnt[i]=lastAzi-i;                                  //线的方位
		}
		else if(curAzi>lastAzi && curAzi > 4096-20<20 && lastAzi<20)         //首尾补线：  < 40  24 
		{
			dispbuf->znum=4096+lastAzi-curAzi;
			for(i=0;i<dispbuf->znum && i<max_azi_num_per_video;i++)            //可能没有补全？
				dispbuf->azicnt[i]=(curAzi+i)%4096;
		}
		else                                                              //不补线                           
		{
			dispbuf->znum=1;
			dispbuf->azicnt[0]=curAzi;
			//return;
		}
	}

	lastAzi = curAzi;

	memcpy(dispbuf->videodata,pData+4,len-4);//pData中前四个字节为其他信息          雷达数据线上的值

	//********* 到此，一个dispbuf里的数据已经填满 ********//



//TRACEN; 
	if(id==10){//channel 1
		lastAzi1=lastAzi;
		EnterCriticalSection(&cs1);
		deqDispP1.push_back(dispbuf);      //将dispbuf装到队列deqDispP1的尾部
		LeaveCriticalSection(&cs1);
		post1=true;
		//ReleaseSemaphore(sem1,1,NULL);
	}
	else if(id==11){//channel 2
		lastAzi2=lastAzi;
		EnterCriticalSection(&cs2);
		deqDispP2.push_back(dispbuf);
		LeaveCriticalSection(&cs2);
		post2=true;
		//ReleaseSemaphore(sem2,1,NULL);
	}

	//fprintf(debugfp,"recv%d\n",pos);

	if(pos<0){//大报文(XSMXP)结束,绘制该报文传来的视频
		if(post1)
			ReleaseSemaphore(sem1,1,NULL);               // 装完一个dispbuf，信号量增1，最多只能装2000个
		if(post2)
			ReleaseSemaphore(sem2,1,NULL);
	}

}



DWORD WINAPI CNetRadar::RadarThread_1(void* pParam)
{
	CNetRadar * pointer = (CNetRadar *) pParam;

	if (pointer)
	{
		pointer->RadarLoop_1();
		return 1;
	}
	return 0;
}

DWORD WINAPI CNetRadar::RadarThread_2(void* pParam)
{
	CNetRadar * pointer = (CNetRadar *) pParam;

	if (pointer)
	{
		pointer->RadarLoop_2();
		return 1;
	}
	return 0;
}

void CNetRadar::RadarLoop_1()// update
{
	s_ppi_video_for_disp * p;
	int num;
	static int a=0;
	bool drawScan=false;

	//fprintf(debugfp,"running radar loop 1\n");

	while(true)
	{
		WaitForSingleObject(sem1,INFINITE);

		EnterCriticalSection(&cs1);
		num = deqDispP1.size();
		LeaveCriticalSection(&cs1);

		//while(num-->0)
		//{
		//	EnterCriticalSection(&cs1);
		//	p=*deqDispP1.begin();
		//	LeaveCriticalSection(&cs1);

		//	EnterCriticalSection(&csTex);
		//	pPPI[0] -> PPIDraw(p);
		//	pAR[0] -> ARDraw(p);
		//	LeaveCriticalSection(&csTex);

		//	EnterCriticalSection(&cs1);
		//	deqDP1.push_back(p);
		//	deqDispP1.pop_front();
		//	LeaveCriticalSection(&cs1);
		//}
		

		while(num-->0)
		{
			EnterCriticalSection(&csTex);

			EnterCriticalSection(&cs1);
			p=*deqDispP1.begin();
			LeaveCriticalSection(&cs1);

			pPPI[0] -> PPIDraw(p);
			pAR[0] -> ARDraw(p);

			EnterCriticalSection(&cs1);
			deqDP1.push_back(p);
			deqDispP1.pop_front();
			LeaveCriticalSection(&cs1);

			LeaveCriticalSection(&csTex);
		}
		


		//while(num >= NUM_PACK)
		//{
		//	EnterCriticalSection(&csTex);
		//	for(int i = 0; i < NUM_PACK; i ++)
		//	{
		//	EnterCriticalSection(&cs1);
		//	p=*deqDispP1.begin();
		//	LeaveCriticalSection(&cs1);

		//	pPPI[0] -> PPIDraw(p);
		//	pAR[0] -> ARDraw(p);

		//	EnterCriticalSection(&cs1);
		//	deqDP1.push_back(p);
		//	deqDispP1.pop_front();
		//	LeaveCriticalSection(&cs1);
		//	num--;
		//	}
		//	LeaveCriticalSection(&csTex);
		//}


	}
}
void CNetRadar::RadarLoop_2()
{
	s_ppi_video_for_disp * p;
	int num;
	static int a=0;
	bool drawScan=false;

	//fprintf(debugfp,"running radar loop 2\n");

	while(true)
	{
		WaitForSingleObject(sem2,INFINITE);

		EnterCriticalSection(&cs2);
		num = deqDispP2.size();
		LeaveCriticalSection(&cs2);


		//if(num>0){
		//	p_widget->widget2->ppi->ppiClearScanLine(a);
		//	drawScan = true;
		//}

		//while(num-->0)
		//{
		//	EnterCriticalSection(&cs2);
		//	p=*deqDispP2.begin();
		//	LeaveCriticalSection(&cs2);


		//	EnterCriticalSection(&csTex);
		//	pPPI[1] -> PPIDraw(p);
		//	pAR[1] -> ARDraw(p);
		//	LeaveCriticalSection(&csTex);


		//	EnterCriticalSection(&cs2);
		//	deqDP2.push_back(p);
		//	deqDispP2.pop_front();
		//	LeaveCriticalSection(&cs2);
		//}


		while(num-->0)
		{
			EnterCriticalSection(&csTex);

			EnterCriticalSection(&cs2);
			p=*deqDispP2.begin();
			LeaveCriticalSection(&cs2);


			pPPI[1] -> PPIDraw(p);
			pAR[1] -> ARDraw(p);


			EnterCriticalSection(&cs2);
			deqDP2.push_back(p);
			deqDispP2.pop_front();
			LeaveCriticalSection(&cs2);

			LeaveCriticalSection(&csTex);

		}


		//while(num >= NUM_PACK)
		//{
		//	EnterCriticalSection(&csTex);
		//	for(int i = 0; i < NUM_PACK; i ++)
		//	{
		//	EnterCriticalSection(&cs2);
		//	p=*deqDispP2.begin();
		//	LeaveCriticalSection(&cs2);

		//	pPPI[1] -> PPIDraw(p);
		//	pAR[1] -> ARDraw(p);

		//	EnterCriticalSection(&cs2);
		//	deqDP2.push_back(p);
		//	deqDispP2.pop_front();
		//	LeaveCriticalSection(&cs2);
		//	num--;
		//	}
		//	LeaveCriticalSection(&csTex);
		//}



	}
}