// Name:   CAR.cpp
// Author: Yan Guo
// Date:   Feb 4, 2013 in the canteen of Rohm Building

#include "CAR.h"
#include "ppidef.h"
#include "CGraphicsManager.h"
#include "mycolor.h"

CAR::CAR( CGraphicsManager* pGM, char videoChannel )
{
	
	VideoChannel = videoChannel;
	winth = 3 * videoChannel;        // 分别对应第3和第6个窗口

	video_data = new VIDEODATATYPE[CNTVIDEO];
	dispRadius = 1200;

	isOpen = 1;
	frost = 0;
	changeBackgroundColor = FALSE;
	for(int i = 0; i < 2; i ++)
		visibleAR[i] = 1;
 
	base = 60;
	backColor = 0;

	ARSetColor(0xff00007f);
//	ARSetBackgroundColor(0x0000007f);

	PPIRange = 150;
	range = 150;
	
	pGraphicsManager = pGM;
	vertices = new int[2 * CNTVIDEO];
	colors = new unsigned int[CNTVIDEO];

	pGraphicsManager ->clear (winth, 2, 0x00000000);
	pGraphicsManager ->clear (winth, 3, 0x0000004f);

	//pGraphicsManager ->clear (6, 3, 0x0000007f);
}


//函数功能： 视屏值小于theGate的数据不显示
int CAR::ARSetGate(unsigned char theGate)
{
	base = theGate;

	return 0;
}


//函数功能：设置AR的量程
int CAR::ARSetDist(short theDist)
{
	range = theDist;
	if(range > PPIRange)
		return -7;

	return 0;
}

int CAR::ARsetPPIRange(short theDist)
{
	PPIRange = theDist;

	return 0;
}


//函数功能：设置AR余晖衰减的程度
int CAR::ARSetTail(int theTail)
{
	decayAR =128 - theTail / 512;

	int r, g, b, a;

	r = backColor >> 24;
	g = (backColor >> 16) & 0xff;
	b = (backColor >> 8)& 0xff ;
	a = decayAR ;

	backColor = REFC(a,b,g,r);

	changeBackgroundColor = 1;


	return 0;
}

//函数功能：打开AR窗口中的雷达数据层，关闭的时候，雷达数据层被清屏，并且不再画了
int CAR::ARSetSwitch(short theSwitch)
{
	isOpen = theSwitch;	

	return 0;
}


//函数功能：是否冻结AR窗口中的雷达数据层，不清屏，后台也不再画了
int CAR::ARSetFrost(char theFrost)
{
	frost = theFrost;

	return 0;
}


//函数功能：设置AR线的颜色
int CAR::ARSetColor(unsigned int theColor)
{
	int r, g, b, a;

	r = theColor >> 24;
	g = (theColor >> 16) & 0xff;
	b = (theColor >> 8) & 0xff;
	a = 0xff;

	colorAR = REFC(r, g, b, a);

	return 0;
}

//函数功能：设置AR雷达数据层的背景颜色
int CAR::ARSetBackgroundColor(unsigned int theColor)
{
	int r, g, b, a;

	r = theColor >> 24;
	g = (theColor >> 16) & 0xff;
	b = (theColor >> 8)& 0xff ;
	a = 0x4f;

	backColor = REFC(a,b,g,r);


	changeBackgroundColor = 1;
//	ARClear(3);
//	pGraphicsManager ->clear (winth, 2, 0x00ff0000); 
	return 0;
}


//函数功能：控制窗口叠加。关闭时，不清屏。当处于关闭状态时，PPI后页层继续绘制，不叠加。
int CAR::ARSetVisible(int surfth, short theSwitch)
{
	visibleAR[surfth - 1] = theSwitch;
	return 0;
}


//函数功能：将dispdat中的数据绘制到AR窗口的后页FBO中
int CAR::ARDraw(s_ppi_video_for_disp* dispdat)
{
	unsigned char v;

	if(isOpen && (!frost))       // 只有雷达数据层被打开，且不被冻结的情况下，后台才会画
	{	
		for(int i = 0; i < CNTVIDEO; i ++)
		{
			v = dispdat->videodata[i];
			video_data[i]  = (v > base) ? v : base;
		}

		int c = 10;
		int numPoints = dispRadius/c;
		float percent = float(range) / PPIRange;
		//获取待显示点的坐标值和颜色值
		for(int i = 0; i < numPoints; i ++)
		{
			vertices[2 * i] = float(i) / numPoints * SURF_PPI_WIDTH  ;          // 所需绘制到后页FBO中的点的坐标值     AR窗口后页的大小为：1200 * 1200
			vertices[2 * i + 1] = float(video_data[int(c /2 * i * percent)]) / 256 * SURF_PPI_HEIGHT;

			colors[i] = colorAR;       //AR窗口的雷达数据层在最底层
		}

		pGraphicsManager -> drawLines(winth, vertices, colors, numPoints);    // 将雷达数据线绘制到AR窗口后页FBO的第2层中

	}

	return 0;
}



//函数功能： 清除AR窗口中一个层的整个屏幕
int CAR::ARClear(int surfth)       // surfth: 1, 2
{
	if((surfth != 1) && (surfth != 2)&&(surfth != 3))
	{
		return -1;
	}
	
	pGraphicsManager -> clear(winth, surfth, backColor);       // AR：第1层是用户标绘层，第2层是雷达数据层
	return 0;
}


//函数功能： 将AR后页FBO所绘制的图形贴到Windows屏幕上，图层1与图层2进行叠加
int CAR::ARUpdate()
{
//changeBackgroundColor = 0;
	if(changeBackgroundColor)
	{
//		pGraphicsManager ->clear (winth, 4, backColor);
		pGraphicsManager ->clear (winth, 2, backColor);
		pGraphicsManager ->clear (winth, 3, backColor);
		pGraphicsManager -> update(winth);
		changeBackgroundColor = 0;
	}
	else{

	if(visibleAR[1])
	{
		pGraphicsManager -> copyTexFBO(winth, 2, winth, 3);           // 衰减
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 2);           // 雷达数据层
	}
	if(visibleAR[0])
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 1);           // 用户层
	
	pGraphicsManager -> copyTex(winth, winth, 4);
	pGraphicsManager -> update(winth);}

	return 0;
}

int CAR::ARsetColorKey(unsigned int colorKey,BMPImageRec* pBackMap1, int x, int y)
{
	pGraphicsManager -> setColorKey(winth, colorKey, pBackMap1,x,y);
	return 1;
}


CAR::~CAR()
{
	delete[]video_data;
	delete[]vertices;
	delete[]colors;
}
