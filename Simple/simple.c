// Simple.c
// The Simplest OpenGL program with GLUT
// OpenGL SuperBible, 3rd Edition
// Richard S. Wright Jr.
// rwright@starstonesoftware.com

#include "OpenGLSB.h"	// System and OpenGL Stuff

#include "math.h"


#define GL_PI 3.14
int BOX=1;

float dx=0.0f;

double TIME=1;
 int i=0;
 int n=0;

    //������������
     double xd[9];
	 double yd[9];

     //�������ٶ�
	 double vox[9]={250,-400,200,600,-100,-300,-500,90,300};
	 double voy[9]={-400,-300,600,0,300,300,500,-250,0};
	 double ax[9]={-4,10,16,-12,0,0,12,0,0};
	 double ay[9]={12,-40,0,0,0,0,-6,0,10};
 
     double list[9][2][52];  //���溽����

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
	{
    float angle=0.0;
	 float q,p;
	 
      int j;
	  int a;

       
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 1.0f, 0.0f); 
	 //�򿪿���ݣ�����ʾ�����ܽ�����ѵĴ���
	 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	 glEnable(GL_BLEND);
     glEnable(GL_POINT_SMOOTH);
	 glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
     glEnable(GL_LINE_SMOOTH);
     glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
     glEnable(GL_POLYGON_SMOOTH);
     glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);   
	 glLineWidth(1.0);    //�����߿�
     glPointSize(10.0);    //���õ�Ĵ�С
	 
  
     
     glBegin(GL_POINTS);   //����
     glVertex2f(0.0,0.0);     
	 glEnd();
//////////////���̶�//////////
     
	 for( angle=1.57f;angle<=(GL_PI*2); angle+=GL_PI/6.0f)  
	 {                                                                 
         float x,y;
		 float w;
		 float v;
		 x=(0.9f)*sin(angle);   
         y=(0.9f)*cos(angle); 
         w=(0.8f)*sin(angle);   
         v=(0.8f)*cos(angle); 		 
	
		 glBegin(GL_LINES);   //����
		 glVertex2f(x,y);  
	 	 glVertex2f(w,v);
		 glEnd();
	 } 

 for( angle=0.0;angle<=(GL_PI/2); angle+=GL_PI/18.0f)  
	 {                                                                 
         float x,y;
		 float w;
		 float v;
		 x=(0.9f)*sin(angle);   
         y=(0.9f)*cos(angle); 
         w=(0.8f)*sin(angle);   
         v=(0.8f)*cos(angle); 		 
	
		 glBegin(GL_LINES);   //����
		 glVertex2f(x,y);  
	 	 glVertex2f(w,v);
		 glEnd();
	 } 


/*
         glBegin(GL_LINES);   //����
		 glVertex2f(-0.9f,0.0);  
	 	 glVertex2f(-0.8f,0.0);
		 glEnd();

         glBegin(GL_LINES);   //����
		 glVertex2f(0.0,-0.9f);  
	 	 glVertex2f(0.0,-0.8f);
		 glEnd();
*/
////////////��С����///////////////////////
     glColor3f(0.8f,0.8f,1.0f);
	 glBegin(GL_TRIANGLES);
     glVertex2f(0.0,0.9f);
     glVertex2f(0.05f,1.0f);
     glVertex2f(-0.05f,1.0f);
	 glEnd();


//////////////��Բ////////////////	 
	 glColor3f(0.0f, 1.0f, 0.0f);
	 glLineWidth(2.0);    //�����߿�
		   
	 for(angle=0.0;angle<=(2.0*GL_PI); angle+=0.05f)  
	 {  
		 float x,y;
         x=(0.9f)*sin(angle);   
         y=(0.9f)*cos(angle); 
         glBegin(GL_LINE_LOOP);   //���պ�����
		 glVertex2f(x,y);  
	 	
	 }    	
	 glEnd();   

for(angle=0.0;angle<=(2.0*GL_PI); angle+=0.05f)  
	 {  
		 float x,y;
         x=(0.6f)*sin(angle);   
         y=(0.6f)*cos(angle); 
         glBegin(GL_LINE_LOOP);   //���պ�����
		 glVertex2f(x,y);  
	 	
	 }    	
	 glEnd();   

for(angle=0.0;angle<=(2.0*GL_PI); angle+=0.05f)  
	 {  
		 float x,y;
         x=(0.3f)*sin(angle);   
         y=(0.3f)*cos(angle); 
         glBegin(GL_LINE_LOOP);   //���պ�����
		 glVertex2f(x,y);  
	 	
	 }    	
	 glEnd();   


/////////////////������//////////////////

	 //������Χ20000��
    
	
    //����1
	xd[0]=0.9f*(-15000+vox[0]*TIME+0.5*ax[0]*TIME*TIME)/20000;  
	yd[0]=0.9f*(15000+voy[0]*TIME+0.5*ay[0]*TIME*TIME)/20000;
	if(xd[0]*xd[0]+yd[0]*yd[0]>0.81)
		yd[0]=2;
      
    //����2
    xd[1]=0.9f*(10000+vox[1]*TIME+0.5*ax[1]*TIME*TIME)/20000;
    yd[1]=0.9f*(10000+(voy[1]+0.5*(ay[1]+0.8*TIME)*TIME)*TIME)/20000;
 	if(xd[1]*xd[1]+yd[1]*yd[1]>0.81)		
	  xd[1]=2;

	//����3
    xd[2]=0.9f*(-15000+vox[2]*TIME+0.5*ax[2]*TIME*TIME)/20000;
    yd[2]=0.9f*(-15000+voy[2]*TIME)/20000;
	if(xd[2]*xd[2]+yd[2]*yd[2]>0.81) 
	  xd[2]=2;

	//����4
    xd[3]=0.9f*(-15000+vox[3]*TIME+0.5*ax[3]*TIME*TIME)/20000;
	yd[3]=0.9f*(-15000+0.5*(ay[3]-0.16*TIME)*TIME*TIME)/20000;
     if(xd[3]*xd[3]+yd[3]*yd[3]>0.81) 
	  xd[3]=2;

	 //����5
    xd[4]=0.9f*(10000+vox[4]*TIME)/20000;
	yd[4]=0.9f*(-20000+voy[4]*TIME)/20000;
    if(xd[4]*xd[4]+yd[4]*yd[4]>0.81) 
	  xd[4]=2;

	//����6
    xd[5]=0.9f*(20000+vox[5]*TIME)/20000;
	yd[5]=0.9f*(-20000+voy[5]*TIME)/20000;
    if(xd[5]*xd[5]+yd[5]*yd[5]>0.81) 
	  xd[5]=2;

	//����7
    xd[6]=0.9f*(8000+vox[6]*TIME+0.5*ax[6]*TIME*TIME)/20000;
	yd[6]=0.9f*(8000+voy[6]*TIME+0.5*ay[6]*TIME*TIME)/20000;
    if(xd[6]*xd[6]+yd[6]*yd[6]>0.81) 
	  xd[6]=2;

	//����8
    xd[7]=0.9f*(-10000+vox[7]*TIME)/20000;
	yd[7]=0.9f*(20000+voy[7]*TIME)/20000;
    if(xd[7]*xd[7]+yd[7]*yd[7]>0.81) 
	  xd[7]=2;
    //����9
	xd[8]=0.9f*(-10000+vox[8]*TIME)/20000;
	yd[8]=0.9f*(-10000+0.5*ay[8]*TIME*TIME)/20000;
    if(xd[8]*xd[8]+yd[8]*yd[8]>0.81) 
	  xd[8]=2;

     

  glColor3f(1.0f, 1.0f, 0.0f); //��ɫ
     glPointSize(1.0);    //���õ�Ĵ�С     

	 for(j=0;j<=i;j++)
{ 
	 glBegin(GL_POINTS);   //����
         for(a=0;a<=8;a++)
		 {
            glVertex2f(list[a][0][j],list[a][1][j]);
		 }
    
glEnd();

 }


 
	if(i%51==0&&i!=0)
	{
 //glNewList(BOX,GL_COMPILE_AND_EXECUTE);   //���沢��ʾ
 glNewList(BOX,GL_COMPILE);  //������ʾ�б�
for(j=0;j<=i;j++)
{
	glBegin(GL_POINTS);   //����
      for(a=0;a<=8;a++)
	  {
       glVertex2f(list[a][0][j],list[a][1][j]);
	  }
glEnd();   }

glEndList();

	}

for(j=1;j<=BOX;j++)
{
glCallList(j);

}

   // ��С����
    
	for(j=0;j<=8;j++)
	{

		glColor3f(1.0f, 0.0f, 0.0f);
		glRectd (list[j][0][i-1]+0.02,list[j][1][i-1]-0.02,list[j][0][i-1]+0.08,list[j][1][i-1]-0.08);
	


	    glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2f(list[j][0][i-1]+0.03,list[j][1][i-1]-0.07);  //����λ��
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,49+j);  //д���    1-9��49-57
	
	}




 
//////////////////��ɨ��//////////////////	

	 
	 glLineWidth(1.5);    //�����߿�
	 glColor3f(1.0f, 1.0f, 1.0f);
	 
	 q=(0.9f)*sin(dx);   
     p=(0.9f)*cos(dx);
    // glPushMatrix();    // ����Ŀǰ�������D�Q���	 
	 glBegin(GL_LINES);   //����
	 glVertex2f(0.0f,0.0f);  
	 glVertex2f(q,p);	 
	 glEnd();
	// glPopMatrix(); // �����������D�Q�ᣬ�֏͞�ԭ�ȵ������D�Q���

  glutSwapBuffers();

	// Flush drawing commands
//  glFlush();
	}

///////////////////////////////////////////////////////////
// Setup the rendering state
void SetupRC(void)
    {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    }

///////////////////////////////////




///////////////////////////////////////////////////////////

void TimerFunction(int value)
    {

	int b;
   
       
      dx+=0.0628f;
	  if(dx >GL_PI*2)
		  dx=0.0f;
  
        TIME+=0.01;
          n+=1;

        if(i>50)
		 i=0;
	 
	 if(n>100)
	 {n=0;
	 
   for(b=0;b<=8;b++)
	 {
 list[b][0][i]=xd[b];
 list[b][1][i]=yd[b];
     }

     i+=1;
	 BOX+=1;

	 }
	 

     // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(10,TimerFunction, 1);
    }


///////////////////////////////////////////////////////////
// Called by GLUT library when the window has chanaged size

///////////////////////////////////////////////////////////
// Main program entry point
void main(int argc, char* argv[])
	{
	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   
    glutInitWindowSize(600,600);
   
	glutCreateWindow("SRadar");
   
	glutTimerFunc(10,TimerFunction, 1);
	SetupRC();
	glutDisplayFunc(RenderScene);

	

	glutMainLoop();
    }

