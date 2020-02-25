/********************************************************************
* Name      :  DriverTestCommunicate.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#ifndef DRIVER_TESTCOMMUNICATE_H
#define DRIVER_TESTCOMMUNICATE_H

#include "Driver.h"

class DriverTestCommunicate: public Driver
{
public: 
  DriverTestCommunicate(unsigned int nCaseId );
  virtual int init();
  virtual int run();
  virtual int uninit();
  virtual ~DriverTestCommunicate();
 protected:
 private:
};
#endif
