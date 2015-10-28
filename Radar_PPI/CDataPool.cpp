// Name:   CDataPool.cpp
// Author: Yan Guo
// Date:   Mar 8, 2013 in the canteen of Rohm Building


// �ļ����������� Ϊ�����ļ��ṩ���ݣ���Ҫ����3�ű�
//   
//     1. ����ɨ��任�� ������������ݰ��е��״����ݣ����Լ��������ʽ�������ģ����Կ��л���ͼ�Σ��Ǹ���ֱ������ϵ�µ�����ֵ��ȷ��λ�õ�
//     2. ��������� ��������˥��ʱ��������ÿһ�������ֵ����Ҫ����˥��������������һ������ˢ�¼���ڣ��޷���������������������÷ֿ����ˢ�£�ÿһ�鶼�������ѡ��һ���ֵ�
//     3. ��ɫ�� 



#include "CDataPool.h"
#include "mycolor.h"
#include "ppidef.h"



CDataPool::CDataPool()
{
	//-------����ɨ��任��--------
    addr_near_x = new int*[ADDR_NEAR_AZI_LEN];      //����
    addr_near_y = new int*[ADDR_NEAR_AZI_LEN];
    for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )
    {
        addr_near_x[i] = new int[ADDR_NEAR_SMP_LEN];
        addr_near_y[i] = new int[ADDR_NEAR_SMP_LEN];
    }
    addr_far_x = new int*[ADDR_FAR_AZI_LEN];        //Զ��
    addr_far_y = new int*[ADDR_FAR_AZI_LEN];
    for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
    {
        addr_far_x[i] = new int[ADDR_FAR_SMP_LEN];
        addr_far_y[i] = new int[ADDR_FAR_SMP_LEN];
    }


	//---------��������-----------
    randomx = new int[RND_LEN]; 
    randomy = new int[RND_LEN];

    if(loadDataTable()!=0)
    {
        printf("load addr table error\n");
        exit(-1);
    }


	//-------------��ɫ��------------
	for(int i = 0; i < 2; i ++)
	{
		colortablePPI[i] = new DISPMEMTYPE[256];
		setColorTable(colortablePPI[i], 0xffff00ff);             //��ɫ

		colortableWin[i] = new DISPMEMTYPE[256];
		setColorTable(colortableWin[i], 0xffff007f);             //��ɫ

		colortableTail[i] = new DISPMEMTYPE[256];
		setColorTable(colortableTail[i], 0xff0000ff);            //��ɫ
	}
		//setColorTable(colortablePPI[1], 0x00000ff7f);             //��ɫ


	//colorAR = 0xffff00ff;


	//--------��Ƶ֡������----------
	for(int i = 0; i < 2; i ++)
	{
		layerPPI[i] = new s_layer_unit* [SURF_PPI_HEIGHT];
		for(int j = 0; j < SURF_PPI_HEIGHT; j++)
		{
			layerPPI[i][j] = new s_layer_unit [SURF_PPI_WIDTH];
			memset(layerPPI[i][j],0,SURF_PPI_WIDTH*sizeof(s_layer_unit));
		}

		layerWin[i] = new s_layer_unit*[SURF_PPI_HEIGHT];
		for(int j = 0; j < SURF_PPI_HEIGHT; j++)
		{
			layerWin[i][j] = new s_layer_unit [SURF_PPI_WIDTH];
			memset(layerWin[i][j],0,SURF_PPI_WIDTH*sizeof(s_layer_unit));
		}

		layerAR[i] = new s_layer_unit*[SURF_PPI_HEIGHT];
		for(int j = 0; j < SURF_PPI_HEIGHT; j++)
		{
			layerAR[i][j] = new s_layer_unit [SURF_PPI_WIDTH];
			memset(layerAR[i][j],0,SURF_PPI_WIDTH*sizeof(s_layer_unit));
		}
	}

}


// �������ܣ� �������ű�����ɨ��任�����������
int CDataPool::loadDataTable()
{
    FILE *fp;
	int i;

	if( (fp=fopen("./dat/sysdat","rb"))==NULL )        // ϵͳ�����ļ�
	{
		printf("read ./dat/sysdat error!\n");
		return -1;
	}

	//-------����ɨ��任��--------
	fseek(fp,datPos[6],SEEK_SET);
	for(  i=0; i<ADDR_NEAR_AZI_LEN; i++ )
		fread(addr_near_x[i],sizeof(int),ADDR_NEAR_SMP_LEN,fp);       
	fseek(fp,datPos[7],SEEK_SET);
	for(  i=0; i<ADDR_NEAR_AZI_LEN; i++ )
		fread(addr_near_y[i],sizeof(int),ADDR_NEAR_SMP_LEN,fp);
	fseek(fp,datPos[8],SEEK_SET);
	for(  i=0; i<ADDR_FAR_AZI_LEN; i++ )
		fread(addr_far_x[i],sizeof(int),ADDR_FAR_SMP_LEN,fp);
	fseek(fp,datPos[9],SEEK_SET);
	for(  i=0; i<ADDR_FAR_AZI_LEN; i++ )
		fread(addr_far_y[i],sizeof(int),ADDR_FAR_SMP_LEN,fp);

	//---------��������-----------
	fseek(fp,datPos[4],SEEK_SET);
	fread(randomx,sizeof(int),RND_LEN,fp);         // ��ϵͳ�����ļ�sysdat�ж��������������ܳ���Ϊ�� RND_LEN
	fseek(fp,datPos[5],SEEK_SET);
	fread(randomy,sizeof(int),RND_LEN,fp);

	fclose(fp);

    return 0;
}


//�������ܣ�������ʾ��ɫ��
int CDataPool::setColorTable(DISPMEMTYPE* colortable, unsigned int theColor)    // RGBA
{                                                                                    
	float h,s,l;                 // ������ɫtheColor�ĸ�ʽ�Ӹ��ֽڵ����ֽڵ�˳��Ϊ�� RGBA                                                    
	int r, g, b, a;              // OpenGL�ڽ���ʱ���ӵ͵�ַ���ߵ�ַ��˳���ǰ�RGBA���н��룬��ˣ��ڵ�ַ�� int -> char ��ǿ��ת����Ҫע��
                                 // ������洢�������ݵ�˳���ǣ����ֽ��ڵ͵�ַ�����ֽ��ڸߵ�ַ������ڼ�����У��ӵ͵�ַ���ߵ�ַ��˳��洢���ǣ�ABGR
	r = theColor >> 24;
	g = (theColor >> 16) & 0xff;
	b = (theColor >> 8) & 0xff;
	a = 0xff;

	rgb_to_hsv(r,g,b,&h,&s,&l);

	for(int i=0;i<256;i++)
	{
		l = i/255.0;                        // ������ֵ���еȷ�

		hsv_to_rgb(h,s,l,&r,&g,&b);

		colortable[i] = REFC(r,g,b,a);      // ���colortablePPI�еõ�����ɫ�㣬����ַ�ӵ͵��ߵ�˳���ǣ�RGBA
	}
	colortable[0] = REFC(0,0,0,0);

	return 0;
}


//�������ܣ�������ʾ��ɫ��
int CDataPool::setColorTable(DISPMEMTYPE* colortable, unsigned int* colorTable)
{
		for(int i=1;i<256;i++)
		{
			int r, g, b, a;              // OpenGL�ڽ���ʱ���ӵ͵�ַ���ߵ�ַ��˳���ǰ�RGBA���н��룬��ˣ��ڵ�ַ�� int -> char ��ǿ��ת����Ҫע��
	                                 // ������洢�������ݵ�˳���ǣ����ֽ��ڵ͵�ַ�����ֽ��ڸߵ�ַ������ڼ�����У��ӵ͵�ַ���ߵ�ַ��˳��洢���ǣ�ABGR
			r = colorTable[i] >> 24;
			g = (colorTable[i] >> 16) & 0xff;
			b = (colorTable[i] >> 8) & 0xff;
			a = 0xff;

			colortable[i] = REFC(r,g,b,a);
		}
//	memcpy(colortable, colorTable, 256);
	return 0;
}



//�������ܣ��ͷ����ű��������Ƶ֡������
CDataPool::~CDataPool()
{
	//---------����ɨ��任��-----------
    for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )     
    {
        delete[] addr_near_x[i];
        delete[] addr_near_y[i];
    }
    delete[] addr_near_x;
    delete[] addr_near_y;

    for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
    {
        delete[] addr_far_x[i];
        delete[] addr_far_y[i];
    }
    delete[] addr_far_x;
    delete[] addr_far_y;


	//------------��������------------
    delete[] randomx;
    delete[] randomy;


	//-------------��ɫ��------------
	for(int i = 0; i < 2; i ++)
	{
		delete[] colortablePPI[i];
		delete[] colortableWin[i];
		delete[] colortablePPI[i];
	}

	

	//-----------��Ƶ֡������------------
	for(int i = 0; i < 2; i ++)
	{
		for(int j = 0; j < SURF_PPI_HEIGHT; j++)
		{
			delete[] layerPPI[i][j];
		}
		delete layerPPI[i];

		for(int j = 0; j < SURF_PPI_HEIGHT; j++)
		{
			delete[] layerWin[i][j];
		}
		delete layerWin[i];

		for(int j = 0; j < SURF_PPI_HEIGHT; j++)
		{
			delete[] layerAR[i][j];
		}
		delete layerAR[i];
	}

}
