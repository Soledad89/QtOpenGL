

//******************************
//
//  author : zhaolimin, lishidan
//
//  create time: 2007-9-5
//
//  last modify time:2007-10-16
//
//  Version: 1.3
//
//******************************

#ifndef _DATAPOOL_H_
#define _DATAPOOL_H_

#include "ppidef.h"

class CDataPool
{
public:
	//----����ɨ��任��-----
    int** addr_near_x;
    int** addr_near_y;
    int** addr_far_x;
    int** addr_far_y;
    
	//--------��Ա�---------
    int* randomx;
    int* randomy;

	//--------��ɫ��---------
	DISPMEMTYPE* colortablePPI[2];    // PPI��ʾ��ɫ��
	DISPMEMTYPE* colortableWin[2];    // С������ʾ��ɫ��
	DISPMEMTYPE* colortableTail[2];   // PPI��ʾ��ɫ��
	//DISPMEMTYPE colorAR;           // AR�ߵ���ɫ


    //-----��Ƶ֡������------
    s_layer_unit** layerPPI[2];          //ÿ������ӵ���Լ���һ��layer��CPU���ڴ棩����layer�洢�����ݺ���ʾƵ�ϵ������Ƕ�Ӧ�ģ����ڱȴ�д��
	s_layer_unit** layerWin[2];
	s_layer_unit** layerAR[2];


public:
    CDataPool();
    ~CDataPool();
	int setColorTable(DISPMEMTYPE* colortable, unsigned int theColor);
	int setColorTable(DISPMEMTYPE* colortable, unsigned int* colorTable);

private:
    int loadDataTable();     //�������ű�����ɨ��任������������

};

#endif

