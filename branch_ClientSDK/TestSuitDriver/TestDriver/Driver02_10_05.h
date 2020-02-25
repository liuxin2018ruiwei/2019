/********************************************************************
* Name      :  Driver02_10_05.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#ifndef DRIVER02_10_05_H
#define DRIVER02_10_05_H

#include "Driver.h"

class Driver02_10_05:public Driver
{
public: 
 Driver02_10_05( unsigned int nCaseId );
  virtual int init();
  virtual int run();
  virtual int uninit();
 virtual ~Driver02_10_05();
 protected:
 private:
 void LXTestWBS_02_10_05();
};
#endif
