/********************************************************************
* Name      :  SoftUpgradeAppCfgCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x231
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef SOFT_UPGRADE_APP_CFG_CMD_H
#define SOFT_UPGRADE_APP_CFG_CMD_H

#include "UartCommand.h"

class SoftUpgradeAppCfgCmd: public UartCommand
{
 public:  
  SoftUpgradeAppCfgCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~SoftUpgradeAppCfgCmd();
 protected:
 private:
};
#endif
