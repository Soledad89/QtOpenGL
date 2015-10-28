// Name:   CDataPool.cpp
// Author: Yan Guo
// Date:   Mar 8, 2013 in the canteen of Rohm Building


// 文件功能描述： 为其他文件提供数据，主要包括3张表
//   
//     1. 坐标扫描变换表： 外界所传入数据包中的雷达数据，是以极坐标的形式来描述的，在显卡中绘制图形，是根据直角坐标系下的坐标值来确定位置的
//     2. 余晖随机表： 在做余晖衰减时，本来对每一点的亮度值都需要进行衰减，但是由于在一个余晖刷新间隔内，无法完成这个处理，所以这里采用分块完成刷新，每一块都是随机地选择一部分点
//     3. 颜色表： 



#include "CDataPool.h"
#include "mycolor.h"
#include "ppidef.h"



CDataPool::CDataPool()
{
	//-------坐标扫描变换表--------
    addr_near_x = new int*[ADDR_NEAR_AZI_LEN];      //近区
    addr_near_y = new int*[ADDR_NEAR_AZI_LEN];
    for( int i=0; i<ADDR_NEAR_AZI_LEN; i++ )
    {
        addr_near_x[i] = new int[ADDR_NEAR_SMP_LEN];
        addr_near_y[i] = new int[ADDR_NEAR_SMP_LEN];
    }
    addr_far_x = new int*[ADDR_FAR_AZI_LEN];        //远区
    addr_far_y = new int*[ADDR_FAR_AZI_LEN];
    for( int i=0; i<ADDR_FAR_AZI_LEN; i++ )
    {
        addr_far_x[i] = new int[ADDR_FAR_SMP_LEN];
        addr_far_y[i] = new int[ADDR_FAR_SMP_LEN];
    }


	//---------余辉随机表-----------
    randomx = new int[RND_LEN]; 
    randomy = new int[RND_LEN];

    if(loadDataTable()!=0)
    {
        printf("load addr table error\n");
        exit(-1);
    }


	//-------------颜色表------------
	for(int i = 0; i < 2; i ++)
	{
		colortablePPI[i] = new DISPMEMTYPE[256];
		setColorTable(colortablePPI[i], 0xffff00ff);             //黄色

		colortableWin[i] = new DISPMEMTYPE[256];
		setColorTable(colortableWin[i], 0xffff007f);             //黄色

		colortableTail[i] = new DISPMEMTYPE[256];
		setColorTable(colortableTail[i], 0xff0000ff);            //红色
	}
		//setColorTable(colortablePPI[1], 0x00000ff7f);             //蓝色


	//colorAR = 0xffff00ff;


	//--------视频帧缓冲区----------
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


// 函数功能： 上载两张表，坐标扫描变换表、余晖随机表
int CDataPool::loadDataTable()
{
    FILE *fp;
	int i;

	if( (fp=fopen("./dat/sysdat","rb"))==NULL )        // 系统数据文件
	{
		printf("read ./dat/sysdat error!\n");
		return -1;
	}

	//-------坐标扫描变换表--------
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

	//---------余辉随机表-----------
	fseek(fp,datPos[4],SEEK_SET);
	fread(randomx,sizeof(int),RND_LEN,fp);         // 从系统数据文件sysdat中读入余辉随机表，其总长度为： RND_LEN
	fseek(fp,datPos[5],SEEK_SET);
	fread(randomy,sizeof(int),RND_LEN,fp);

	fclose(fp);

    return 0;
}


//函数功能：设置显示颜色表
int CDataPool::setColorTable(DISPMEMTYPE* colortable, unsigned int theColor)    // RGBA
{                                                                                    
	float h,s,l;                 // 输入颜色theColor的格式从高字节到低字节的顺序为： RGBA                                                    
	int r, g, b, a;              // OpenGL在解码时，从低地址到高地址的顺序，是按RGBA进行解码，因此，在地址从 int -> char 的强制转换中要注意
                                 // 计算机存储整型数据的顺序是：低字节在低地址，高字节在高地址，因而在计算机中，从低地址到高地址的顺序存储的是：ABGR
	r = theColor >> 24;
	g = (theColor >> 16) & 0xff;
	b = (theColor >> 8) & 0xff;
	a = 0xff;

	rgb_to_hsv(r,g,b,&h,&s,&l);

	for(int i=0;i<256;i++)
	{
		l = i/255.0;                        // 将亮度值进行等分

		hsv_to_rgb(h,s,l,&r,&g,&b);

		colortable[i] = REFC(r,g,b,a);      // 这里，colortablePPI中得到的颜色点，按地址从低到高的顺序是：RGBA
	}
	colortable[0] = REFC(0,0,0,0);

	return 0;
}


//函数功能：设置显示颜色表
int CDataPool::setColorTable(DISPMEMTYPE* colortable, unsigned int* colorTable)
{
		for(int i=1;i<256;i++)
		{
			int r, g, b, a;              // OpenGL在解码时，从低地址到高地址的顺序，是按RGBA进行解码，因此，在地址从 int -> char 的强制转换中要注意
	                                 // 计算机存储整型数据的顺序是：低字节在低地址，高字节在高地址，因而在计算机中，从低地址到高地址的顺序存储的是：ABGR
			r = colorTable[i] >> 24;
			g = (colorTable[i] >> 16) & 0xff;
			b = (colorTable[i] >> 8) & 0xff;
			a = 0xff;

			colortable[i] = REFC(r,g,b,a);
		}
//	memcpy(colortable, colorTable, 256);
	return 0;
}



//函数功能：释放两张表和两块视频帧缓冲区
CDataPool::~CDataPool()
{
	//---------坐标扫描变换表-----------
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


	//------------余辉随机表------------
    delete[] randomx;
    delete[] randomy;


	//-------------颜色表------------
	for(int i = 0; i < 2; i ++)
	{
		delete[] colortablePPI[i];
		delete[] colortableWin[i];
		delete[] colortablePPI[i];
	}

	

	//-----------视频帧缓冲区------------
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
