#pragma once

typedef enum { YUV420_VIMICRO } FrameTypeVimicro;

/**
   ����ص������ݽṹ
 */
typedef struct _FRAME_INFO_VIMICRO
{
	long nWidth;	/**ͼ����*/
	long nHeight;	/**ͼ��߶�*/
	unsigned long long nTimestamp;/**ʱ��*/
	long nType;		/**֡����*/
	long nFrameRate;/**֡��*/
	unsigned int nFrameId;	/**֡id*/	
	char* pBuffer;/* ���ݻ����� */	
	long nSize;/* ���ݳ��� */
}FRAME_INFO_VIMICRO;