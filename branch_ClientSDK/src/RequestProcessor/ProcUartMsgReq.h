/********************************************************************
* Name      :  ProcUartMsgReq.h                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-13                                                 
********************************************************************/

#ifndef PROCUARTMSGREQ_H
#define PROCUAARTMSGREQ_H

#include "BaseReq.h"

class ProcUartMsgReq:public BaseReq
{
 public: 
  ProcUartMsgReq();
  virtual int processReq( Requestion *pReq );	
  virtual ~ProcUartMsgReq();
 protected:
 private:
};
#endif
