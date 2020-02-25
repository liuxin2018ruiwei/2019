/********************************************************************
* Name      :  MemoryNode.h                                  
* Describ   :  the class used to get a chunk of memory, and releas memory
*              when destoried                                        
* Author    :  Zhang Botong                                          
* Date      :  2016-10-23                                                 
********************************************************************/

#ifndef MEMORYNODE_H
#define MEMORYNODE_H

class MemoryNode
{
public: 
 MemoryNode();
 MemoryNode( int nLen );
 char * getMemory();
 int getMemoryLen();
 int setMemoryLen( int nLen );
 virtual ~MemoryNode();
protected:
private:
 char * m_MemPtr;
 int m_MemLen;
};
#endif
