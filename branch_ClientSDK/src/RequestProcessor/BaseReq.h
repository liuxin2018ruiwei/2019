/********************************************************************
* Name      :  BaseReq.h                                  
* Describ   :  the class is used to declare some interface.
* Author    :  Zhang Botong                                          
* Date      :  2018-06-12                                                 
********************************************************************/

#ifndef BASEREQ_H
#define BASEREQ_H

#include "Requestion.h"

class BaseReq
{
 public: 
  BaseReq(Requestion::ReqID id);
  virtual int processReq( Requestion *pReq );
  virtual Requestion::ReqID getReqID();
  virtual ~BaseReq();
 protected:
 private:
  Requestion::ReqID m_id;
};
#endif
