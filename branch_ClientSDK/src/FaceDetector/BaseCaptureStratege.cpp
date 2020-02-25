/********************************************************************
* Name      :  BaseCaptureStratege.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                    
* Author    :  liuxin                                        
* Date      :  2018-06-25                                                 
********************************************************************/
#include <stdio.h>
#include "BaseCaptureStratege.h"


int BaseCaptureStratege::init(void *priData )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif	
	
	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	return 0;
}

int BaseCaptureStratege::captureFace(FaceNode *pFaceNode)
{
	return 0;
}


BaseCaptureStratege::~BaseCaptureStratege()
{
}


BaseCaptureStratege::BaseCaptureStratege()
{
}
