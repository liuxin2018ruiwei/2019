/********************************************************************
* Name      :  HeartBeatUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x0101
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef HEARTBEATUCMD_H
#define HEARTBEATUCMD_H

#include "UartCommand.h"

class HeartBeatUCmd: public UartCommand
{
 public: 
  HeartBeatUCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~HeartBeatUCmd();
 protected:
 private:
};
#endif
