/********************************************************************
* Name      :  StartFaceCheckUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x0101
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef STARTFACECHECKUCMD_H
#define STARTFACECHECKUCMD_H

#include "UartCommand.h"

class StartFaceCheckUCmd: public UartCommand
{
 public: 
  StartFaceCheckUCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~StartFaceCheckUCmd();
 protected:
 private:
};
#endif
