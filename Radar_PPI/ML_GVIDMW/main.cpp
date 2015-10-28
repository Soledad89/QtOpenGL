#include "ML_GVIDMW.h"

int main()
{
	ML_GVID_Init();
	ML_GVID_OpenPPI(1, 0, 0, 300, 300); 
	return 0;
}