//Name:   MyPackage.cpp
//Data:   Aug 30, 2011
//Author: GuoYan in Dongzhulou Building 10-409 , Tsinghua

//一些值得保存的：函数和数据定义
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "bmp.h"

/****************获取一幅BMP图像的宽、高、数据指针*******************/

BMPImageRec * loadBMP(const char * fileName)
{
	BMPImageRec * pBMPImageRec = NULL;	
	FILE * pFileName;
	GLuint width , height , dataLength , lineLength;

	pFileName = fopen(fileName , "rb");
	if(pFileName == NULL)
	{
		printf("Open file: %s error!\n" , fileName);
		return NULL;
	}
		
	fseek(pFileName , 0x0012 , SEEK_SET);
	fread(&width , 4 , 1 , pFileName);
	fread(&height , 4 , 1 , pFileName);

	lineLength = 3 * width;
	while(lineLength % 4)
	{
		lineLength++;
	}
    dataLength = lineLength * height;

	GLubyte * pData = new GLubyte[dataLength];
	if(pData == NULL)
	{
		printf("Memory allocating for the file: %s error!\n" , fileName);
		return NULL;
	}
	fseek(pFileName , 54 , SEEK_SET);
	for (GLuint i = 0; i < height; i++)
	{
		fread(pData + i*lineLength, 1, 3*width, pFileName);      //读取一行图像的数据
	}

	fclose(pFileName);

	pBMPImageRec = new BMPImageRec;
	if(pBMPImageRec == NULL)
	{
		printf("Memory allocating for the image: %s error!\n" , fileName);
		return NULL;
	}
	pBMPImageRec -> sizeX = width;
	pBMPImageRec -> sizeY = height;
	pBMPImageRec -> data = pData;

	return pBMPImageRec;
}



/********************只获取一幅BMP图像的数据指针******************/
GLubyte * loadBMPData(const char * fileName)  
{
	FILE * pFileName;
	GLuint width , height , dataLength , lineLength;

	pFileName = fopen(fileName , "rb");    //Images/backmap.bmp
	if(pFileName == NULL)
	{
		printf("Open file error!\n");
		return NULL;
	}
	
	fseek(pFileName , 0x0012 , SEEK_SET);
	fread(&width , 4 , 1 , pFileName);
	fread(&height , 4 , 1 , pFileName);
	//printf("width = %d , height = %d\n" , width , height);

	lineLength = width;
	while(lineLength % 4)
	{
		lineLength++;
	}
    dataLength = lineLength * height * 3;

	GLubyte * pData = new GLubyte[dataLength];
	if(pData == NULL)
	{
		printf("Memory allocating for the file: %s error!\n" , fileName);
		return NULL;
	}
	fseek(pFileName , 54 , SEEK_SET);
	fread(pData , 1 , dataLength , pFileName);

	fclose(pFileName);

	return pData;
}

