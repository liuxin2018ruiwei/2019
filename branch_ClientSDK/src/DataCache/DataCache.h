/********************************************************************
* Name      :  DataCache.h                                  
* Describ   :  the class is used to manage some cache,create or destory them
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/

#ifndef DATACACHE_H
#define DATACACHE_H

#include "DetectFaceIFCommon.h"
class DataCache
{
 public: 
  static DataCache * getInstance();

  int initDataCache( RWImgInfo *FrameInfo );
  int uninitDataCache();

  virtual ~DataCache();
 protected:
  DataCache();
 private:
  static DataCache * m_this;

};
#endif
