
#include "DataRecver.h"
#include "CGraphicsManager.h"

class NewDataRecver:public DataRecver
{
public:
	NewDataRecver(CGraphicsManager* pGraphicsManager,char ind,BMPImageRec *bmp);
	~NewDataRecver();
	bool unFrost;
	BMPImageRec *od;
	unsigned char * BGRBuffer;
private:
	char index;
	virtual void DoInputData(const char * buf, unsigned int size );//������ִ����Ҫ���Ĺ���
	CGraphicsManager* pGraphicsManager;
};