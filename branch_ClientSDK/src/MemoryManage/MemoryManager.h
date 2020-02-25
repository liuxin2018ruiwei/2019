/********************************************************************
* Name      :  MemoryManager.h                                  
* Describ   :  the class used to manager the large chunk of memory
* Author    :  Zhang Botong                                          
* Date      :  2016-10-23                                                 
********************************************************************/

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "RecoLock.h"
#include<list>

class MemoryManager
{
 public:
  typedef struct _strMemBlock
  {
	char * MemHeader;
	int MemLen;
  }MemBlock;

  typedef std::list< MemBlock > MemList;
  typedef MemList::iterator MemListItr;

  static const int MEM_BLOCK_LEN_MIN = 2000000;
  static MemoryManager * getInstance();

  char *getMemory( int nLen );

  int initMemory( int nLen=MEM_BLOCK_LEN_MIN );

  int releaseMemory(char *pMem );

  int uninitMemory();

  virtual ~MemoryManager();
 protected:
  int insertFreeMemNode( MemBlock &block );
  MemoryManager();
 private:
  static MemoryManager *m_instance;
  char *m_pMemPtr;
  MemList m_MemFree;
  MemList m_MemUsed;
  RecoLock m_MemLock;
  bool m_initFlg;
};
#endif
