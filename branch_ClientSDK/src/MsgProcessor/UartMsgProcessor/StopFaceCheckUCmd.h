/********************************************************************
* Name      :  StopFaceCheckUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x0101
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef STOP_FACE_CHECK_UCMD_H
#define STOP_FACE_CHECK_UCMD_H

#include "UartCommand.h"

class StopFaceCheckUCmd: public UartCommand
{
 public: 
  StopFaceCheckUCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~StopFaceCheckUCmd();
 protected:
 private:
};
#endif
