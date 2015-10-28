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

	yhDecay = 65;      // ÿ�θ���ʱ������˥���ĳ̶�
	tailGate = 200;
	tailSwitch = 1;
	tailDecay = 30;     // β������������ʱ��˥���ĳ̶�

	//yhDecay = 0;
	//tailGate = 0;
	//tailSwitch = 0;
	//tailDecay = 0;

	pGraphicsManager = pGM;
	vertices = new int[2 * yhLen];
	colorsPPI = new unsigned int[yhLen];
	colorsWin = new unsigned int[yhLen];

}


//�������ܣ�����PPI���ͳ̶ȣ���theTail = 0ʱ������������˥��
int CYH::setPPIDecay(int theTail)
{
	yhDecay = theTail;

	return 0;
}


//�������ܣ�����PPIβ������
int CYH::setPPITailSwitch(short theSwitch)
{
	tailSwitch = theSwitch;

	return 0;
}


//�������ܣ�����PPIβ������
int CYH::setPPITailGate(unsigned char theGate)
{
	tailGate = theGate;

	return 0;
}


//�������ܣ�����PPIβ��ʱ��
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


// �������ܣ������ݽ���˥��������˥�����С��0��������Ϊ0
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
	
	//��ȡ�������˥���ĵ������λ�ú���ɫֵ
	for(int i = 0; i < yhLen; i++)      // yhLen = 30000     RND_LEN = 1440000   ÿ��һ����ԣ�����RND_LEN���������������ѡȡyhLen�����ݽ��д���
	{
		x = randomx[yhIndex + i];      //��ȡ��Ҫ������Դ���ĵ������λ�ã�yh_index��¼�ϴδ�����λ��
		y = randomy[yhIndex + i];     
			
		//��һ���������״�����ֵ����˥�����õ�˥��֮������ֵ����Ӧ����ɫֵ
		if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))      //(x,y)�ں�ҳFBO����ϵͳ��0��1200������0��1200����      ����Ա��м�¼����ʲô�أ����ǵ���ֵ���ܻᳬ��1200��
        {
			vertices[2 * np] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
			vertices[2 * np + 1] = y;

			v = layerPPI[y][x].first;      // ��������

			if(tailSwitch)     // β�����ش�        ������˥��������ֵʱ��������β����־���Ժ��˥����β��˥�����������У�ֱ����һ���ϴ��Ŀ�꽫�串��
			{
				if(layerPPI[y][x].tailMark)     // ����һ����Ϊβ���㣬���䰴β��˥�����ɽ���˥��
				{
					layerPPI[y][x].first = decayData(v, tailDecay);	
					colorsPPI[np] = colortableTail[layerPPI[y][x].first];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
					colorsWin[np] = colortableTail[layerPPI[y][x].first];
				}
				else                           // ���Ǵ˵㲻Ϊβ����
				{
					if((layerPPI[y][x].first > tailGate) && (decayData(v, yhDecay) < tailGate))      // ������˥����β������ֵʱ
					{
						layerPPI[y][x].tailMark = true;
						layerPPI[y][x].first = tailGate;
						colorsPPI[np] = colortableTail[layerPPI[y][x].first];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
						colorsWin[np] = colortableTail[layerPPI[y][x].first];
					}
					else              //���䰴��������˥�����ɽ���˥��
					{
						layerPPI[y][x].first = decayData(v, yhDecay);
						colorsPPI[np] = colortablePPI[layerPPI[y][x].first];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
						colorsWin[np] = colortableWin[layerPPI[y][x].first];
					}
				}
			}
			else             // β�����عر�         ֻ��������˥�����ɽ���˥��
			{
				layerPPI[y][x].first = decayData(v, yhDecay);
				colorsPPI[np] = colortablePPI[layerPPI[y][x].first];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
				colorsWin[np] = colortableWin[layerPPI[y][x].first];
			}
			np++;
		}
	}

	pGraphicsManager -> drawPoints(winth, 2, vertices, colorsPPI, yhLen);        //��PPI���ڵĺ�ҳFBO�л���˥��֮��ĵ�
	pGraphicsManager -> drawPoints(winth + 1, 2, vertices, colorsWin, yhLen);        //��Win���ڵĺ�ҳFBO�л���˥��֮��ĵ�

	yhIndex = yhIndex + yhLen;        //��¼��Ա��λ��
	if(yhIndex >= RND_LEN)             //���������������ܳ���RND_LEN֮����0
	{
		yhIndex = 0;
	}

	return 0;
}



CYH::~CYH()
{

}