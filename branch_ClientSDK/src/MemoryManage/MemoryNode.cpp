/********************************************************************
* Name      :  MemoryNode.cpp                                  
* Describ   :  the defination of the class                                                      
* Author    :  Zhang Botong                                          
* Date      :  2016-10-23                                                 
********************************************************************/

#include "MemoryNode.h"
#include "MemoryManager.h"

MemoryNode::MemoryNode():m_MemPtr(NULL), m_MemLen( 0 )
{
}

MemoryNode::MemoryNode( int nLen ):m_MemPtr(NULL), m_MemLen( nLen )
{
	setMemoryLen( nLen );
}

char * MemoryNode::getMemory()
{
	return m_MemPtr;
}
int MemoryNode::getMemoryLen()
{
	return m_MemLen;
}

int MemoryNode::setMemoryLen( int nLen )
{
	int nRet = -1;
	if( nLen < 0 )
	{
		return nRet;
	}

	if( NULL == m_MemPtr )
	{
		m_MemPtr = MemoryManager::getInstance()->getMemory( nLen );
		if( NULL != m_MemPtr )
		{
			m_MemLen = nLen;
			nRet = 0;
		}
		else
		{
			m_MemLen = 0;
		}
	}
	else
	{
		if( m_MemLen < nLen )
		{
			MemoryManager::getInstance()->releaseMemory( m_MemPtr );
			
			m_MemPtr = MemoryManager::getInstance()->getMemory( nLen );
			if( NULL != m_MemPtr )
			{
				m_MemLen = nLen;
				nRet = 0;
			}
			else
			{
				m_MemLen = 0;
			}
			
		}
		else
		{
			nRet = 0;
		}
	}

	return nRet;
		
}

MemoryNode::~MemoryNode()
{
	if( NULL != m_MemPtr )
	{
		MemoryManager::getInstance()->releaseMemory( m_MemPtr );
	}
}
