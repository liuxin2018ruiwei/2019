/********************************************************************
* Name      :  UartCommandCtrl.h                                  
* Describ   :  the class is used to control the creation of commands about Uart
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef UARTCOMMANDCTRL_H
#define UARTCOMMANDCTRL_H

#include <map>
#include "UartCommand.h"
#include "UartCommandType.h"

class Message;
class UartCommandCtrl
{
 public: 
  typedef char InitInfo;
  typedef std::map< UartCommandCode::CodeType, UartCommand* > UartCommandMap;
  typedef UartCommandMap::iterator CmdMapItr;
  UartCommandCtrl();
  int initUartCommandCtrl( InitInfo *pInfo );
  int processMsg( Message * pMsg );
  void unInitUartCommandCtrl();

  virtual ~UartCommandCtrl();
 protected:
 private:
  UartCommandMap m_cmdMap;
};
#endif
