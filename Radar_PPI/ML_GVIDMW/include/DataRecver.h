//////////////////////////////////////////////////////////////////////////////////////////////////////
// 数据处理接口
//     此文件定义数据处理接口，用户实现此接口的纯虚函数DoInputData，lib库通过调用此接口的实现的此函
//     数传递解码后的数据
//////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

class DataRecver
{
public:
	DataRecver(void) { isFramePacket_ = true; };
	virtual ~DataRecver(void) {};

	void InputData( const char * buf, unsigned int size );

	void SetFramePacketFlag( bool isFramePacket ) { isFramePacket_ = isFramePacket; };
	bool GetFramePacketFlag () const { return isFramePacket_; };

private:
	virtual void DoInputData( const char * buf, unsigned int size ) = 0;

private:
	bool isFramePacket_;
};
