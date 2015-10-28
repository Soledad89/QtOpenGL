
#include "stdafx.h"
#include "yuvrgb24.h"
/************************************************************************
*
* yuvrgb24.c, colour space conversion for tmndecode (H.263 decoder)
* Copyright (C) 1996 Telenor R&D, Norway
* Karl Olav Lillevold <Karl.Lillevold@nta.no>
************************************************************************/
//#include "config.h"
//#include "tmndec.h"
//#include "global.h"

/* Data for ConvertYUVtoRGB*/

static long int crv_tab[256];
static long int cbu_tab[256];
static long int cgu_tab[256];
static long int cgv_tab[256];
static long int tab_76309[256];
void init_dither_tab()
{
long int crv,cbu,cgu,cgv;
int i,j;
crv = 104597; cbu = 132201; /* fra matrise i global.h */
cgu = 25675; cgv = 53279;
for (i = 0; i < 256; i++)
{
 crv_tab[i] = (i-128) * crv;
 cbu_tab[i] = (i-128) * cbu;
 cgu_tab[i] = (i-128) * cgu;
 cgv_tab[i] = (i-128) * cgv;
 tab_76309[i] = 76309*(i-16);

}
}
/**********************************************************************
*
* Name: ConvertYUVtoRGB 
* Description: Converts YUV image to RGB (packed mode)
* 
* Input: pointer to source luma, Cr, Cb, destination,
* image width and height
* Returns: 
* Side effects:
*
* Date: 951208 Author: Karl.Lillevold@nta.no
*
***********************************************************************/


void YUV420toRGB(
 unsigned char *src, 
 unsigned char *dst_ori, 
 int width, 
 int height,bool revers)
{
long y11;
long y12;
long y21;
long y22;
int u, v; 
int i, j;
long crv, cbu, cgu,cgv;
unsigned long DW; // 4 bytes
unsigned char * id1,*id2; // 4 bytes
unsigned char *py1,*py2, *pu,*pv;

unsigned char r,g,b;
long pix =  width*height;


 
py1 =revers? src+pix-width:src;
pv = revers?src+pix*5/4-width/2:src+pix; 
pu = revers?src+pix*3/2-width/2:src+pix*5/4;
int linel=revers?-width:width;
id1 = dst_ori;
for (j = 0; j < height; j +=2 )
{
 id2 = id1+width*3;
 py2 = py1+linel; 
 for (i = 0; i < width; i += 2 )
 {
	u = *pu;
	v = *pv;
	pu ++;
	pv ++;
	crv=crv_tab[v];cbu=cbu_tab[u]; cgu=cgu_tab[u];cgv=cgv_tab[v];
 
	y11=tab_76309[*py1];
 
	*id1++ = CharClip((y11+crv)>>16);
	*id1++ =CharClip((y11-cgu-cgv)>>16);
	*id1++ =CharClip((y11+cbu)>>16);
	py1 ++;

	y12=tab_76309[*py1];
	*id1++= CharClip((y12+crv)>>16);
	*id1++=CharClip((y12-cgu-cgv)>>16);
	*id1++=CharClip((y12+cbu)>>16);
	py1 ++;

	y21=tab_76309[*py2];
	*id2++ = CharClip((y21+crv)>>16);
	*id2++ =CharClip((y21-cgu-cgv)>>16);
	*id2++ =CharClip((y21+cbu)>>16);
	py2 ++;

	y22=tab_76309[*py2];
	*id2++ = CharClip((y22+crv)>>16);
	*id2++ =CharClip((y22-cgu-cgv)>>16);
	*id2++ =CharClip((y22+cbu)>>16);
	py2 ++;

 }
  pu =revers?pu-width:pu;
  pv =revers?pv-width:pv;
  id1 = id2;
 py1 = revers?py2-2*width:py2; 
} 
}

void YUV420toBGR(
 unsigned char *src, 
 unsigned char *dst_ori, 
 int width, 
 int height,bool revers)
{
long y11;
long y12;
long y21;
long y22;
int u, v; 
int i, j;
long crv, cbu, cgu,cgv;
unsigned long DW; // 4 bytes
unsigned char * id1,*id2; // 4 bytes
unsigned char *py1,*py2, *pu,*pv;

unsigned char r,g,b;
long pix =  width*height;


 
py1 =revers? src+pix-width:src;
pv = revers?src+pix*5/4-width/2:src+pix; 
pu = revers?src+pix*3/2-width/2:src+pix*5/4;
int linel=revers?-width:width;
id1 = dst_ori;
for (j = 0; j < height; j +=2 )
{
 id2 = id1+width*3;
 py2 = py1+linel; 
 for (i = 0; i < width; i += 2 )
 {
	u = *pu;
	v = *pv;
	pu ++;
	pv ++;
	crv=crv_tab[v];cbu=cbu_tab[u]; cgu=cgu_tab[u];cgv=cgv_tab[v];
 
	y11=tab_76309[*py1];
	*id1++ =CharClip((y11+cbu)>>16);
	*id1++ =CharClip((y11-cgu-cgv)>>16);
	*id1++ = CharClip((y11+crv)>>16);	
	py1 ++;

	y12=tab_76309[*py1];	
	*id1++=CharClip((y12+cbu)>>16);
	*id1++=CharClip((y12-cgu-cgv)>>16);
	*id1++= CharClip((y12+crv)>>16);
	py1 ++;

	y21=tab_76309[*py2];
	*id2++ =CharClip((y21+cbu)>>16);
	*id2++ =CharClip((y21-cgu-cgv)>>16);
	*id2++ = CharClip((y21+crv)>>16);
	py2 ++;

	y22=tab_76309[*py2];
	*id2++ =CharClip((y22+cbu)>>16);
	*id2++ =CharClip((y22-cgu-cgv)>>16);
	*id2++ = CharClip((y22+crv)>>16);
	py2 ++;

 }
  pu =revers?pu-width:pu;
  pv =revers?pv-width:pv;
  id1 = id2;
 py1 = revers?py2-2*width:py2; 
} 
}

bool  RGB2YUV420(unsigned char * RgbBuf,int nWidth,int nHeight,unsigned char * yuvBuf,unsigned long *len)
{

    int i, j; 
 unsigned char*bufY, *bufU, *bufV, *bufRGB,*bufYuv; 
 memset(yuvBuf,0,(unsigned int )*len);
 bufY = yuvBuf; 
 bufV = yuvBuf + nWidth * nHeight; 
 bufU = bufV + (nWidth * nHeight* 1/4); 
 *len = 0; 
 unsigned char y, u, v, r, g, b,testu,testv; 
    unsigned int ylen = nWidth * nHeight;
 unsigned int ulen = (nWidth * nHeight)/4;
 unsigned int vlen = (nWidth * nHeight)/4; 
 for (j = 0; j<nHeight;j++)
 {
  bufRGB = RgbBuf + nWidth * (nHeight - 1 - j) * 3 ; 
  for (i = 0;i<nWidth;i++)
  {
   int pos = nWidth * i + j;
   r = *(bufRGB++);
   g = *(bufRGB++);
   b = *(bufRGB++);

  y = (unsigned char)( ( 66 * r + 129 * g +  25 * b + 128) >> 8) + 16  ;           
     u = (unsigned char)( ( -38 * r -  74 * g + 112 * b + 128) >> 8) + 128 ;           
        v = (unsigned char)( ( 112 * r -  94 * g -  18 * b + 128) >> 8) + 128 ;
  *(bufY++) = max( 0, min(y, 255 ));
   
   if (j%2==0&&i%2 ==0)
   {
    if (u>255)
    {
     u=255;
    }
    if (u<0)
    {
     u = 0;
    }
    *(bufU++) =u;
//存u分量
   }
   else
   {
    //存v分量
    if (i%2==0)
    {
     if (v>255)
     {
      v = 255;
     }
     if (v<0)
     {
      v = 0;
     }
     *(bufV++) =v;
    
    }
   }

  }

 }
 *len = nWidth * nHeight+(nWidth * nHeight)/2;
 return true;
} 
