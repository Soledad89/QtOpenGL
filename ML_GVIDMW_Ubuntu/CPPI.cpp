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
	
	video_data = new VIDEODATATYPE[CNTVIDEO];       //���ڴ洢һ����Ƶ����
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

// �������ܣ�������յ��İ�������Ŀ�ﵽһ���̶�ʱ��������Ƶ���ҳFBO��
int CPPI::PPIDraw(s_ppi_video_for_disp* dispdat)
{
	PPIPackProc(dispdat);
	numPack++;
	if(numPack == NUM_PACK)
	{		
		pGraphicsManager -> drawPoints(winth, 2, vertices, colorsPPI, np);   // ���״������߻��Ƶ�PPI���ڵĺ�ҳFBO��
		pGraphicsManager -> drawPoints(winth + 1, 2, vertices, colorsWin, np);   // ���״������߻��Ƶ�Win���ڵĺ�ҳFBO��
		numPack = 0;
		np = 0;
	}

	return 0;
}


//�������ܣ������ݰ�dispdat�е����ݻ��Ƶ���ҳFBO�У�����������Կ����Ƕ���Ľӿ�
int CPPI::PPIPackProc(s_ppi_video_for_disp* dispdat)
{
	int ta;//��ͼ��λ
	int j;
	unsigned char v;

	if(isOpen && (!frost))       // PPI��ʱ����
	{
		for(int i = 0; i < CNTVIDEO; i ++)
		{
			v = dispdat->videodata[i];
			video_data[i]  = (v > base) ? v : 0;
		}

		switch(dispMode)        // 0��P��   1��B��    2��E��
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
	

	// ����������Ƶ���ҳFBO�еĸ��������ֵ����ɫֵ�����������µ����ݴ洢��layerPPI�У��洢��Ŀ����Ϊ����һ�εıȴ�д��
	
	if(dispRadius <= ADDR_NEAR_SMP_LEN)             // Case 1�� ��Ҫ��ʾ���״����ݸ���С�ڵ���512,ֻ�н�����Ҫ����
	{
		for(int i = 0; i < dispRadius; i ++)
		{
 			x = xorg + addr_near_x[azicnt][i];              //ֱ������ֵ
			y = yorg + addr_near_y[azicnt][i];

			scanLineVertices[2 * line] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
			scanLineVertices[2 *line + 1] = y;	
			line ++;
		
			if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
			{
				v = video_data[i];      //һ��������ǿ�ȣ���Ӧ��Ļ�����ص����ɫֵ

				if(v > layerPPI[y][x].first)     //�ȴ�д�룺����һ��������µ���Ƶ����ֵ����֮ǰ������ֵ���͸��£����򲻸��£��ŵ㣺����������Ŀ�꣬ȱ�㣺ʧȥʵʱ�ԣ���ʱ���λ�õĴ�Ŀ���Ѿ���������
				{
					if(layerPPI[y][x].tailMark)    // ���˵���β���㣬�����ᱻ���ǣ�ȥ����β����־
					{
						layerPPI[y][x].tailMark = false;
					}
					layerPPI[y][x].first = v;                     // ԭʼ�״�����ֵ[0,255]
					
					vertices[2 * np] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
					vertices[2 * np + 1] = y;				
					colorsPPI[np] = colortablePPI[v];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
					colorsWin[np] = colortableWin[v];
					np++;
				}
			}
		}
	}
	else                       // Case 2�� ��Ҫ��ʾ���״����ݸ�������512����Ϊ������Զ��
	{
		for(int i = 0; i < ADDR_NEAR_SMP_LEN; i ++)         //���ƽ���
		{
 			x = xorg + addr_near_x[azicnt][i];              //FBO���ջ�������ֵ��PPI�����ĵ���(xorg,yorg)����ҳFBO������ϵͳ�ǣ�0,1200������0,1200��
			y = yorg + addr_near_y[azicnt][i];          // note this is minus	

			scanLineVertices[2 * line] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
			scanLineVertices[2 * line + 1] = y;	
			line ++;
		
			if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
			{
				v = video_data[i];      //һ��������ǿ�ȣ���Ӧ��Ļ�����ص����ɫֵ
		
				if(v > layerPPI[y][x].first)     //�ȴ�д�룺����һ��������µ���Ƶ����ֵ����֮ǰ������ֵ���͸��£����򲻸��£��ŵ㣺����������Ŀ�꣬ȱ�㣺ʧȥʵʱ�ԣ���ʱ���λ�õĴ�Ŀ���Ѿ���������
				{
					if(layerPPI[y][x].tailMark)
					{
						layerPPI[y][x].tailMark = false;
					}
					layerPPI[y][x].first = v;                     // ԭʼ�״�����ֵ[0,255]

					vertices[2 * np] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
					vertices[2 * np + 1] = y;
					colorsPPI[np] = colortablePPI[v];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
					colorsWin[np] = colortableWin[v];
					np++;
				}
			}
		}
		for(int i = ADDR_NEAR_SMP_LEN; i < dispRadius; i ++)     //����Զ����Զ����һ������azicnt��Ҫ���������ߣ�azicnt = 0 ~ 4095
		{
            x = xorg + addr_far_x[2*(azicnt)][i-ADDR_NEAR_SMP_LEN];        //��һ���������ֵ
            y = yorg + addr_far_y[2*(azicnt)][i-ADDR_NEAR_SMP_LEN];

			scanLineVertices[2 * line] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
			scanLineVertices[2 * line + 1] = y;	
			line ++;

			if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
			{	
				v = video_data[i]; 
				if((v>layerPPI[y][x].first)&&(v>base))     //�ȴ�д���һ�������
				{
					if(layerPPI[y][x].tailMark)
					{
						layerPPI[y][x].tailMark = false;
					}
					layerPPI[y][x].first = v;                  // ԭʼ�״�����ֵ[0,255]

					vertices[2 * np] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
					vertices[2 * np + 1] = y;
					colorsPPI[np] = colortablePPI[v];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
					colorsWin[np] = colortableWin[v];
					np++;
				}
			}

            x = xorg + addr_far_x[2*(azicnt)+1][i-ADDR_NEAR_SMP_LEN];        //�ڶ����������ֵ
            y = yorg + addr_far_y[2*(azicnt)+1][i-ADDR_NEAR_SMP_LEN];

			scanLineVertices[2 * line] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
			scanLineVertices[2 * line + 1] = y;	
			line ++;
			
			if((x >= 0) && (x < SURF_PPI_WIDTH) && (y >= 0) && (y < SURF_PPI_HEIGHT))
			{
				v = video_data[i]; 
				if((v>layerPPI[y][x].first)&&(v>base))     //�ȴ�д��ڶ��������
				{
					if(layerPPI[y][x].tailMark)
					{
						layerPPI[y][x].tailMark = false;
					}
					layerPPI[y][x].first = v;                  // ԭʼ�״�����ֵ[0,255]

					vertices[2 * np] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
					vertices[2 * np + 1] = y;
					colorsPPI[np] = colortablePPI[v];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
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
			if((v>layerPPI[y][x].first)&&(v>base))     //�ȴ�д��
			{
				if(layerPPI[y][x].tailMark)
				{
					layerPPI[y][x].tailMark = false;
				}
				layerPPI[y][x].first = v;                  // ԭʼ�״�����ֵ[0,255]

				vertices[2 * np] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
				vertices[2 * np + 1] = y;
				colorsPPI[np] = colortablePPI[v];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
				colorsWin[np] = colortableWin[v];
				np++;
			}
		}
	}

	return 0;
}


//�������ܣ����״���������E�Եķ�ʽ���Ƶ���ҳFBO��
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
			if((v>layerPPI[y][x].first)&&(v>base))     //�ȴ�д��
			{
				if(layerPPI[y][x].tailMark)
				{
					layerPPI[y][x].tailMark = false;
				}
				layerPPI[y][x].first = v;                  // ԭʼ�״�����ֵ[0,255]
				
				vertices[2 * np] = x;          // ������Ƶ���ҳFBO�еĵ������ֵ			
				vertices[2 * np + 1] = y;
				colorsPPI[np] = colortablePPI[v];   // ������Ƶ���ҳFBO�еĵ����ɫֵ
				colorsWin[np] = colortableWin[v];
				np++;
			}
		}
	}

	return 0;
}


// �������ܣ����Ʊ�����ͼ
int CPPI::PPIDrawBackMap(BMPImageRec* pBackMap)
{
	pGraphicsManager -> drawBackMap(winth, pBackMap);

	return 0;
}


//�������ܣ� ���PPI������һ�����������Ļ
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


//�������ܣ�����PPI����ʾģʽ
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


//�������ܣ�����P�Ե�����λ��
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


//�������ܣ����ý�����λ
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



//�������ܣ������Ƿ���ʾPPIɨ����
int CPPI::PPISetScan(int theScan)
{
	scanLine = 1 - theScan;

	return 0;
}


//�������ܣ�����PPIɨ���ߵ���ɫ
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
		scanLineColor[i] = REFC(r,g,b,a);         //��RGBAת����AGBR��OpenGL�ڶ����ݵ�ʱ�򣬰���ַ��С�����˳����RGBARGBA�ڶ�
	}
	
	return 0;
}

int CPPI::PPISetMoveMode(short theMode)
{
	moveMode = theMode;
	return 0;
}



//�������ܣ���PPI�״����ݲ�
int CPPI::PPISetSwitch(short theSwitch)
{
	isOpen = theSwitch;
	return 0;
}


// �������ܣ� ������·�״�����ͬ������ʾ
int CPPI::PPISetTogether(short theSwitch)
{
	comdisp = theSwitch;
	return 0;
}



//�������ܣ������Ƿ񶳽�PPI��Ƶ�������ʱ�����ݻ���
int CPPI::PPISetFrost(char theFrost)
{
	frost = theFrost;
	return 0;
}


//�������ܣ�����PPI����
int CPPI::PPISetRange(short theDist)
{
	range = theDist;

	return 0;
}


//�������ܣ�����PPI�ı�����ɫ
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


//�������ܣ�����PPI����ֵ��ֻ�д���һ������ֵ�����ݲŻ���ʾ
int CPPI::PPISetGate(unsigned char theGate)
{
	base = theGate;

	return 0;
}


//�������ܣ����ƴ��ڵ��ӡ��ر�ʱ���������������ڹر�״̬ʱ��PPI��ҳ��������ƣ������ӡ�
int CPPI::PPISetVisible(int winth, int surfth, short theSwitch)
{
	if(winth == 1)
		visiblePPI[surfth - 1] = theSwitch;
	else
		visibleWin[surfth - 1] = theSwitch;
	
	return 0;
}



//�������ܣ� ��PPI��ҳFBO�����Ƶ�ͼ������Windows��Ļ�ϣ�ͼ��1��ͼ��2�Լ�ͼ��3���е���
//int CPPI::PPIUpdate()
//{
//	//pGraphicsManager -> clearWin(winth, 0x01010101); 
//
//	if(visiblePPI[2])
//		pGraphicsManager -> copyTex(winth, winth, 3);           // ������ͼ��
//
//	if(visiblePPI[1])
//	{
//		if(comdisp)
//		{
//			pGraphicsManager -> copyTex(winth, winth+3, 2);           // �״����ݲ�
//		}
//		pGraphicsManager -> copyTex(winth, winth, 2);           // �״����ݲ�
//
//		if(scanLine)
//			pGraphicsManager -> drawWinPoints(winth, vertices, scanLineColor, CNTVIDEO); 
//	}
//
//	if(visiblePPI[0])
//		pGraphicsManager -> copyTex(winth, winth, 1);           // �û���
//
//	pGraphicsManager -> update(winth);
//
//	return 0;
//}


int CPPI::PPIUpdate()
{
	//pGraphicsManager -> clearWin(winth, 0x01010101); 

	if(visiblePPI[2])
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 3);           // ������ͼ��

	if(visiblePPI[1])
	{
		if(comdisp)
		{
			pGraphicsManager -> copyTexFBO(winth, 4, winth+3, 2);           // �״����ݲ�
		}
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 2);           // �״����ݲ�

		if(scanLine)
			pGraphicsManager -> drawPoints(winth, 4, scanLineVertices, scanLineColor, CNTVIDEO); 
	}

	if(visiblePPI[0])
		pGraphicsManager -> copyTexFBO(winth, 4, winth, 1);           // �û���
	
	//pGraphicsManager -> update(winth);


	pGraphicsManager -> copyTex(winth, winth, 4);
	pGraphicsManager -> update(winth);

	return 0;
}




// �������ܣ������״���ƵС���ھ���ͷ�λ�����ݾ���ͷ�λ����ȷ��Ҫ���Ĵ��ڵ�����
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


// �������ܣ�������ƵС���ڵķŴ���
int CPPI::WinSetScale(int theScale)
{
	if(theScale!=scaleWin)
	{
		WinClear(2);
		scaleWin = theScale;
	}
	
	return 0;
}


// �������ܣ�����С�������״����ݲ�ı�����ɫ
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


//�������ܣ� ���С������һ�����������Ļ
int CPPI::WinClear(int surfth) 
{
	pGraphicsManager -> clear(winth + 1, surfth, backColorWin);

	return 0;
}



//�������ܣ� ��PPI��ҳFBO�����Ƶ�һ����ͼ������Windows��Ļ�ϣ�ͼ��1��ͼ��2���е���
int CPPI::WinUpdate()
{
	int R = float(distanceWin) / range * dispRadius;
	int Azi = directionWin / 360 * AZI_NUM;

	float theX_Center = float(xorg + addr_near_x[Azi][R]) / SURF_PPI_WIDTH;  
	float theY_Center = float(yorg + addr_near_y[Azi][R]) / SURF_PPI_HEIGHT;  

	float theWidth = float(winWidth[winth]) / (winWidth[winth - 1] * scaleWin);         // wMin / (wPPI * scale)
	float theHeight = float(winHeight[winth]) / (winHeight[winth - 1] * scaleWin); 

	if(visibleWin[1])                                        // �״����ݲ�
	{
		if(comdisp)
			pGraphicsManager -> copyTexRectFBO(winth+1, 3, winth+4, 2, theX_Center - theWidth/2, theY_Center - theHeight/2, theWidth, theHeight);           // �״����ݲ�   ���½�λ�ã�(theX, theY)�� ��theWidth�� �ߣ�theHeight
		pGraphicsManager -> copyTexRectFBO(winth+1, 3, winth, 3, theX_Center - theWidth/2, theY_Center - theHeight/2, theWidth, theHeight); 
		pGraphicsManager -> copyTexRectFBO(winth+1, 3, winth+1, 2, theX_Center - theWidth/2, theY_Center - theHeight/2, theWidth, theHeight);           // �״����ݲ�   ���½�λ�ã�(theX, theY)�� ��theWidth�� �ߣ�theHeight
	}		
	if(visibleWin[0])
		pGraphicsManager -> copyTexFBO(winth+1, 3, winth+1, 1);           // �û���

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
