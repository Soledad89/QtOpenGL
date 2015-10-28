//Name:   MyPackage.h
//Data:   Sep 6, 2011
//Author: GuoYan in Dongzhulou Building 10-409 , Tsinghua

#include <GL/glew.h>


#ifndef _BMP_H_
#define _BMP_H_

struct BMPImageRec{
    GLuint sizeX, sizeY;
    GLubyte *data;
};

BMPImageRec * loadBMP(const char * fileName);


#endif