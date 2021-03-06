/********************************************************************
* Name      :  Driver02_10_17.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#ifndef DRIVER02_10_17_H
#define DRIVER02_10_17_H

#include "Driver.h"

class Driver02_10_17:public Driver
{
public: 
 Driver02_10_17( unsigned int nCaseId );
  virtual int init();
  virtual int run();
  virtual int uninit();
 virtual ~Driver02_10_17();
protected:
private:
 void LXTestWBS_02_10_17();
 static void testTest(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,UINT32 *pDataLen);

};
#endif
