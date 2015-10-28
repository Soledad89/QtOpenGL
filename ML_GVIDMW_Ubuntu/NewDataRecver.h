
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
	virtual void DoInputData(const char * buf, unsigned int size );//这里面执行需要做的工作
	CGraphicsManager* pGraphicsManager;
};