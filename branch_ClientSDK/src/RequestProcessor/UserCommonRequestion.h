/********************************************************************
* Name      :  UserCommonRequestion.h                                  
* Describ   :  the class is used to save some common requestion from user
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#ifndef USERCOMMONREQUESTION_H
#define USERCOMMONREQUESTION_H

#include "Requestion.h"

class UserCommonRequestion:public Requestion
{
public: 
  UserCommonRequestion( UINT64 usrReqID, ReqID id );
  UserCommonRequestion( UINT64 usrReqID, ReqID id, Priority pro );

  UINT64 getUserReqID();
  virtual ~UserCommonRequestion();
 protected:
 private:
  UINT64 m_usrReqID;
};
#endif
