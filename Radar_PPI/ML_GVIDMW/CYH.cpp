// Name:   CYH.cpp
// Author: Yan Guo
// Date:   Mar 7, 2013 in the canteen of Rohm Building


#include "CYH.h"
#include "ppidef.h"
#include "CGraphicsManager.h"


CYH::CYH(CDataPool *dp, CGraphicsManager* pGM, char videoChannel)
{
	VideoChannel = videoChannel;
	winth = 3 * (VideoChannel - 1) + 1;

	randomx = dp -> randomx;
	randomy = dp -> randomy;

	layerPPI = dp -> layerPPI[VideoChannel - 1];
	layerWin = dp -> layerWin[VideoChannel - 1];
	layerAR = dp -> layerAR[VideoChannel - 1];

	colortablePPI = dp -> colortablePPI[VideoChannel - 1];
	colortableWin = dp -> colortableWin[VideoChannel - 1];
	colortableTail = dp -> colortableTail[VideoChannel - 1];

	base = 40;

	yhLen = 9000;
	yhIndex = 0;

	yhDecay = 65;      // 每次更新时，余晖衰减的程度
	tailGate = 200;
	tailSwitch = 1;
	tailDecay = 30;     // 尾迹点在做更新时，衰减的程度

	//yhDecay = 0;
	//tailGate = 0;
	//tailSwitch = 0;
	//tailDecay = 0;

	pGraphicsManager = pGM;
	vertices = new int[2 * yhLen];
	colorsPPI = new unsigned int[yhLen];
	colorsWin = new unsigned int[yhLen];

}


//函数功能：设置PPI余晖程度，当theTail = 0时，不进行余晖衰减
int CYH::setPPIDecay(int theTail)
{
	yhDecay = theTail;

	return 0;
}


//函数功能：设置PPI尾迹开关
int CYH::setPPITailSwitch(short theSwitch)
{
	tailSwitch = theSwitch;

	return 0;
}


//函数功能：设置PPI尾迹门限
int CYH::setPPITailGate(unsigned char theGate)
{
	tailGate = theGate;

	return 0;
}


//函数功能：设置PPI尾迹时间
int CYH::setPPITailTimer(int theTime)
{
	tailDecay = theTime;

	return 0;
}


int CYH::setYHGate(unsigned char theGate)
{
	base = theGate * 32;
	return 0;
}


// 函数功能：对数据进行衰减，若是衰减结果小于0，则令其为0
VIDEODATATYPE CYH::decayData(VIDEODATATYPE data, VIDEODATATYPE decay)
{
	VIDEODATATYPE result = (data - decay) > base ? (data - decay) : 0;

	return result;
}


int CYH::yhPPI()
{
	unsigned char v;
    int x,y;
	int np = 0;
	
	//获取所需进行衰减的点的坐标位置和颜色值
	for(int i = 0; i < yhLen; i++)      // yhLen = 30000     RND_LEN = 1440000   每做一次余辉，会在RND_LEN个随机数据中依次选取yhLen个数据进行处理
	{
		x = randomx[yhIndex + i];      //获取所要进行余辉处理的点的坐标位置，yh_index记录上次处理后的位置
		y = randomy[yhIndex + i];     
			
		//对一个坐标点的雷达数据值进行衰减，得到衰减之后数据值所对应的颜色值
		if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))      //(x,y)在后页FBO坐标系统（0，1200），（0，1200）中      这余辉表中记录的是什么呢？它们的数值可能会超过1200吗？
        {
			vertices[2 * np] = x;          // 所需绘制到后页FBO中的点的坐标值			
			vertices[2 * np + 1] = y;

			v = layerPPI[y][x].first;      // 操作对象

			if(tailSwitch)     // 尾迹开关打开        当余晖衰减到门限值时，对其做尾迹标志，以后的衰减按尾迹衰减规律来进行，直到下一个较大的目标将其覆盖
			{
				if(layerPPI[y][x].tailMark)     // 若是一个点为尾迹点，则其按尾迹衰减规律进行衰减
				{
					layerPPI[y][x].first = decayData(v, tailDecay);	
					colorsPPI[np] = colortableTail[layerPPI[y][x].first];   // 所需绘制到后页FBO中的点的颜色值
					colorsWin[np] = colortableTail[layerPPI[y][x].first];
				}
				else                           // 若是此点不为尾迹点
				{
					if((layerPPI[y][x].first > tailGate) && (decayData(v, yhDecay) < tailGate))      // 当余晖衰减到尾迹门限值时
					{
						layerPPI[y][x].tailMark = true;
						layerPPI[y][x].first = tailGate;
						colorsPPI[np] = colortableTail[layerPPI[y][x].first];   // 所需绘制到后页FBO中的点的颜色值
						colorsWin[np] = colortableTail[layerPPI[y][x].first];
					}
					else              //则其按正常余晖衰减规律进行衰减
					{
						layerPPI[y][x].first = decayData(v, yhDecay);
						colorsPPI[np] = colortablePPI[layerPPI[y][x].first];   // 所需绘制到后页FBO中的点的颜色值
						colorsWin[np] = colortableWin[layerPPI[y][x].first];
					}
				}
			}
			else             // 尾迹开关关闭         只按照余晖衰减规律进行衰减
			{
				layerPPI[y][x].first = decayData(v, yhDecay);
				colorsPPI[np] = colortablePPI[layerPPI[y][x].first];   // 所需绘制到后页FBO中的点的颜色值
				colorsWin[np] = colortableWin[layerPPI[y][x].first];
			}
			np++;
		}
	}

	pGraphicsManager -> drawPoints(winth, 2, vertices, colorsPPI, yhLen);        //在PPI窗口的后页FBO中绘制衰减之后的点
	pGraphicsManager -> drawPoints(winth + 1, 2, vertices, colorsWin, yhLen);        //在Win窗口的后页FBO中绘制衰减之后的点

	yhIndex = yhIndex + yhLen;        //记录余辉表的位置
	if(yhIndex >= RND_LEN)             //超过随机余辉数据总长度RND_LEN之后，置0
	{
		yhIndex = 0;
	}

	return 0;
}



CYH::~CYH()
{

}