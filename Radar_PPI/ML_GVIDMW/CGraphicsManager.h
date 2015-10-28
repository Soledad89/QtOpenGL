//Name:   CGraphicsManager.h
//Date:   Jan 5, 2012
//Author: GuoYan in the Canteen Rohm Building, Tsinghua University

// 

#include <windows.h>

#include <gl/glew.h>
#include <gl/glut.h>


#include "bmp.h"

#ifndef _C_GRAGHICS_MANAGER_H_
#define _C_GRAGHICS_MANAGER_H_

#define SURF_PPI_WIDTH 1200                 // 后页FBO的尺寸
#define SURF_PPI_HEIGHT 1200 
#define SURF_MINI_WIDTH 340
#define SURF_MINI_HEIGHT 340
#define SURF_AR_WIDTH 360
#define SURF_AR_HEIGHT 300
#define SURF_TV_WIDTH 720
#define SURF_TV_HEIGHT 576



#define MAX_NUM_WIN   8
#define MAX_NUM_SURF  3

//#define NUM_FBO 8

class CGraphicsManager
{
private:	
	HDC*    hDC; 
	HGLRC  hRC;

	//bool visible[MAX_NUM_WIN][MAX_NUM_SURF];

	int numFBO;
	int* pNumTex;	           // 记录FBO中纹理的层数
	GLuint* pWidth;            // FBO宽度
	GLuint* pHeight;           // FBO高度

	GLuint* pFBO;              // 记录各FBO的代号
	GLuint* pDepthBuffer;      // 
	GLuint** ppTextures;       // 第几个FBO的第几层纹理

	int curDrawFBO;

	bool initFlag;

public:

	CGraphicsManager(HDC* hdc, int numWin);
	~CGraphicsManager();
	
	int init();
	int drawPoints(int winth, int surfth, int* vertices, unsigned int* colors, int numPoints);   // 在FBO中画点
	int drawLines(int winth, int* vertices, unsigned int* colors, int numPoints);   // 在FBO中画线
	int drawBackMap(int winth, BMPImageRec* pBackMap);
	int clear(int winth, int surfth, unsigned int theColor);
	int drawWinPoints(int winth, int* vertices, unsigned int* colors, int numPoints);
	int copyTex(int winth, int fboth, int surfth);
	int copyTexRectFBO(int fbothDst, int surfthDst, int fbothSrc, int surfthSrc, float theX, float theY, float theWidth, float theHeight);
	int update(int winth);
	int copyTexFBO(int fbothDst, int surfthDst, int fbothSrc, int surfthSrc);

	int clearWin(int winth, unsigned int theColor);

	int setColorKey(int winth, unsigned int colorKey, BMPImageRec* pBackMap1,int x, int y);

private:
	int createFBO(int fboth, GLsizei numfboTex, GLsizei fboWidth, GLsizei fboHeight);    //Create FBO and its related Textures and Render Buffer			
	int createFBOCoord(int fboth);
	int setDrawFBO(int drawFBOth);     // input drawFBOth = 0, 1, 2, ... , numFBO
};


#endif

