/********************************************************************
* Name      :  MemoryManager.cpp                                  
* Describ   :  the defination of the class 
* Author    :  Zhang Botong                                          
* Date      :  2016-10-23                                                 
********************************************************************/

#include"MemoryManager.h"
#include"MemoryNode.h"
#include <stdio.h>

MemoryManager * MemoryManager::m_instance = NULL;

MemoryManager::MemoryManager():m_initFlg(false), m_pMemPtr( NULL )
{
}

MemoryManager * MemoryManager::getInstance()
{
	if( NULL == m_instance )
	{
		m_instance = new MemoryManager;
	}

	return m_instance;
}

char *MemoryManager::getMemory( int nLen )
{
	char * pMem = NULL;
	if( nLen < 0 )
	{
		return pMem;
	}

	RecoLockGuard autolock( m_MemLock );
	MemListItr itr = m_MemFree.begin();
	for( ; itr != m_MemFree.end(); itr++ )
	{
		if( nLen <= itr->MemLen  )
		{
			pMem = itr->MemHeader;

			// add used-Block into used-list
			MemBlock usedBlock;
			usedBlock.MemHeader = itr->MemHeader;
			usedBlock.MemLen = nLen;
			m_MemUsed.push_back( usedBlock );
//			insertUsedMemNode( usedBlock );

			// add new free-block into the free-list
			MemBlock freeBlock;
			freeBlock.MemHeader = usedBlock.MemHeader + nLen;
			freeBlock.MemLen = itr->MemLen - nLen;

			// remove the block from free-list
			m_MemFree.erase( itr );
			
			insertFreeMemNode( freeBlock );

			break;
		}
	}

	return pMem;
	
}

int MemoryManager::initMemory( int nLen )
{
	if( true == m_initFlg )
	{
		return 0;
	}
	
	if( nLen < MEM_BLOCK_LEN_MIN )
	{
		nLen = MEM_BLOCK_LEN_MIN;
	}

	m_pMemPtr = new char[nLen];
	MemBlock block;
	block.MemHeader = m_pMemPtr;
	block.MemLen = nLen;

	m_MemFree.push_back( block );

	m_initFlg = true;

	return 0;
}

int MemoryManager::insertFreeMemNode( MemBlock &block )
{
	MemListItr preItr = m_MemFree.end();
	MemListItr nextItr = m_MemFree.end();
	MemListItr itr = m_MemFree.begin();
	for( ; itr != m_MemFree.end(); itr++ )
	{
		if( itr->MemHeader < block.MemHeader )
		{
			preItr = itr;
		}
		else if( itr->MemHeader > block.MemHeader )
		{
			nextItr = itr;
			break;
		}
		else
		{
			// NOP impossible
		}
	}

	// P: the memory belongs to  preItr
	// B: the memory belongs to block
	// N: the memory belongs to nextItr
	// -: the memory in using
	if( (m_MemFree.end() != preItr) && (m_MemFree.end() != nextItr ) )
	{
		// ---<PPPPPPPPPPPPPPPP|BBBBBBBBBBBBBBBBBB|NNNNNNNNNNNNNNNNNNNNNNNNNNNN>---
		if( (preItr->MemHeader + preItr->MemLen + block.MemLen ) == nextItr->MemHeader )
		{
			preItr->MemLen = preItr->MemLen + block.MemLen + nextItr->MemLen;
			m_MemFree.erase( nextItr );
		}
		// ---<PPPPPPPPPPPPPPPP|BBBBBBBBBBBBBBBBB>-----------|NNNNNNNNNNNNNNNNN|---
		else if( ( preItr->MemHeader + preItr->MemLen ) == block.MemHeader )
		{
			preItr->MemLen += block.MemLen;
		}
		// ---|PPPPPPPPPPPPPPPP|-----------<BBBBBBBBBBBBBBBBB|NNNNNNNNNNNNNNNNN>---
		else if( (block.MemHeader + block.MemLen ) == nextItr->MemHeader )
		{
			nextItr->MemHeader = block.MemHeader;
			nextItr->MemLen += block.MemLen;
		}
		// ---|PPPPPPPPPPPPPPPP|-----|BBBBBBBBBBBBBBBBB|-----|NNNNNNNNNNNNNNNNN|---
		else
		{
			m_MemFree.insert( nextItr, block );
		}
	}
	else if(m_MemFree.end() != nextItr)
	{
		// <BBBBBBBBBBBBBBBBB|NNNNNNNNNNNNNNNNN>---
		if( (block.MemHeader + block.MemLen) == nextItr->MemHeader )
		{
			nextItr->MemHeader = block.MemHeader;
			nextItr->MemLen += block.MemLen;
		}
		// <BBBBBBBBBBBBBBBBB|------|NNNNNNNNNNNNNNNNN>---
		else
		{
			m_MemFree.insert( nextItr, block );			
		}
	}
	else if( m_MemFree.end() != preItr)
	{
		// ------<PPPPPPPPPPPPPPPP|BBBBBBBBBBBBBBBBB>
		if( (preItr->MemHeader + preItr->MemLen ) == block.MemHeader )
		{
			preItr->MemLen += block.MemLen;
		}
		// ------<PPPPPPPPPPPPPPPP|-----|BBBBBBBBBBBBBBBBB>
		else
		{
			m_MemFree.push_back( block );
		}
	}
	// |BBBBBBBBBBBBBBBBB|
	else // the condition is preItr=end(), nextItr=end()
	{
		m_MemFree.push_back( block );
	}
	return 0;
}

int MemoryManager::releaseMemory(char *pMem )
{
	if( NULL == pMem )
	{
		return -1;
	}

	RecoLockGuard autolock( m_MemLock );
	MemListItr itr = m_MemUsed.begin();
	for( ; itr != m_MemUsed.end(); itr++ )
	{
		if( pMem == itr->MemHeader  )
		{
			MemBlock freeBlock;
			freeBlock.MemHeader = itr->MemHeader;
			freeBlock.MemLen = itr->MemLen;
			
			insertFreeMemNode( freeBlock );

			m_MemUsed.erase( itr );
			break;
		}
	}

//	std::cout << "MemoryManager::releaseMemory the free memory =" << m_MemFree.size()
//			  << " used memory=" << m_MemUsed.size() << std::endl;
	return 0;
}

int MemoryManager::uninitMemory()
{
	if( false == m_initFlg )
	{
		return 0;
	}

	if( m_MemUsed.size() > 0 )
	{
		return -1;
	}

	delete m_pMemPtr;
	m_initFlg = false;
	return 0;
}

MemoryManager::~MemoryManager()
{
	uninitMemory();
}

