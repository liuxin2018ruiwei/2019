#include <stdio.h>
#include "Protocol.h"
#include "StubProtocol.h"

StubProtocol* StubProtocol::m_this = nullptr;

StubProtocol* StubProtocol::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new StubProtocol;
	}

	return m_this;
}
StubProtocol::StubProtocol()
{
}

// check the data if the data is corrent
// pData	(in) the data want to check
// nLen		(in) the length of the data
// return	(out) 
//			READ_DATA_CHECK_OK 
//			READ_DATA_cHECK_HEADER_NG -1
//			READ_DATA_CHECK_SUM_NG -2
int StubProtocol::Check(unsigned char* pData, int nLen)
{
	return READ_DATA_CHECK_OK;
	
}


// clean up the buffer hold by m_fd
// return  (out) the result of the last read
int StubProtocol::cleanupFD()
{
	int nRet = 0;

	return nRet;
}


// return (out) the fd of the file
int StubProtocol::getFD()
{
	return 0;
}

int StubProtocol::init( void * priData )
{
	printf("StubProtocol::init--");
	return RET_OK;
}

// return (out) 0 or error code
//init the protocol obj before start to use it
int StubProtocol::init()
{
	//printf("StubProtocol::init start");
	//m_pReply = new unsigned char[READ_HEADER_SIZE + 1];
	return RET_OK;
}

// pBuffer (out) the buffer to save the data read
// nBuffLen (in) the length of data want to read
// nReadLen (out) the length of data actually read
// return (out)RET_NG time out or error happened
//				RET_OK read ok
int StubProtocol::readFD(unsigned char* pBuffer, int nBufferLen, int& nReadLen)
{
	return 0;
}
// pBuffer (out) the pointer of the buffer, if received data ,save into it
// nLen (in) the length of the buffer
// return (out) the length of the data be saved
int StubProtocol::receiveData( unsigned char* pBuffer, int nLen )
{
	return 0;
}

// reply the message to notify it with error code
// ErrorCode	(in) the code to reply the message
// pData		(in) the pointer of the data which have some mistake, and have to reply
// nLen			(in) the length of the data
int StubProtocol::replyError(short ErrorCode, unsigned char* pData, int nLen)
{
//	printf(" StubProtocol::replyError CMD=0x%X, Error=0x%X\n",cmd_code, ErrorCode);

	return RET_OK;
}


// pData (in) the pointer of the data  to send ;
// nDataLen (in) the length of the data
int StubProtocol::saveData(unsigned char* pData, int nDataLen)
{
	return RET_OK;
}


// pData (in) the pointer of the data to send
// nLen (in) the length of the data
// return (out) the data have send
int StubProtocol::sendData( unsigned char* pData, int nLen)
{
	printf("STUB Protocol::sendData (%d) :\n", nLen);

	for(int i = 0; i < nLen; i++)
    {
        printf("%02x ",*(unsigned char*)(pData + i));
    }
    printf("\n");
	

	return 0;

}

// set the check sum into the data ,and save at the end of it
// pData	(in/out) the source data to get check sum and the destination to set checksum
// nLen		(in) the length of the data
int StubProtocol::setCheckSum(unsigned char* pData, int nLen)
{
	return 0;
}

// fd (in) the fd of the file
int StubProtocol::setFD(int fd)
{

 	return RET_OK;
}

int StubProtocol::setSendQueue(SendMessageQueue * queue)
{
	return RET_OK;
}

StubProtocol::~StubProtocol()
{
	//delete m_pReply;
}

