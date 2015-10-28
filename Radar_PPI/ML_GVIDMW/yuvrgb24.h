/**************************************
File: yuvrgb24.h
Description: header file for yuvrgb24.c
Date: Nov. 4, 2002
*****************************************/


#include "stdafx.h"
#include "atlstr.h"
#ifndef _YUVRGB24_H_
#define _YUVRGB24_H_
#define CharClip(x) ((unsigned int) x <= 255 ?x : (x < 0 ? 0: 255))

void init_dither_tab();
//YUV图像转化为RGB图像时宽度高度都不变 

//src:YUV数据，dst_ori：RGB缓存区，revers：图像是否翻转
void YUV420toRGB(
 unsigned char *src, 
 unsigned char *dst_ori, 
 int width, 
 int height,bool revers=false);

//src:YUV数据，dst_ori：BGR缓存区，revers：图像是否翻转
void YUV420toBGR(
 unsigned char *src, 
 unsigned char *dst_ori, 
 int width, 
 int height,bool revers);

bool  RGB2YUV420(unsigned char * RgbBuf,
	int nWidth,
	int nHeight,
	unsigned char * yuvBuf,
	unsigned long *len);
#endif
