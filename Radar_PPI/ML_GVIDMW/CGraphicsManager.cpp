//Name:   CGraphicsManager.cpp
//Date:   Jan 5, 2012
//Author: GuoYan in the Canteen Rohm Building, Tsinghua University

// �ļ����������ļ��У�����copyTex�ǶԴ�����ʾ���������в�����д���������������ǶԺ�ҳFBO�Ļ��������в�����

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


CGraphicsManager::CGraphicsManager(HDC* hdc, int numDC)          //���ⲿ���������ڵľ������������У�ÿ�����ڶ�����һ��DC��ͨ����DC���Դ��֮��Ӧ��OpenGL��Ⱦ����
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


//����������Ϊ8�����ڴ���OpenGL��Ⱦ������Ϊ8�����ڴ�������֡������FBO��Ϊ�ں�ҳ��FBO���ϻ���ͼ������׼��
int CGraphicsManager::init()
{
	hRC = wglCreateContext(hDC[0]);      // RC will inherit the nature of the DC from which it is created.
	wglMakeCurrent(hDC[0], hRC);         // If no RC is made current, the OpenGL functions will take no effect.
	
	glewInit();

	createFBO(1, 4, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);      //���ø������ں�ҳ�Ĳ����ʹ�С
	createFBO(2, 3, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	createFBO(3, 4, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	//createFBO(3, 2, SURF_AR_WIDTH, SURF_AR_HEIGHT);

	createFBO(4, 4, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	createFBO(5, 3, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	createFBO(6, 4, SURF_PPI_WIDTH, SURF_PPI_HEIGHT);
	//createFBO(6, 2, SURF_AR_WIDTH, SURF_AR_HEIGHT);

	createFBO(7, 3, SURF_TV_WIDTH, SURF_TV_HEIGHT);
	createFBO(8, 3, SURF_TV_WIDTH, SURF_TV_HEIGHT);

	glEnable(GL_TEXTURE_2D);                 // ��������ӹ���

	wglMakeCurrent(NULL, NULL);               // Make the RC associated to the given DC inactive

	return 0;
}



//�������ܣ�����winth�����ں�ҳFBO�ĵ�surfth��ȫ��������ע�⣺ֻҪ�漰����ҳFBO��������һ���л����̣�Windows -> FBO -> Windows
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

	wglMakeCurrent(hDC[winth - 1], hRC); 	//�κ�ʱ�̣�ֻ����һ��RC���ڵ�ǰ����״̬

	setDrawFBO(winth);        //�л�����winth�����ڵĺ�ҳFBO���ӿ�������ϵ��任
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + surfth - 1);    //����FBO�Ĳ㣬���ϲ㣨��0�㣩���û�����

	glClearColor(r ,g , b , a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 	setDrawFBO(0);     //�л���Windows��Ļ��Ⱦ��ʽ

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




//������������pBackMap��ָ���ͼ����Ƶ���winth�������������ĺ�ҳFBO�ϣ����̣�CPU�ڴ�(pBackMap) -> GPU�Դ棨texture�� -> GPU�Դ棨FBO��
int CGraphicsManager::drawBackMap(int winth, BMPImageRec* pBackMap1)   // Possible Problems: frequent state conversions
{
	if ((winth != 1) && (winth != 4) && (winth != 7) && (winth != 8))
	{
		return -1;
	}

	wglMakeCurrent(hDC[winth - 1], hRC); 	

	//��pBackMap��ָ���ڴ�����CPU�ˣ������ݰᵽ�Դ棨GPU�ˣ��У�������ĸ�ʽ����
 	GLuint texture;	
	glGenTextures(1 , &texture);
			
	glBindTexture(GL_TEXTURE_2D , texture);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , pBackMap1->sizeX , pBackMap1->sizeY , 0 , GL_BGR , GL_UNSIGNED_BYTE , pBackMap1->data);
	glBindTexture(GL_TEXTURE_2D , 0);
	
	//����Ⱦģʽ��Windows��Ļ��Ⱦ��ʽ�л���FBO������Ⱦ��ʽ��ʹ��֮ǰ���������������������ҳFBO�ϣ�������ֱ������Windows��Ļ��Ӧ����ʾ��������
	setDrawFBO(winth);
	if ((winth == 1) || (winth == 4))
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + 2);  //0�� �û�����     1�� �״����ݲ�     2�� ������ͼ��
	else
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + 1);     //0:�û�����  1:����


	//��ǰ���������Դ��е�����������winth�����ڵĺ�ҳFBO��
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

 	setDrawFBO(0);              // �л���Windows��Ļ��Ⱦ��ʽ

	glDeleteTextures(1 , &texture);

	wglMakeCurrent(NULL, NULL);

	return 0;
}


//��������:����fbothSrc��FBO��ҳ�ĵ�surfthSrc�����Ի�ϵķ���������fbothDst��FBO�ĵ�surfthDst��
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

 	setDrawFBO(0);              // �л���Windows��Ļ��Ⱦ��ʽ

	wglMakeCurrent(NULL, NULL);

	return 0;
}


// ��������:����winth��FBO��ҳ�ĵ�surfth�����ĳһ�ض������Ի�ϵķ���������winth��������
int CGraphicsManager::copyTexRectFBO(int fbothDst, int surfthDst, int fbothSrc, int surfthSrc, float theX, float theY, float theWidth, float theHeight)
{
	wglMakeCurrent(hDC[0], hRC); 	    // �����winth�����ڵ�OpenGL��Ⱦ����
   
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

 	setDrawFBO(0);              // �л���Windows��Ļ��Ⱦ��ʽ

	wglMakeCurrent(NULL, NULL);

	return 0;
}



//��������:����winth��FBO��ҳ�ĵ�surfth�����Ի�ϵķ���������winth��������
int CGraphicsManager::copyTex(int winth, int fboth, int surfth)
{
	wglMakeCurrent(hDC[winth - 1], hRC); 	    // �����winth�����ڵ�OpenGL��Ⱦ����
	glViewport(0 , 0 , (GLsizei) winWidth[winth - 1] , (GLsizei) winHeight[winth - 1]);   

	glBindTexture(GL_TEXTURE_2D, ppTextures[fboth - 1][surfth - 1]);        // ����winth��FBO��ҳ�ĵ�surfth����������winth��������
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


//�������ܣ�ִ������֮ǰ�����OpenGL���ʹ���ں󻺳����н�����Ⱦ������Ⱦ���֮�󣬽���ǰ�󻺳�����ʹ�󻺳����е����ݽ�����ʾ
int CGraphicsManager::update(int winth)
{
	wglMakeCurrent(hDC[winth - 1], hRC); 	    // �����winth�����ڵ�OpenGL��Ⱦ����

	glFlush();         // ǿ��ˢ�»���ȥ�е�OpenGLָ���֤��ͼ�����ִ�У������Ǵ洢�ڻ������еȴ�������OpenGL����
	//SwapBuffers(hDC[winth - 1]);    //���󻺳����л��õ����ݷŵ�ǰ�������У�������ʾ����Ļ����ʾ��������ǰ�������е�

	wglMakeCurrent(NULL, NULL);

	return 0;
}



//�������ܣ�ֱ���ڵ�winth�������л�numPoints����    ���Ի�ϵķ�ʽ����
int CGraphicsManager::drawWinPoints(int winth, int* vertices, unsigned int* colors, int numPoints)
{
	wglMakeCurrent(hDC[winth - 1], hRC); 	
	glViewport(0, 0, (GLsizei) winWidth[winth - 1], (GLsizei) winHeight[winth - 1]);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_INT, 0, vertices);  //ָ�������λ������
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, (unsigned char*)colors);   //ָ���������ɫ����

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


//������������numPoints������Ƶ���winth�����ڵĺ�ҳ�ϣ����̣�CPU�ڴ棨vertices�� -> GPU�Դ棨FBO��
int CGraphicsManager::drawPoints(int fboth, int surfth, int* vertices, unsigned int* colors, int numPoints)  //Possible problems: frequent state convertions
{                                                           //Windows -> FBO -> Windows -> FBO ... ÿ��һ���ߣ������л�һ��
	wglMakeCurrent(hDC[0], hRC); 	

	setDrawFBO(fboth);     //�򿪵�winth��FBO
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + surfth - 1);   // 1: PPI�״����ݲ�

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_INT, 0, vertices);  //ָ�������λ������
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, (unsigned char*)colors);   //ָ���������ɫ����

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


// �������ܣ��ں�ҳFBO�л���
int CGraphicsManager::drawLines(int winth, int* vertices, unsigned int* colors, int numPoints)  //Possible problems: frequent state convertions
{                                                           //Windows -> FBO -> Windows -> FBO ... ÿ��һ���ߣ������л�һ��

	wglMakeCurrent(hDC[0], hRC); 	

	setDrawFBO(winth);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + 1);   // 1: PPI�״����ݲ�

	float c = 1.0 / 8;
	glViewport(c * pWidth[winth - 1], c * pHeight[winth - 1], (1 - 2 * c) * pWidth[winth - 1] , (1 - 2 * c) *  pHeight[winth - 1]);       // 

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_INT, 0, vertices);  //ָ�������λ������
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, (unsigned char*)colors);   //ָ���������ɫ����

	glLineWidth(5.0);
	glDrawArrays(GL_LINE_STRIP, 0, numPoints);


	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);

 	setDrawFBO(0);

	wglMakeCurrent(NULL, NULL);

	return 0;
}


//����������������fboth��FBO�����FBO�Ĳ���ΪnumfboTex�����в�Ŀ�fboWidth����fboHeight
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

	glGenFramebuffersEXT(1 , &pFBO[fboth - 1]);	  //����fbo    A FBO is a container.    
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT , pFBO[fboth - 1]);	
	

	glGenTextures(pNumTex[fboth - 1] , ppTextures[fboth - 1]);          //����numTex������������������
	for(int i = 0; i < pNumTex[fboth - 1]; i ++)
	{
		glBindTexture(GL_TEXTURE_2D , ppTextures[fboth - 1][i]);
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , pWidth[fboth - 1] , pHeight[fboth - 1] , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT , GL_COLOR_ATTACHMENT0_EXT + i , GL_TEXTURE_2D , ppTextures[fboth - 1][i] , 0);   //��������fbo����

	}
	glBindTexture(GL_TEXTURE_2D , 0);
   
	glGenRenderbuffersEXT(1 , &pDepthBuffer[fboth - 1]);     //���������Ⱦ������
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT , pDepthBuffer[fboth - 1]);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT , GL_DEPTH_COMPONENT , pWidth[fboth - 1] , pHeight[fboth - 1]);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT , 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT , GL_DEPTH_ATTACHMENT_EXT , GL_RENDERBUFFER_EXT , pDepthBuffer[fboth - 1]);  //������Ȼ�����fbo����


	GLenum fboStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);   //��鵱ǰFBO��״̬
	if(fboStatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		cout << "FBO����ʧ�ܣ�" << endl;
		exit(1);
	}
	else
		cout << "FBO�����ɹ���" << endl;


	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT , 0);      // Restore to the window on-screeen rendering.


	return 0;
}


//�������ܣ����õ�fboth��FBO������ϵͳ
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
		gluOrtho2D(0.0, (GLdouble) pWidth[fboth - 1], 0.0, (GLdouble) pHeight[fboth - 1]);         // FBO�Ŀ���
		//gluOrtho2D(0.0, (GLdouble) SURF_PPI_WIDTH, 0.0, (GLdouble) SURF_PPI_HEIGHT);         // FBO�Ŀ���
	}
	else
	{
		gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	}

	return 0;
}


//�������ܣ�����Ⱦ�����л�����fboth�����ڵĺ�ҳFBO������������OpenGL��������Ⱦ�ĳ������ᱻ���Ƶ��˺�ҳFBO��
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
				
			glViewport(0 , 0 , pWidth[fboth - 1] , pHeight[fboth - 1]);	     // �ӿ�  
			createFBOCoord(fboth);		                                     // ����ϵ
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

//�����������ı��ҳFBO��ĳ����ɫ��alfaֵ�����̣� GPU�Դ棨texture�� -> CPU�ڴ�(pBackMap) -> GPU�Դ棨FBO��
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

	//��pBackMap��ָ���ڴ�����CPU�ˣ������ݰᵽ�Դ棨GPU�ˣ��У�������ĸ�ʽ����
 	GLuint texture;	
	glGenTextures(1 , &texture);
			
	glBindTexture(GL_TEXTURE_2D , texture);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , NewpBackMap->sizeX , NewpBackMap->sizeY , 0 , GL_BGRA , GL_UNSIGNED_BYTE , NewpBackMap->data);
	glBindTexture(GL_TEXTURE_2D , 0);
	
	//����Ⱦģʽ��Windows��Ļ��Ⱦ��ʽ�л���FBO������Ⱦ��ʽ��ʹ��֮ǰ���������������������ҳFBO�ϣ�������ֱ������Windows��Ļ��Ӧ����ʾ��������
	setDrawFBO(winth);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);     //0:�û�����

	int WIDTH,HEIGHT;
	WIDTH = winth < 7 ? SURF_PPI_WIDTH:SURF_TV_WIDTH;
	HEIGHT = winth < 7 ? SURF_PPI_HEIGHT:SURF_TV_HEIGHT;

	//��ǰ���������Դ��е�����������winth�����ڵĺ�ҳFBO��
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

 	setDrawFBO(0);              // �л���Windows��Ļ��Ⱦ��ʽ

	glDeleteTextures(1 , &texture);

	wglMakeCurrent(NULL, NULL);
	delete(NewpBackMap);

	return 0;	
} 



//�������ܣ��ͷ�
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