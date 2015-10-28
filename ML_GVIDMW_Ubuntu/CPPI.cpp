// Name:   CPI.cpp
// Author: Yan Guo
// Date:   Feb 27, 2013 in the canteen of Rohm Building

#include "CPPI.h"

#include "CAR.h"
#include "ppidef.h"
#include "CWindowsManager.h"
#include "CGraphicsManager.h"
#include "mycolor.h"

extern int winWidth[WINDOW_NUM];
extern int winHeight[WINDOW_NUM];

CPPI::CPPI( CDataPool *dp, CGraphicsManager* pGM, char videoChannel )
{
	VideoChannel = videoChannel;
	winth = 3 * (VideoChannel - 1) + 1;

    addr_near_x = dp->addr_near_x;
    addr_near_y = dp->addr_near_y;
    addr_far_x = dp->addr_far_x;
    addr_far_y = dp->addr_far_y;

	layerPPI = dp -> layerPPI[VideoChannel - 1];
	layerWin = dp -> layerWin[VideoChannel - 1];

	colortablePPI = dp -> colortablePPI[VideoChannel - 1];
	colortableWin = dp -> colortableWin[VideoChannel - 1];	

	scanLine = 0;
	scanLineColor = new unsigned int[CNTVIDEO];
	PPISetScanLineColor(0xff0000ff);

	isOpen = 1;
	frost = 0;
	for(int i = 0; i < 3; i ++)
		visiblePPI[i] = 1;
	for(int i = 0; i < 2; i ++)
		visibleWin[i] = 1;


	backColor = 0;
	base = 0;
	dispMode = 1;
	prowIndex = 0;

	range = 150;
	distanceWin = 100;
	directionWin = 30;
	scaleWin = 2;
	
	comdisp = 0;
	
	video_data = new VIDEODATATYPE[CNTVIDEO];       //用于存储一次视频数据
	dispRadius = 600;
	xorg = 600;
	yorg = 600;


	numPack = 0;
	np = 0;

	line = 0;

	pGraphicsManager = pGM;
	vertices = new int[2 * CNTVIDEO * 50];
	scanLineVertices = new int[2 * CNTVIDEO * 50];
	colorsPPI = new unsigned int[CNTVIDEO * 50];
	colorsWin = new unsigned int[CNTVIDEO * 50];

	pGraphicsManager ->clear (winth, 3, 0x000000ff);
}

//#define NUM_PACK 10

// 函数功能：处理接收到的包，当数目达到一定程度时，将其绘制到后页FBO中
int CPPI::PPIDraw(s_ppi_video_for_disp* dispdat)
{
	PPIPackProc(dispdat);
	numPack++;
	if(numPack == NUM_PACK)
	{		
		pGraphicsManager -> drawPoints(winth, 2, vertices, colorsPPI, np);   // 将雷达数据线绘制到PPI窗口的后页FBO中
		pGraphicsManager -> drawPoints(winth + 1, 2, vertices, colorsWin, np);   // 将雷达数据线绘制到Win窗口的后页FBO中
		numPack = 0;
		np = 0;
	}

	return 0;
}


//函数功能：将数据包dispdat中的数据绘制到后页FBO中，这个函数可以看作是对外的接口
int CPPI::PPIPackProc(s_ppi_video_for_disp* dispdat)
{
	int ta;//绘图方位
	int j;
	unsigned char v;

	if(isOpen && (!frost))       // PPI打开时绘制
	{
		for(int i = 0; i < CNTVIDEO; i ++)
		{
			v = dispdat->videodata[i];
			video_data[i]  = (v > base) ? v : 0;
		}

		switch(dispMode)        // 0；P显   1：B显    2：E显
		{
		case 0:
			for(j=0;j<dispdat->znum;j++)
			{
				ta=(dispdat->azicnt[j] + prowIndex)%4096;
				BScopeLineProc(ta);				
			}
			break;

		case 1:
			for(j=0;j<dispdat->znum;j++)
			{
				ta=(dispdat->azicnt[j] + prowIndex)%4096;
				PPILineProc(ta);
			}
			break;

		case 2:

			for(j=0;j<dispdat->znum;j++)
			{
				ta=(dispdat->azicnt[j] + prowIndex)%4096;
				EScopeLineProc(ta);
			}
			break;

		default:
			break;
		}
	}

	return 0;
}



int CPPI::PPILineProc(int azicnt)
{
	unsigned char v;
	int x, y;

	line = 0;
	

	// 计算所需绘制到后页FBO中的各点的坐标值和颜色值，并将待更新的数据存储在layerPPI中，存储的目的是为了下一次的比大写入
	
	if(dispRadius <= ADDR_NEAR_SMP_LEN)             // Case 1： 所要显示的雷达数据个数小于等于512,只有近区需要绘制
	{
		for(int i = 0; i < dispRadius; i ++)
		{
 			x = xorg + addr_near_x[azicnt][i];              //直角坐标值
			y = yorg + addr_near_y[azicnt][i];

			scanLineVertices[2 * line] = x;          // 所需绘制到后页FBO中的点的坐标值			
			scanLineVertices[2 *line + 1] = y;	
			line ++;
		
			if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
			{
				v = video_data[i];      //一个坐标点的强度，对应屏幕上像素点的颜色值

				if(v > layerPPI[y][x].first)     //比大写入：若是一个坐标点新的视频数据值大于之前的数据值，就更新，否则不更新，优点：尽量保留大目标，缺点：失去实时性，此时这个位置的大目标已经不存在了
				{
					if(layerPPI[y][x].tailMark)    // 若此点是尾迹点，则它会被覆盖，去掉其尾迹标志
					{
						layerPPI[y][x].tailMark = false;
					}
					layerPPI[y][x].first = v;                     // 原始雷达数据值[0,255]
					
					vertices[2 * np] = x;          // 所需绘制到后页FBO中的点的坐标值			
					vertices[2 * np + 1] = y;				
					colorsPPI[np] = colortablePPI[v];   // 所需绘制到后页FBO中的点的颜色值
					colorsWin[np] = colortableWin[v];
					np++;
				}
			}
		}
	}
	else                       // Case 2： 所要显示的雷达数据个数大于512，分为近区和远区
	{
		for(int i = 0; i < ADDR_NEAR_SMP_LEN; i ++)         //绘制近区
		{
 			x = xorg + addr_near_x[azicnt][i];              //FBO最终绘制坐标值，PPI的中心点在(xorg,yorg)，后页FBO的坐标系统是（0,1200），（0,1200）
			y = yorg + addr_near_y[azicnt][i];          // note this is minus	

			scanLineVertices[2 * line] = x;          // 所需绘制到后页FBO中的点的坐标值			
			scanLineVertices[2 * line + 1] = y;	
			line ++;
		
			if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
			{
				v = video_data[i];      //一个坐标点的强度，对应屏幕上像素点的颜色值
		
				if(v > layerPPI[y][x].first)     //比大写入：若是一个坐标点新的视频数据值大于之前的数据值，就更新，否则不更新，优点：尽量保留大目标，缺点：失去实时性，此时这个位置的大目标已经不存在了
				{
					if(layerPPI[y][x].tailMark)
					{
						layerPPI[y][x].tailMark = false;
					}
					layerPPI[y][x].first = v;                     // 原始雷达数据值[0,255]

					vertices[2 * np] = x;          // 所需绘制到后页FBO中的点的坐标值			
					vertices[2 * np + 1] = y;
					colorsPPI[np] = colortablePPI[v];   // 所需绘制到后页FBO中的点的颜色值
					colorsWin[np] = colortableWin[v];
					np++;
				}
			}
		}
		for(int i = ADDR_NEAR_SMP_LEN; i < dispRadius; i ++)     //绘制远区，远区的一个方向azicnt需要绘制两条线，azicnt = 0 ~ 4095
		{
            x = xorg + addr_far_x[2*(azicnt)][i-ADDR_NEAR_SMP_LEN];        //第一个点的坐标值
            y = yorg + addr_far_y[2*(azicnt)][i-ADDR_NEAR_SMP_LEN];

			scanLineVertices[2 * line] = x;          // 所需绘制到后页FBO中的点的坐标值			
			scanLineVertices[2 * line + 1] = y;	
			line ++;

			if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
			{	
				v = video_data[i]; 
				if((v>layerPPI[y][x].first)&&(v>base))     //比大写入第一个坐标点
				{
					if(layerPPI[y][x].tailMark)
					{
						layerPPI[y][x].tailMark = false;
					}
					layerPPI[y][x].first = v;                  // 原始雷达数据值[0,255]

					vertices[2 * np] = x;          // 所需绘制到后页FBO中的点的坐标值			
					vertices[2 * np + 1] = y;
					colorsPPI[np] = colortablePPI[v];   // 所需绘制到后页FBO中的点的颜色值
					colorsWin[np] = colortableWin[v];
					np++;
				}
			}

            x = xorg + addr_far_x[2*(azicnt)+1][i-ADDR_NEAR_SMP_LEN];        //第二个点的坐标值
            y = yorg + addr_far_y[2*(azicnt)+1][i-ADDR_NEAR_SMP_LEN];

			scanLineVertices[2 * line] = x;          // 所需绘制到后页FBO中的点的坐标值			
			scanLineVertices[2 * line + 1] = y;	
			line ++;
			
			if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
			{
				v = video_data[i]; 
				if((v>layerPPI[y][x].first)&&(v>base))     //比大写入第二个坐标点
				{
					if(layerPPI[y][x].tailMark)
					{
						layerPPI[y][x].tailMark = false;
					}
					layerPPI[y][x].first = v;                  // 原始雷达数据值[0,255]

					vertices[2 * np] = x;          // 所需绘制到后页FBO中的点的坐标值			
					vertices[2 * np + 1] = y;
					colorsPPI[np] = colortablePPI[v];   // 所需绘制到后页FBO中的点的颜色值
					colorsWin[np] = colortableWin[v];
					np++;
				}
			}
		}
	}

	return 0;
}



int CPPI::BScopeLineProc( int azicnt )
{
	unsigned char v;
	int x, y;

	x = azicnt * SURF_PPI_WIDTH / AZI_NUM;

	for(int i = 0; i < 600; i++)
	{
		//v = video_data[i];
		//if(v==0)
		//	continue;
		//else
		//{
		//	v += base;
		//	if(v<=0)continue;
		//}

		y = i + 300;
		
		if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
		{
			v = video_data[i];
			if((v>layerPPI[y][x].first)&&(v>base))     //比大写入
			{
				if(layerPPI[y][x].tailMark)
				{
					layerPPI[y][x].tailMark = false;
				}
				layerPPI[y][x].first = v;                  // 原始雷达数据值[0,255]

				vertices[2 * np] = x;          // 所需绘制到后页FBO中的点的坐标值			
				vertices[2 * np + 1] = y;
				colorsPPI[np] = colortablePPI[v];   // 所需绘制到后页FBO中的点的颜色值
				colorsWin[np] = colortableWin[v];
				np++;
			}
		}
	}

	return 0;
}


//函数功能：将雷达数据线以E显的方式绘制到后页FBO中
int CPPI::EScopeLineProc( int azicnt )
{
	unsigned char v;
	int x, y;

	y = azicnt * SURF_PPI_HEIGHT / AZI_NUM;

	for(int i = 0; i < 600; i++)
	{
		//v = video_data[i];
		//if(v==0)
		//	continue;
		//else{
		//	v += base;
		//	if(v<=0)continue;
		//}

		x = i + 300;
		
		if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
		{
			v = video_data[i];
			if((v>layerPPI[y][x].first)&&(v>base))     //比大写入
			{
				if(layerPPI[y][x].tailMark)
				{
					layerPPI[y][x].tailMark = false;
				}
				layerPPI[y][x].first = v;                  // 原始雷达数据值[0,255]
				
				vertices[2 * np] = x;          // 所需绘制到后页FBO中的点的坐标值			
				vertices[2 * np + 1] = y;
				colorsPPI[np] = colortablePPI[v];   // 所需绘制到后页FBO中的点的颜色值
				colorsWin[np] = colortableWin[v];
				np++;
			}
		}
	}

	return 0;
}


// 函数功能：绘制背景海图
int CPPI::PPIDrawBackMap(BMPImageRec* pBackMap)
{
	pGraphicsManager -> drawBackMap(winth, pBackMap);

	return 0;
}


//函数功能： 清除PPI窗口中一个层的整个屏幕
int CPPI::PPIClear(int surfth) 
{
	pGraphicsManager -> clear(winth, surfth, backColor);
	return 0;
}


int CPPI::PPIClearLayer()
{
	for(int i = 0; i < SURF_PPI_HEIGHT; i ++)
	{
		memset(layerPPI[i], 0, SURF_PPI_WIDTH * sizeof(s_layer_unit));
	}
	return 0;
}


//函数功能：设置PPI的显示模式
int CPPI::PPISetDispMode(int mode)
{
	if(mode != dispMode)
	{
		PPIClearLayer();
		PPIClear(2);
		WinClear(2);

		dispMode = mode;
	}

	return 0;
}


//函数功能：设置P显的中心位置
int CPPI::PPISetDispCenter(short theX ,short theY)
{
	if((theX!=xorg) || (theY!=yorg))
	{
		PPIClearLayer();
		PPIClear(2);
		WinClear(2);

		xorg = theX;
		yorg = theY;
	}

	return 0;
}


//函数功能：设置舰艏方位
int	CPPI::PPISetShipDir(float theShipDir)
{
	if(theShipDir!=prowIndex)
	{
		PPIClearLayer();
		PPIClear(2);
		WinClear(2);

		prowIndex = theShipDir;
	}

	return 0;
}



//函数功能：设置是否显示PPI扫描线
int CPPI::PPISetScan(int theScan)
{
	scanLine = 1 - theScan;

	return 0;
}


//函数功能：设置PPI扫描线的颜色
int CPPI::PPISetScanLineColor(unsigned int theColor)      // RGBA
{
	int r, g, b, a;

	r = theColor >> 24;
	g = (theColor >> 16) & 0xff;
	b = (theColor >> 8) & 0xff;
	a = 255;
	
	for(int i = 0; i < CNTVIDEO; i ++)
	{
		scanLineColor[i] = theColor;
		scanLineColor[i] = REFC(r,g,b,a);         //将RGBA转换成AGBR，OpenGL在读数据的时候，按地址从小到大的顺序是RGBARGBA在读
	}
	
	return 0;
}

int CPPI::PPISetMoveMode(short theMode)
{
	moveMode = theMode;
	return 0;
}



//函数功能：打开PPI雷达数据层
int CPPI::PPISetSwitch(short theSwitch)
{
	isOpen = theSwitch;
	return 0;
}


// 函数功能： 设置两路雷达数据同窗口显示
int CPPI::PPISetTogether(short theSwitch)
{
	comdisp = theSwitch;
	return 0;
}



//函数功能：设置是否冻结PPI视频，冻结的时候数据还在
int CPPI::PPISetFrost(char theFrost)
{
	frost = theFrost;
	return 0;
}


//函数功能：设置PPI量程
int CPPI::PPISetRange(short theDist)
{
	range = theDist;

	return 0;
}


//函数功能：设置PPI的背景颜色
int CPPI::PPISetBackgroundColor(unsigned int theColor)
{
	int r, g, b, a;

	r = theColor >> 24;
	g = (theColor >> 16) & 0xff;
	b = (theColor >> 8) & 0xff;
	a = 255;

	backColor = REFC(r, g, b, 0);

	return 0;
}


//函数功能：设置PPI门限值，只有大于一定门限值的数据才会显示
int CPPI::PPISetGate(unsigned char theGate)
{
	base = theGate;

	return 0;
}


//函数功能：控制窗口叠加。关闭时，不清屏。当处于关闭状态时，PPI后页层继续绘制，不叠加。
int CPPI::PPISetVisible(int winth, int surfth, short theSwitch)
{
	if(winth == 1)
		visiblePPI[surfth - 1] = theSwitch;
	else
		visibleWin[surfth - 1] = theSwitch;
	
	return 0;
}



//函数功能： 将PPI后页FBO所绘制的图形贴到Windows屏幕上，图层1、图层2以及图层3进行叠加
//int CPPI::PPIUpdate()
//{
//	//pGraphicsManager -> clearWin(winth, 0x01010101); 
//
//	if(visiblePPI[2])
//		pGraphicsManager -> copyTex(winth, winth, 3);           // 背景海图层
//
//	if(visiblePPI[1])
//	{
//		if(comdisp)
//		{
//			pGraphicsManager -> copyTex(winth, winth+3, 2);           // 雷达数据层
//		}
//		pGraphicsManager -> copyTex(winth, winth, 2);           // 雷达数据层
//
//		if(scanLine)
//			pGraphicsManager -> drawWinPoints(winth, vertices, scanLineColor, CNTVIDEO); 
//	}
//
//	if(visiblePPI[0])
//		pGraphicsManager -> copyTex(winth, winth, 1);           // 用户层
//
//	pGraphicsManager -> update(winth);
//
//	return 0;
//}


int CPPI::PPIUpdate()
{
	//pGraphicsManager -> clearWin(winth, 0x01010101); 

	if(visiblePPI[2])
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 3);           // 背景海图层

	if(visiblePPI[1])
	{
		if(comdisp)
		{
			pGraphicsManager -> copyTexFBO(winth, 4, winth+3, 2);           // 雷达数据层
		}
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 2);           // 雷达数据层

		if(scanLine)
			pGraphicsManager -> drawPoints(winth, 4, scanLineVertices, scanLineColor, CNTVIDEO); 
	}

	if(visiblePPI[0])
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 1);           // 用户层
	
	//pGraphicsManager -> update(winth);


	pGraphicsManager -> copyTex(winth, winth, 4);
	pGraphicsManager -> update(winth);

	return 0;
}




// 函数功能：设置雷达视频小窗口距离和方位，根据距离和方位可以确定要开的窗口的中心
int CPPI::WinSetDistDir(short theDistance ,float theDirection)
{
	if((theDistance!=distanceWin) || (theDirection!=directionWin))
	{
		WinClear(2);
		distanceWin = theDistance;
		directionWin = theDirection;
	}

	return 0;
}


// 函数功能：设置视频小窗口的放大倍数
int CPPI::WinSetScale(int theScale)
{
	if(theScale!=scaleWin)
	{
		WinClear(2);
		scaleWin = theScale;
	}
	
	return 0;
}


// 函数功能：设置小窗口中雷达数据层的背景颜色
int CPPI::WinSetBackgroundColor(unsigned int theColor)
{
	int r, g, b, a;

	r = theColor >> 24;
	g = (theColor >> 16) & 0xff;
	b = (theColor >> 8) & 0xff;
	a = 255;

	backColorWin = REFC(r, g, b, a);

//	backColorWin = theColor;
	WinClear(2);

	return 0;
}


//函数功能： 清除小窗口中一个层的整个屏幕
int CPPI::WinClear(int surfth) 
{
	pGraphicsManager -> clear(winth + 1, surfth, backColorWin);

	return 0;
}



//函数功能： 将PPI后页FBO所绘制的一部分图形贴到Windows屏幕上，图层1和图层2进行叠加
int CPPI::WinUpdate()
{
	int R = float(distanceWin) / range * dispRadius;
	int Azi = directionWin / 360 * AZI_NUM;

	float theX_Center = float(xorg + addr_near_x[Azi][R]) / SURF_PPI_WIDTH;  
	float theY_Center = float(yorg + addr_near_y[Azi][R]) / SURF_PPI_HEIGHT;  

	float theWidth = float(winWidth[winth]) / (winWidth[winth - 1] * scaleWin);         // wMin / (wPPI * scale)
	float theHeight = float(winHeight[winth]) / (winHeight[winth - 1] * scaleWin); 

	if(visibleWin[1])                                        // 雷达数据层
	{
		if(comdisp)
			pGraphicsManager -> copyTexRectFBO(winth+1, 3, winth+4, 2, theX_Center - theWidth/2, theY_Center - theHeight/2, theWidth, theHeight);           // 雷达数据层   左下角位置：(theX, theY)， 宽：theWidth， 高：theHeight
		pGraphicsManager -> copyTexRectFBO(winth+1, 3, winth, 3, theX_Center - theWidth/2, theY_Center - theHeight/2, theWidth, theHeight); 
		pGraphicsManager -> copyTexRectFBO(winth+1, 3, winth+1, 2, theX_Center - theWidth/2, theY_Center - theHeight/2, theWidth, theHeight);           // 雷达数据层   左下角位置：(theX, theY)， 宽：theWidth， 高：theHeight
	}		
	if(visibleWin[0])
		pGraphicsManager -> copyTexFBO(winth+1, 3, winth+1, 1);           // 用户层

	pGraphicsManager -> copyTex(winth+1, winth+1, 3);
	pGraphicsManager -> update(winth + 1);

	return 0;
}

int CPPI::PPIsetColorKey(unsigned int theColor, BMPImageRec* pLayerUser, int x, int y)
{
	pGraphicsManager -> setColorKey(winth, theColor, pLayerUser, x, y);
	return 1;
}

int CPPI::WinSetColorKey(unsigned int theColor, BMPImageRec* pLayerUser, int x, int y)
{
	pGraphicsManager -> setColorKey(winth + 1, theColor, pLayerUser, x, y);
	return 1;
}


CPPI::~CPPI()
{
	delete[]video_data;
	delete[]vertices;
	delete[]colorsPPI;
	delete[]colorsWin;

}
