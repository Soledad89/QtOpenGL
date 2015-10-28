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
	char* pBuffer = frameInfo->pBuffer;	// ͼ�����ݵ�ַ	
	long nSize = frameInfo->nSize;		// ͼ�����ݳ���	

	
//	if(RecRGBCallBack!=NULL)//�ж��Ƿ��лص�����
//	{
		unsigned char * BGRBuffer=new unsigned char [nWidth*nHeight*3];//����BGRͼ���ڴ�
		//ͼ��ת��ΪRGB
		//YUV420toRGB((unsigned char *)pBuffer,BGRBuffer,nWidth,nHeight,true);//ͼ�������·�ת
		//ͼ��ת��ΪBGR
		YUV420toRGB((unsigned char *)pBuffer,BGRBuffer,nWidth,nHeight,true);//ͼ�������·�ת
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
		delete(BGRBuffer);//����RGB�ڴ�
//		RecRGBCallBack( *od);//ִ�лص�����

//	}	
	//ת���������ɶ������ڴ˴����
}