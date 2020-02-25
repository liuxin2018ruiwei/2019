#include "Observer.h"

const int Observer::UPDATE_OK  = 0;
const int Observer::UPDATE_NG =-1;
const int Observer::UPDATE_BUSY =-2;

Observer::Observer()
{
}


Observer::~Observer()
{
}


// pData (in) the notify info will be contained in the space of pData return OK 0; NG -1;
int Observer::update(void* pData)
{
	return UPDATE_OK;
}
