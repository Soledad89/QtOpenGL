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
	winth = 3 * videoChannel;        // �ֱ��Ӧ��3�͵�6������

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


//�������ܣ� ����ֵС��theGate�����ݲ���ʾ
int CAR::ARSetGate(unsigned char theGate)
{
	base = theGate;

	return 0;
}


//�������ܣ�����AR������
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


//�������ܣ�����AR����˥���ĳ̶�
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

//�������ܣ���AR�����е��״����ݲ㣬�رյ�ʱ���״����ݲ㱻���������Ҳ��ٻ���
int CAR::ARSetSwitch(short theSwitch)
{
	isOpen = theSwitch;	

	return 0;
}


//�������ܣ��Ƿ񶳽�AR�����е��״����ݲ㣬����������̨Ҳ���ٻ���
int CAR::ARSetFrost(char theFrost)
{
	frost = theFrost;

	return 0;
}


//�������ܣ�����AR�ߵ���ɫ
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

//�������ܣ�����AR�״����ݲ�ı�����ɫ
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


//�������ܣ����ƴ��ڵ��ӡ��ر�ʱ���������������ڹر�״̬ʱ��PPI��ҳ��������ƣ������ӡ�
int CAR::ARSetVisible(int surfth, short theSwitch)
{
	visibleAR[surfth - 1] = theSwitch;
	return 0;
}


//�������ܣ���dispdat�е����ݻ��Ƶ�AR���ڵĺ�ҳFBO��
int CAR::ARDraw(s_ppi_video_for_disp* dispdat)
{
	unsigned char v;

	if(isOpen && (!frost))       // ֻ���״����ݲ㱻�򿪣��Ҳ������������£���̨�Żử
	{	
		for(int i = 0; i < CNTVIDEO; i ++)
		{
			v = dispdat->videodata[i];
			video_data[i]  = (v > base) ? v : base;
		}

		int c = 10;
		int numPoints = dispRadius/c;
		float percent = float(range) / PPIRange;
		//��ȡ����ʾ�������ֵ����ɫֵ
		for(int i = 0; i < numPoints; i ++)
		{
			vertices[2 * i] = float(i) / numPoints * SURF_PPI_WIDTH  ;          // ������Ƶ���ҳFBO�еĵ������ֵ     AR���ں�ҳ�Ĵ�СΪ��1200 * 1200
			vertices[2 * i + 1] = float(video_data[int(c /2 * i * percent)]) / 256 * SURF_PPI_HEIGHT;

			colors[i] = colorAR;       //AR���ڵ��״����ݲ�����ײ�
		}

		pGraphicsManager -> drawLines(winth, vertices, colors, numPoints);    // ���״������߻��Ƶ�AR���ں�ҳFBO�ĵ�2����

	}

	return 0;
}



//�������ܣ� ���AR������һ�����������Ļ
int CAR::ARClear(int surfth)       // surfth: 1, 2
{
	if((surfth != 1) && (surfth != 2)&&(surfth != 3))
	{
		return -1;
	}
	
	pGraphicsManager -> clear(winth, surfth, backColor);       // AR����1�����û����㣬��2�����״����ݲ�
	return 0;
}


//�������ܣ� ��AR��ҳFBO�����Ƶ�ͼ������Windows��Ļ�ϣ�ͼ��1��ͼ��2���е���
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
		pGraphicsManager -> copyTexFBO(winth, 2, winth, 3);           // ˥��
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 2);           // �״����ݲ�
	}
	if(visibleAR[0])
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 1);           // �û���
	
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
