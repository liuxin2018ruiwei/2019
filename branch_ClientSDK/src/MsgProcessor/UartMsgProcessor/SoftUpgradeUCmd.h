/********************************************************************
* Name      :  SoftUpgradeUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x0101
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef SOFT_UPGRADE_U_CMD_H
#define SOFT_UPGRADE_U_CMD_H

#include "UartCommand.h"


class SoftUpgradeUCmd: public UartCommand
{
 public: 
  SoftUpgradeUCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~SoftUpgradeUCmd();
 protected:
 private:
};
#endif
