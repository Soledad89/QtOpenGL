//Name:   CGraphicsManager.cpp
//Date:   Jan 5, 2012
//Author: GuoYan in the Canteen Rohm Building, Tsinghua University

// 文件描述：此文件中，除了copyTex是对窗口显示缓冲区进行操作（写），其他函数都是对后页FBO的缓冲区进行操作。

#include <windows.h>
#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <GL/glut.h>

#include "bmp.h"
#include "CGraphicsManager.h"
#include "CWindowsManager.h"

#define min(x,y) (x)>(y)?(y):(x)

extern int winWidth[WINDOW_NUM];
extern int winHeight[WINDOW_NUM];

//int SURF_WIDTH[WINDOW_NUM] = {1200, 340,360,1200, 340,360,720,720};
//int SURF_HEIGHT[WINDOW_NUM] = {1200,340,300,1200,340,300,576,576};


CGraphicsManager::CGraphicsManager(HDC* hdc, int numDC)          //将外部所建立窗口的句柄传入进此类中，每个窗口都会有一个DC，通过此DC可以搭建与之相应的OpenGL渲染环境
{
	hDC = new HDC[numDC];
	for(int i = 0; i < numDC; i ++)
	{
		hDC[i] = hdc[i];
	}

	numFBO = numDC;
	pNumTex = new int[numFBO];
	pWidth = new GLuint[numFBO];
	pHeight = new GLuint[numFBO];

	pFBO = new GLuint[numFBO];
	pDepthBuffer = new GLuint[numFBO];
	ppTextures = new GLuint* [numFBO];

	curDrawFBO = 0;      //Windows On-Screen Mode	
	
	init();

}


//功能描述：为8个窗口创建OpenGL渲染环境，为8个窗口创建离屏帧缓冲区FBO，为在后页（FBO）上绘制图形做好准备
int CGraphicsManager::init()
{
	hRC = wglCreateContext(hDC[0]);      // RC will inherit the nature of the DC from which it is created.
	wglMakeCurrent(hDC[0], hRC);         // If no RC is made current, the OpenGL functions will take no effect.
	
	glewInit();

	createFBO(1, 4, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);      //设置各个窗口后页的层数和大小
	createFBO(2, 3, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	createFBO(3, 4, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	//createFBO(3, 2, SURF_AR_WIDTH, SURF_AR_HEIGHT);

	createFBO(4, 4, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	createFBO(5, 3, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	createFBO(6, 4, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	//createFBO(6, 2, SURF_AR_WIDTH, SURF_AR_HEIGHT);

	createFBO(7, 3, SURF_TV_WIDTH, SURF_TV_HEIGHT);
	createFBO(8, 3, SURF_TV_WIDTH, SURF_TV_HEIGHT);

	glEnable(GL_TEXTURE_2D);                 // 打开纹理叠加功能

	wglMakeCurrent(NULL, NULL);               // Make the RC associated to the given DC inactive

	return 0;
}



//函数功能：将第winth个窗口后页FBO的第surfth层全部清屏，注意：只要涉及到后页FBO，都会有一个切换过程，Windows -> FBO -> Windows
int CGraphicsManager::clear(int winth, int surfth, unsigned int theColor)       // winth: 1 ~ 8      surfth: 1 ~ 3
{
	if ( (winth <= 0) || (winth > numFBO) ) 
	{
		return -1;
	}
	if((surfth <= 0) || (surfth > MAX_NUM_SURF))
	{
		return -1;
	}


	float r, g, b, a;

	r = float((theColor >> 24) & 0xff) / 255;
	g = float((theColor >> 16) & 0xff) / 255;
	b = float((theColor >> 8) & 0xff) / 255;
	a = float(theColor & 0xff) / 255;

	wglMakeCurrent(hDC[winth - 1], hRC); 	//任何时刻，只能有一个RC处于当前激活状态

	setDrawFBO(winth);        //切换到第winth个窗口的后页FBO，视口与坐标系会变换
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + surfth - 1);    //设置FBO的层，最上层（第0层）是用户标绘层

	glClearColor(r ,g , b , a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 	setDrawFBO(0);     //切换回Windows屏幕渲染方式

	wglMakeCurrent(NULL, NULL);

	return 0;
}


int CGraphicsManager::clearWin(int winth, unsigned int theColor)
{
	float r, g, b, a;

	r = float((theColor >> 24) & 0xff) / 255;
	g = float((theColor >> 16) & 0xff) / 255;
	b = float((theColor >> 8) & 0xff) / 255;
	a = float(theColor & 0xff) / 255;

	wglMakeCurrent(hDC[winth - 1], hRC); 

	glClearColor(r ,g , b , a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wglMakeCurrent(NULL, NULL);

	return 0;
}




//功能描述：将pBackMap所指向的图像绘制到第winth个窗口所建立的后页FBO上，过程：CPU内存(pBackMap) -> GPU显存（texture） -> GPU显存（FBO）
int CGraphicsManager::drawBackMap(int winth, BMPImageRec* pBackMap1)   // Possible Problems: frequent state conversions
{
	if ((winth != 1) && (winth != 4) && (winth != 7) && (winth != 8))
	{
		return -1;
	}

	wglMakeCurrent(hDC[winth - 1], hRC); 	

	//将pBackMap所指向内存区域（CPU端）的数据搬到显存（GPU端）中，以纹理的格式储存
 	GLuint texture;	
	glGenTextures(1 , &texture);
			
	glBindTexture(GL_TEXTURE_2D , texture);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , pBackMap1->sizeX , pBackMap1->sizeY , 0 , GL_BGR , GL_UNSIGNED_BYTE , pBackMap1->data);
	glBindTexture(GL_TEXTURE_2D , 0);
	
	//将渲染模式从Windows屏幕渲染方式切换到FBO离屏渲染方式，使得之前所建立的纹理可以贴到后页FBO上，而不是直接贴到Windows屏幕对应的显示缓冲区中
	setDrawFBO(winth);
	if ((winth == 1) || (winth == 4))
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + 2);  //0： 用户标绘层     1： 雷达数据层     2： 背景海图层
	else
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + 1);     //0:用户标会层  1:电视


	//将前面所建立显存中的纹理贴到第winth个窗口的后页FBO上
	glClearColor(0.0 ,0.0 , 0.0 , 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D , texture);
	glBegin(GL_QUADS);
	if ((winth == 1) || (winth == 4))
	{
		glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex2f(0.0, SURF_PPI_HEIGHT);
		glTexCoord2f(1.0, 1.0); glVertex2f( SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
		glTexCoord2f(1.0, 0.0); glVertex2f( SURF_PPI_WIDTH , 0.0);
	}
	else
	{
		glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex2f(0.0, SURF_TV_HEIGHT);
		glTexCoord2f(1.0, 1.0); glVertex2f( SURF_TV_WIDTH, SURF_TV_HEIGHT);
		glTexCoord2f(1.0, 0.0); glVertex2f( SURF_TV_WIDTH , 0.0);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D , 0);

 	setDrawFBO(0);              // 切换回Windows屏幕渲染方式

	glDeleteTextures(1 , &texture);

	wglMakeCurrent(NULL, NULL);

	return 0;
}


//函数功能:将第fbothSrc个FBO后页的第surfthSrc个层以混合的方法贴到第fbothDst个FBO的第surfthDst层
int CGraphicsManager::copyTexFBO(int fbothDst, int surfthDst, int fbothSrc, int surfthSrc)
{
	wglMakeCurrent(hDC[0], hRC); 	    

	setDrawFBO(fbothDst);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + surfthDst - 1);  

	glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, ppTextures[fbothSrc - 1][surfthSrc - 1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex2f(0.0, pHeight[fbothDst-1]);
	glTexCoord2f(1.0, 1.0); glVertex2f(pWidth[fbothDst-1], pHeight[fbothDst-1]);
	glTexCoord2f(1.0, 0.0); glVertex2f(pWidth[fbothDst-1], 0.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D , 0);

	glDisable(GL_BLEND);

 	setDrawFBO(0);              // 切换回Windows屏幕渲染方式

	wglMakeCurrent(NULL, NULL);

	return 0;
}


// 函数功能:将第winth个FBO后页的第surfth个层的某一特定区域，以混合的方法贴到第winth个窗口中
int CGraphicsManager::copyTexRectFBO(int fbothDst, int surfthDst, int fbothSrc, int surfthSrc, float theX, float theY, float theWidth, float theHeight)
{
	wglMakeCurrent(hDC[0], hRC); 	    // 激活第winth个窗口的OpenGL渲染环境
   
	setDrawFBO(fbothDst);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + surfthDst - 1);

	glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, ppTextures[fbothSrc - 1][surfthSrc - 1]);
	glBegin(GL_QUADS);
	glTexCoord2f(theX, theY); glVertex2f(0.0, 0.0);
	glTexCoord2f(theX, theY + theHeight); glVertex2f(0.0, pHeight[fbothDst-1]);
	glTexCoord2f(theX + theWidth, theY + theHeight); glVertex2f(pWidth[fbothDst-1], pHeight[fbothDst-1]);
	glTexCoord2f(theX + theWidth, theY); glVertex2f(pWidth[fbothDst-1], 0.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D , 0);

	glDisable(GL_BLEND);

 	setDrawFBO(0);              // 切换回Windows屏幕渲染方式

	wglMakeCurrent(NULL, NULL);

	return 0;
}



//函数功能:将第winth个FBO后页的第surfth个层以混合的方法贴到第winth个窗口中
int CGraphicsManager::copyTex(int winth, int fboth, int surfth)
{
	wglMakeCurrent(hDC[winth - 1], hRC); 	    // 激活第winth个窗口的OpenGL渲染环境
	glViewport(0 , 0 , (GLsizei) winWidth[winth - 1] , (GLsizei) winHeight[winth - 1]);   

	glBindTexture(GL_TEXTURE_2D, ppTextures[fboth - 1][surfth - 1]);        // 将第winth个FBO后页的第surfth个层贴到第winth个窗口中
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex2f(1.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 0.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D , 0);

	wglMakeCurrent(NULL, NULL);

	return 0;
}


//函数功能：执行所有之前缓冲的OpenGL命令，使其在后缓冲区中进行渲染，等渲染完成之后，交换前后缓冲区，使后缓冲区中的内容进行显示
int CGraphicsManager::update(int winth)
{
	wglMakeCurrent(hDC[winth - 1], hRC); 	    // 激活第winth个窗口的OpenGL渲染环境

	glFlush();         // 强制刷新缓冲去中的OpenGL指令，保证绘图命令将被执行，而不是存储在缓冲区中等待其他的OpenGL命令
	//SwapBuffers(hDC[winth - 1]);    //将后缓冲区中画好的内容放到前缓冲区中，进行显示，屏幕上显示的内容是前缓冲区中的

	wglMakeCurrent(NULL, NULL);

	return 0;
}



//函数功能：直接在第winth个窗口中画numPoints个点    （以混合的方式？）
int CGraphicsManager::drawWinPoints(int winth, int* vertices, unsigned int* colors, int numPoints)
{
	wglMakeCurrent(hDC[winth - 1], hRC); 	
	glViewport(0, 0, (GLsizei) winWidth[winth - 1], (GLsizei) winHeight[winth - 1]);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_INT, 0, vertices);  //指定顶点的位置坐标
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, (unsigned char*)colors);   //指定顶点的颜色坐标

    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	//glPointSize(5.0);
	glDrawArrays(GL_POINTS, 0, numPoints);

	glDisable(GL_BLEND);

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);

	wglMakeCurrent(NULL, NULL);

	return 0;
}


//功能描述：将numPoints个点绘制到第winth个窗口的后页上，过程：CPU内存（vertices） -> GPU显存（FBO）
int CGraphicsManager::drawPoints(int fboth, int surfth, int* vertices, unsigned int* colors, int numPoints)  //Possible problems: frequent state convertions
{                                                           //Windows -> FBO -> Windows -> FBO ... 每画一条线，都会切换一次
	wglMakeCurrent(hDC[0], hRC); 	

	setDrawFBO(fboth);     //打开第winth个FBO
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + surfth - 1);   // 1: PPI雷达数据层

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_INT, 0, vertices);  //指定顶点的位置坐标
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, (unsigned char*)colors);   //指定顶点的颜色坐标

	//glPointSize(5.0);
	glDrawArrays(GL_POINTS, 0, numPoints);

	//glBegin(GL_QUADS);
	//glArrayElement(0);
	//glArrayElement(1);
	//glArrayElement(2);
	//glArrayElement(3);
	//glEnd();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);

 	setDrawFBO(0);

	wglMakeCurrent(NULL, NULL);

	return 0;
}


// 函数功能：在后页FBO中画线
int CGraphicsManager::drawLines(int winth, int* vertices, unsigned int* colors, int numPoints)  //Possible problems: frequent state convertions
{                                                           //Windows -> FBO -> Windows -> FBO ... 每画一条线，都会切换一次

	wglMakeCurrent(hDC[0], hRC); 	

	setDrawFBO(winth);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + 1);   // 1: PPI雷达数据层

	float c = 1.0 / 8;
	glViewport(c * pWidth[winth - 1], c * pHeight[winth - 1], (1 - 2 * c) * pWidth[winth - 1] , (1 - 2 * c) *  pHeight[winth - 1]);       // 

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_INT, 0, vertices);  //指定顶点的位置坐标
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, (unsigned char*)colors);   //指定顶点的颜色坐标

	glLineWidth(5.0);
	glDrawArrays(GL_LINE_STRIP, 0, numPoints);


	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);

 	setDrawFBO(0);

	wglMakeCurrent(NULL, NULL);

	return 0;
}


//功能描述：创建第fboth个FBO，这个FBO的层数为numfboTex，所有层的宽fboWidth，高fboHeight
int CGraphicsManager::createFBO(int fboth, GLsizei numfboTex, GLsizei fboWidth, GLsizei fboHeight)    // potential problem: repeat creating FBO1
{
	if ( (fboth <= 0) || (fboth > numFBO) ) 
	{
		return -1;
	}

	pNumTex[fboth - 1] = numfboTex;
	pWidth[fboth - 1] = fboWidth;
	pHeight[fboth - 1] = fboHeight;
	ppTextures[fboth - 1] = new GLuint[pNumTex[fboth - 1]];

	glGenFramebuffersEXT(1 , &pFBO[fboth - 1]);	  //创建fbo    A FBO is a container.    
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT , pFBO[fboth - 1]);	
	

	glGenTextures(pNumTex[fboth - 1] , ppTextures[fboth - 1]);          //创建numTex个所需关联的纹理对象
	for(int i = 0; i < pNumTex[fboth - 1]; i ++)
	{
		glBindTexture(GL_TEXTURE_2D , ppTextures[fboth - 1][i]);
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , pWidth[fboth - 1] , pHeight[fboth - 1] , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT , GL_COLOR_ATTACHMENT0_EXT + i , GL_TEXTURE_2D , ppTextures[fboth - 1][i] , 0);   //将纹理与fbo关联

	}
	glBindTexture(GL_TEXTURE_2D , 0);
   
	glGenRenderbuffersEXT(1 , &pDepthBuffer[fboth - 1]);     //创建深度渲染缓冲区
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT , pDepthBuffer[fboth - 1]);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT , GL_DEPTH_COMPONENT , pWidth[fboth - 1] , pHeight[fboth - 1]);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT , 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT , GL_DEPTH_ATTACHMENT_EXT , GL_RENDERBUFFER_EXT , pDepthBuffer[fboth - 1]);  //将和深度缓存与fbo关联


	GLenum fboStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);   //检查当前FBO的状态
	if(fboStatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		cout << "FBO创建失败！" << endl;
		exit(1);
	}
	else
		cout << "FBO创建成功！" << endl;


	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT , 0);      // Restore to the window on-screeen rendering.


	return 0;
}


//函数功能：设置第fboth个FBO的坐标系统
int CGraphicsManager::createFBOCoord(int fboth)
{
	if ( (fboth < 0) || (fboth > numFBO) ) 
	{
		return -1;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(fboth)
	{
		gluOrtho2D(0.0, (GLdouble) pWidth[fboth - 1], 0.0, (GLdouble) pHeight[fboth - 1]);         // FBO的宽、高
		//gluOrtho2D(0.0, (GLdouble) SURF_PPI_WIDTH, 0.0, (GLdouble) SURF_PPI_HEIGHT);         // FBO的宽、高
	}
	else
	{
		gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	}

	return 0;
}


//函数功能：将渲染环境切换到第fboth个窗口的后页FBO，接下来所有OpenGL函数所渲染的场景都会被绘制到此后页FBO中
int CGraphicsManager::setDrawFBO(int fboth)
{
	if ( (fboth < 0) || (fboth > numFBO) ) 
	{
		return -1;
	}

	if(curDrawFBO == 0)     // in Window state
	{
		if(fboth)                 // to FBO state
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT , pFBO[fboth - 1]);
			glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);		
				
			glViewport(0 , 0 , pWidth[fboth - 1] , pHeight[fboth - 1]);	     // 视口  
			createFBOCoord(fboth);		                                     // 坐标系
			curDrawFBO = fboth;
		}
	}
	else               // in FBO state
	{
		if(fboth == 0)             // to Window state
		{
			glPopAttrib();
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT , 0);
			createFBOCoord(fboth);
			curDrawFBO = 0;
		}
		else
		{
			if(fboth != curDrawFBO)            // to other FBO state  
			{
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT , pFBO[fboth - 1]);	
				glViewport(0 , 0 , pWidth[fboth - 1] , pHeight[fboth - 1]);
				createFBOCoord(fboth);	
				curDrawFBO = fboth;
			}
		}
	}
	
	return 0;
}

//功能描述：改变后页FBO中某种颜色的alfa值，过程： GPU显存（texture） -> CPU内存(pBackMap) -> GPU显存（FBO）
int CGraphicsManager::setColorKey(int winth, unsigned int colorKey, BMPImageRec* pBackMap1,int  x, int y)   // Possible Problems: frequent state conversions
{
	if(pBackMap1 == NULL)
		return -1;

	BMPImageRec* NewpBackMap = new BMPImageRec;
	
	int r, g, b, a;

	r = colorKey >> 24;
	g = (colorKey >> 16) & 0xff;
	b = (colorKey >> 8) & 0xff;
	a = colorKey & 0xff;

	GLubyte *newData = new GLubyte [pBackMap1->sizeX* pBackMap1->sizeY*4];
	for(int i =0; i<pBackMap1->sizeX* pBackMap1->sizeY;i++)
	{
		*(newData+i*4) = *(pBackMap1->data+3*i);
		*(newData+i*4+1) = *(pBackMap1->data+3*i+1);
		*(newData+i*4+2) = *(pBackMap1->data+3*i+2);
		if((*(pBackMap1->data+3*i) == b) && (*(pBackMap1->data+3*i+1) == g) && (*(pBackMap1->data+3*i+2) == r))
			*(newData+i*4+3) = 0;
		else
			*(newData+i*4+3) = 255;
	}

	NewpBackMap->sizeX= pBackMap1 ->sizeX;
	NewpBackMap->sizeY= pBackMap1 ->sizeY;
	NewpBackMap->data= newData;
//	delete (newData);


	wglMakeCurrent(hDC[winth - 1], hRC); 	

	//将pBackMap所指向内存区域（CPU端）的数据搬到显存（GPU端）中，以纹理的格式储存
 	GLuint texture;	
	glGenTextures(1 , &texture);
			
	glBindTexture(GL_TEXTURE_2D , texture);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , NewpBackMap->sizeX , NewpBackMap->sizeY , 0 , GL_BGRA , GL_UNSIGNED_BYTE , NewpBackMap->data);
	glBindTexture(GL_TEXTURE_2D , 0);
	
	//将渲染模式从Windows屏幕渲染方式切换到FBO离屏渲染方式，使得之前所建立的纹理可以贴到后页FBO上，而不是直接贴到Windows屏幕对应的显示缓冲区中
	setDrawFBO(winth);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);     //0:用户标会层

	int WIDTH,HEIGHT;
	WIDTH = winth < 7 ? SURF_PPI_WIDTH:SURF_TV_WIDTH;
	HEIGHT = winth < 7 ? SURF_PPI_HEIGHT:SURF_TV_HEIGHT;

	//将前面所建立显存中的纹理贴到第winth个窗口的后页FBO上
	glClearColor(0.0 ,0.0 , 0.0 , 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D , texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(x, y);
		glTexCoord2f(0.0, 1.0); glVertex2f(x, min(y + pBackMap1->sizeY,HEIGHT));
		glTexCoord2f(1.0, 1.0); glVertex2f(min(x + pBackMap1->sizeX,WIDTH), min(y + pBackMap1->sizeY,HEIGHT));
		glTexCoord2f(1.0, 0.0); glVertex2f(min(x + pBackMap1->sizeX,WIDTH), y);
	glEnd();
	glBindTexture(GL_TEXTURE_2D , 0);

 	setDrawFBO(0);              // 切换回Windows屏幕渲染方式

	glDeleteTextures(1 , &texture);

	wglMakeCurrent(NULL, NULL);
	delete(NewpBackMap);

	return 0;	
} 



//函数功能：释放
CGraphicsManager::~CGraphicsManager()
{
	//for(int i = 0; i < numFBO; i ++)
	//{
	//	ReleaseDC(hWnd[i], hDC[i]);          //The number of DC is limited, so it should be released.
	//}
	//delete []hDC;

	wglDeleteContext(hRC);                // Delete the RC

	for(int i = 0; i < numFBO; i ++)
	{
		glDeleteRenderbuffersEXT(1, &pDepthBuffer[i]);
		glDeleteTextures(pNumTex[i], ppTextures[i]);
		glDeleteFramebuffersEXT(1, &pFBO[i]);
	}

	delete []pNumTex;
	delete []pWidth;
	delete []pHeight;

	delete []pFBO;
	delete []pDepthBuffer;
	for(int i = 0; i < numFBO; i ++)
	{
		delete []ppTextures[i];
	}
	delete []ppTextures;

}