/********************************************************************
* Name      :  Driver02_10_33.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#ifndef DRIVER02_10_33_H
#define DRIVER02_10_33_H

#include "Driver.h"

class Driver02_10_33:public Driver
{
public: 
 Driver02_10_33( unsigned int nCaseId );
  virtual int init();
  virtual int run();
  virtual int uninit();
  virtual ~Driver02_10_33();
 protected:
 private:
 void LXTestWBS_02_10_33();
 int processMsg(UINT8 *pMsg, int nLen );
 unsigned short int readInt16(const char* data);
 int readInt32(const char* data);
 void writeInt16(char* data, short int value);
 void writeInt32(char* data, int value);
};
#endif
