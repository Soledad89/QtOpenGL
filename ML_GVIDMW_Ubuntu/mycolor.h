

#ifndef __COLOR_H__
#define __COLOR_H__

#include "ppidef.h"

#define GET32R(c) ((c>>24) & 0x0ff)
#define GET32G(c) ((c>>16) & 0x0ff)
#define GET32B(c) ((c>>8) & 0x0ff)

#define GET16R(c) ((c>>8)&0x0f8)
#define GET16G(c) ((c>>3)&0x0fc)
#define GET16B(c) ((c<<3)&0x0f8)

 /* global using with reference to the system color bits depth*/
//----
//#define  REFC(r,g,b) REFC32(r,g,b)
#define csectocmem(c) c32toc32(c)
#define cmemtocsec(c) c<<8
//----

#define REFC16(r,g,b) (unsigned short)( ( (b & 0x000000f8) >> 3 ) | ( (g & 0x000000fc) << 3 ) | ( (r & 0x000000f8) << 8 ) )
//#define REFC32(r,g,b) (unsigned int)( ((r&0xff)<<24) | ((g&0xff)<<16) | ((b&0xff)<<8) )
#define REFC32(r,g,b) (unsigned int)( ((r&0xff)<<16) | ((g&0xff)<<8) | (b&0xff) )
//#define REFC(r,g,b,a) (unsigned int)( ((r&0xff)<<24) | ((g&0xff)<<16) | ((b&0xff)<<8) | (a&0xff) )
#define REFC(r,g,b,a) (unsigned int)( ((a&0xff)<<24) | ((b&0xff)<<16) | ((g&0xff)<<8) | (r&0xff) )     //´Ó¸ßµ½µÍ£ºABGR



/*
the output 16 bits color format is  r-g-b:5-6-5
the output 32 bits color format is  a-r-g-b:8-8-8-8, (a=0), is the same with video memory format

the input 16 bits color format is  r-g-b:5-6-5
the input 32 bits color format is  r-g-b-a:8-8-8-8, (a=0), is the same with SecGen color format
*/
void hsv_to_rgb(float h, float s, float v, int *r, int *g, int *b);
void rgb_to_hsv(int r, int g, int b, float *h, float *s, float *v);



//32bits color define
const unsigned int MFB_RED = 0xff0000;
const unsigned int MFB_GREEN = 0x00ff00;
const unsigned int MFB_BLUE = 0x0000ff;
const unsigned int MFB_YELLOW =0xffff00;
const unsigned int MFB_WHITE = 0xffffff;
const unsigned int MFB_BLACK = 0x0;

//clear color
const unsigned int MFB_CLEAR=0x0;


typedef unsigned int MFB_COLORTYPE;


#endif