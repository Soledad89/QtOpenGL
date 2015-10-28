//////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ݴ���ӿ�
//     ���ļ��������ݴ���ӿڣ��û�ʵ�ִ˽ӿڵĴ��麯��DoInputData��lib��ͨ�����ô˽ӿڵ�ʵ�ֵĴ˺�
//     �����ݽ���������
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
