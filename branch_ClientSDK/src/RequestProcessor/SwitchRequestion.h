/********************************************************************
* Name      :  SwitchRequestion.h                                  
* Describ   :  this function is use as Requestion,to get more data
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#ifndef SWITCHREQUESTION_H
#define SWITCHREQUESTION_H

#include "Requestion.h"

class SwitchRequestion:public Requestion
{
public: 
  SwitchRequestion( int flag, ReqID id );
  SwitchRequestion( int flag, ReqID id, Priority pro );

  UINT64 getUserFlag();
  virtual ~SwitchRequestion();
 protected:
 private:
  int m_flag;
};
#endif
