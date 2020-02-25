/********************************************************************
* Name      :  setBaudrateRequestion.h                                  
* Describ   :  the class is used to save some common requestion from user
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#ifndef setBaudrateRequestion_H
#define setBaudrateRequestion_H

#include "Requestion.h"

class setBaudrateRequestion:public Requestion
{
public: 
  setBaudrateRequestion( UINT32 setBaudrate, ReqID id );
  setBaudrateRequestion( UINT32 setBaudrate, ReqID id, Priority pro );

  UINT64 getBaudRate();
  virtual ~setBaudrateRequestion();
 protected:
 private:
  UINT64 m_baudrate;
};
#endif
