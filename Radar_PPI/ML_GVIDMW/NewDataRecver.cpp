#include "yuvrgb24.h"
#include "FrameInfoDefine.h"
#include "NewDataRecver.h"


NewDataRecver::NewDataRecver(CGraphicsManager* pG,char ind, BMPImageRec *bmp)
{
	pGraphicsManager = pG;
	index = ind;
	unFrost = 1;
	od = new BMPImageRec;
//	unsigned char * BGRBuffer=new unsigned char [704*576*3];
}

NewDataRecver::~NewDataRecver()
{
}

void NewDataRecver::DoInputData(const char * buf, unsigned int size )
{
	const char * p = buf;
		
	unsigned int len = size;	
	FRAME_INFO_VIMICRO* frameInfo = (FRAME_INFO_VIMICRO*)p;	
	long type = frameInfo->nType;	
	unsigned long long nTimestamp = frameInfo->nTimestamp;	
	long nFrameRate = frameInfo->nFrameRate;	
	unsigned int nFrameId = frameInfo->nFrameId;	
	long nWidth = frameInfo->nWidth;	
	long nHeight = frameInfo->nHeight;
	char* pBuffer = frameInfo->pBuffer;	// 图像数据地址	
	long nSize = frameInfo->nSize;		// 图像数据长度	

	
//	if(RecRGBCallBack!=NULL)//判断是否有回调函数
//	{
		unsigned char * BGRBuffer=new unsigned char [nWidth*nHeight*3];//开辟BGR图像内存
		//图像转码为RGB
		//YUV420toRGB((unsigned char *)pBuffer,BGRBuffer,nWidth,nHeight,true);//图像需上下翻转
		//图像转码为BGR
		YUV420toRGB((unsigned char *)pBuffer,BGRBuffer,nWidth,nHeight,true);//图像需上下翻转
		od->sizeX=(GLuint)nWidth;
		od->sizeY=(GLuint)nHeight;
		od->data=(GLubyte *)BGRBuffer;
//		memcpy(od->data,BGRBuffer,nWidth*nHeight*3);
		if (unFrost)
		{
			pGraphicsManager -> drawBackMap(index+7, od);
			pGraphicsManager -> copyTexFBO(index+7, 3, index+7, 2);
			pGraphicsManager -> copyTexFBO(index+7, 3, index+7, 1);
			pGraphicsManager -> copyTex(index+7, index+7, 3);
			pGraphicsManager -> update(index+7);
		}
		delete (od);
		delete(BGRBuffer);//销毁RGB内存
//		RecRGBCallBack( *od);//执行回调函数

//	}	
	//转码后所需完成动作请在此处完成
}