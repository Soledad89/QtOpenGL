#pragma once

typedef enum { YUV420_VIMICRO } FrameTypeVimicro;

/**
   解码回调的数据结构
 */
typedef struct _FRAME_INFO_VIMICRO
{
	long nWidth;	/**图像宽度*/
	long nHeight;	/**图像高度*/
	unsigned long long nTimestamp;/**时戳*/
	long nType;		/**帧类型*/
	long nFrameRate;/**帧率*/
	unsigned int nFrameId;	/**帧id*/	
	char* pBuffer;/* 数据缓冲区 */	
	long nSize;/* 数据长度 */
}FRAME_INFO_VIMICRO;