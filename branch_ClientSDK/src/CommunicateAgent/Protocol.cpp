#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
//#include "FaceDoorError.h"
#include "Protocol.h"
//#include "log.h"
#include "MessageQueue.h"
#include "Version.h"

#ifdef FUNCTION_TEST
#include "StubProtocol.h"
#endif

Protocol::Protocol():m_fd(FD_INIT)
{
}

// check the data if the data is corrent
// pData	(in) the data want to check
// nLen		(in) the length of the data
// return	(out) 
//			READ_DATA_CHECK_OK 
//			READ_DATA_cHECK_HEADER_NG -1
//			READ_DATA_CHECK_SUM_NG -2
int Protocol::Check(unsigned char* pData, int nLen)
{
	if (NULL == pData) 
	{
		printf("Protocol::Check Params Invalid\n");
		return READ_DATA_CHECK_OTHER;
	}

	unsigned char nSum = 0;
	unsigned int nHeaderSum = 0;


//printf("nLen = %d\n",nLen);
	for (int nCnt = 0; nCnt < READ_HEADER_SIZE; nCnt++)//nLen
	{
		if (nCnt < READ_HEADER_SIZE)
		{
			nHeaderSum += ((pData[nCnt] & PROTOCOL_HEADER_MASK[nCnt])\
						   ^ PROTOCOL_HEADER_FORMATE[nCnt]);
			 //printf("pData[nCnt]=0x%X, nHeaderSum = 0x%X",pData[nCnt], nHeaderSum);
		}
		nSum += pData[nCnt];
	}

	// it's means there's not a  completed  header that the nLen <READ_HEADER_SIZE + 1
	if (( nLen < READ_HEADER_SIZE ) ||( nHeaderSum != 0) )
	{
		printf("ERR Protocol::Check nHeaderSum RET_NG,nHeaderSum = %d,( nLen < READ_HEADER_SIZE ) = %d\n",nHeaderSum,( nLen < READ_HEADER_SIZE ));
		
		return READ_DATA_CHECK_HEADER_NG;
	}

	if (nSum != 0)
	{
	//	printf("ERR  Protocol::Check nSum RET_NG\n");
		
		//return READ_DATA_CHECK_SUM_NG;
	}
	//printf(" Protocol::Check OK\n");
	return READ_DATA_CHECK_OK;
	
}


// clean up the buffer hold by m_fd
// return  (out) the result of the last read
int Protocol::cleanupFD()
{
	int nRet = 0;
	unsigned char buffer[20]={'\0'};
	do
	{
		nRet = read(m_fd, buffer, sizeof(buffer));
	}while(nRet > 0);


	return nRet;
}

#if 0
// sData(out) get the data to send which will save in sData
int Protocol::getDataToSend(std::string& sData)
{
	sData.clear();

	// if there are message to send , send them
	
	if (0 == sem_trywait(&m_MsgAvailable))
	{
		pthread_mutex_lock(&m_SendMutex);
		sData = m_Msgs.front();
		m_Msgs.pop_front();
		pthread_mutex_unlock(&m_SendMutex);
		sem_post(&m_ListAvailable);
	}


	return RET_OK;
}
#endif

// return (out) the fd of the file
int Protocol::getFD()
{
	return m_fd;
}

int Protocol::init( void * priData )
{
	printf("Protocol::init--");
	return RET_OK;
}

// return (out) 0 or error code
//init the protocol obj before start to use it
int Protocol::init()
{
	//printf("Protocol::init start");
#if 0	
	int err = pthread_mutex_init(&m_SendMutex, NULL);
	if (err != 0) 
	{
		printf("Protocol::init pthread_mutex_init error: %s\n", strerror(err));
		return RET_NG;
	}
	err = sem_init(&m_MsgAvailable, 0, 0);
	if (err != 0)
	{
		printf("Protocol::init sem_init error: %s\n", strerror(err));
		return RET_NG;
	}
	err = sem_init(&m_ListAvailable, 0, SEND_MESSAGE_BUFF_MAX);
	if (err != 0)
	{
		printf("Protocol::init sem_init error: %s\n", strerror(err));
		return RET_NG;
	}
#endif
	//m_pReply = new unsigned char[READ_HEADER_SIZE + 1];
	return RET_OK;
}

// pBuffer (out) the buffer to save the data read
// nBuffLen (in) the length of data want to read
// nReadLen (out) the length of data actually read
// return (out)RET_NG time out or error happened
//				RET_OK read ok
int Protocol::readFD(unsigned char* pBuffer, int nBufferLen, int& nReadLen)
{
	int nRemain = nBufferLen;

	nReadLen = 0;
	do
	{
		fd_set ReadFdList;
              
		FD_ZERO(&ReadFdList);
              
		FD_SET(m_fd , &ReadFdList);
		struct timeval waitTime = { 0, SELECT_WAIT_TIME };
              
		// selecte the data and wait until the fd is available
		int nRet = select(m_fd + 1, &ReadFdList, NULL, NULL, &waitTime);
              
		// if time out , return 0;
		// if something error return 0
		// if the ReadFd is not ok , return 0
		if ( (nRet <= 0) ||
			 (0 == FD_ISSET(m_fd, &ReadFdList) ) )
		{
//			printf("UartProtocol::receiveData()  select ret=%d\n", nRet);
			return 0;
		}

		int pos = nBufferLen - nRemain;
		// read data into pRecvBuf
		memset(pBuffer + pos, 0, nRemain);
		// read the header
		nRet = read(m_fd, pBuffer + pos, nRemain);
		if (nRet < 0) {
			printf("Protocol::readFD read head error 0x%X ", nRet);
			return 0;
		}

		nReadLen += nRet;

		nRemain -= nRet;
	} while (nRemain);
	//printf("nRemain=%d",nRemain);

	return nBufferLen - nRemain;

}
// pBuffer (out) the pointer of the buffer, if received data ,save into it
// nLen (in) the length of the buffer
// return (out) the length of the data be saved
int Protocol::receiveData( unsigned char* pBuffer, int nLen )
{
	if( (NULL == pBuffer) ||
		(nLen <= READ_HEADER_SIZE) )
	{
		printf( "Protocol::receiveData Params Invalid nLen=%d", nLen ); 
		return 0;
	}

	int nTotolLen = 0;

	memset(pBuffer, 0, nLen);
	readFD( pBuffer, READ_HEADER_SIZE, nTotolLen );

	if( nTotolLen != READ_HEADER_SIZE )
	{
		if(0 != nTotolLen)
		{
            // clean up  the buffer of file, and return the data read
			int nClr = cleanupFD();
			printf("Protocol::receiveData read header error len=%d, cleared =%d", nTotolLen, nClr );
			
		}
		return nTotolLen;
	}

	int nRet = Check( pBuffer, READ_HEADER_SIZE);
	if( nRet == READ_DATA_CHECK_HEADER_NG )
	{
		// clean up  the buffer of file, and return the data read
		nRet = cleanupFD();
		printf("Protocol::receiveData check header error len=%d. cleared =%d",\
			 nTotolLen, nRet );
		return nTotolLen;

	}
	// get the data total length
	int nDataLen = 0;
	memcpy(&nDataLen,
		   pBuffer + READ_HEADER_LENGTH_INDEX,
		   READ_HEADER_LENGTH_SIZE);
  
	if( nDataLen > nLen )
	{
		nRet = cleanupFD();
            printf("Protocol::receiveData Buffer not enougth len=%d, cleared = %d",\
				 nDataLen, nRet);
		return READ_HEADER_SIZE;
	}
	// read the body
	if (nDataLen > READ_HEADER_SIZE)
	{
		nRet = 0;
		readFD(pBuffer + READ_HEADER_SIZE, nDataLen - READ_HEADER_SIZE, nRet);
		nTotolLen += nRet;

	}

	if (nTotolLen != nDataLen)
	{
            nRet = cleanupFD(); 
            printf("Protocol::receiveData read data error nTotolLen = %d, nDataLen = %d, cleared =%d",\
                nTotolLen, nDataLen, nRet);
            return nTotolLen;
	}

	printf(" Protocol::receiveData() read successfully Len =%d \n", nTotolLen);

	return nTotolLen;
}

// reply the message to notify it with error code
// ErrorCode	(in) the code to reply the message
// pData		(in) the pointer of the data which have some mistake, and have to reply
// nLen			(in) the length of the data
int Protocol::replyError(short ErrorCode, unsigned char* pData, int nLen)
{
	short cmd_code = 0;

	// reply with the command code if the pData contains
	if ( (NULL != pData) 
		 &&( nLen >= (READ_HEADER_CODE_INDEX  + READ_HEADER_CODE_SIZE)) )
	{
		cmd_code = *( (short*)(pData + READ_HEADER_CODE_INDEX) );
	}

	unsigned char replyData[READ_HEADER_SIZE + 1] = \
		{0xFB, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	//Head |Length                            |Retain                             |Type|Command |Replay
	//replyData[READ_HEADER_TYPE_INDEX] = pData[READ_HEADER_TYPE_INDEX] | REPLY_HEADER_TYPE_MASK;

	memcpy(replyData + READ_HEADER_CODE_INDEX,
		   &cmd_code,
		   READ_HEADER_CODE_SIZE);
	memcpy(replyData + READ_HEADER_REPLY_CODE_INDEX,
		   &ErrorCode,
		   READ_HEADER_REPLY_CODE_SIZE);
	setCheckSum(replyData, READ_HEADER_SIZE + 1);

	saveData(replyData, READ_HEADER_SIZE + 1);
	printf(" Protocol::replyError CMD=0x%X, Error=0x%X\n",cmd_code, ErrorCode);

	return RET_OK;
}


// pData (in) the pointer of the data  to send ;
// nDataLen (in) the length of the data
int Protocol::saveData(unsigned char* pData, int nDataLen)
{
#if 0
	if ((NULL == pData) ||
		(nDataLen <= 0))
	{
		printf(" Protocol::saveData params Invalid, nDataLen=%d\n", nDataLen);
		return RET_NG;
	}
	// wait for available space to save the data
	if (0 == sem_trywait(&m_ListAvailable))
	{
		// push the data into the message list
		pthread_mutex_lock(&m_SendMutex);
		m_Msgs.push_back(std::string((char*)pData, nDataLen));
		pthread_mutex_unlock(&m_SendMutex);
		// add the available number to send
		sem_post(&m_MsgAvailable);
		//printf(" Protocol::saveData nDataLen=%d\n", nDataLen);
	}
	else
	{
		printf("send msg queue full");
	}
	return RET_OK;
#else
	m_sendQueue->push(pData, nDataLen);
	return RET_OK;
#endif
}

#if 0
// return (out) the length have send data
int Protocol::sendData()
{
	std::string sData;

	getDataToSend(sData);
	if (sData.size() <= 0){
		// there's no data to send, return
		return 0;
	}

	return sendData( (unsigned char*)sData.c_str(), sData.size());

}
#endif

// pData (in) the pointer of the data to send
// nLen (in) the length of the data
// return (out) the data have send
int Protocol::sendData( unsigned char* pData, int nLen)
{
	//return 0;
#ifdef FUNCTION_TEST 
	return StubProtocol::getInstance()->sendData( pData, nLen );
#endif
	if( (NULL == pData) ||(nLen <= 0) )
	{
		printf( "Protocol::sendData Param Invalid length=%d" , nLen );
		return 0;
	}
	fd_set WriteFd;

	FD_ZERO(&WriteFd);
	
	FD_SET(m_fd, &WriteFd);

	struct timeval waitTime = { 0, 10000 };

	// selecte the data and wait until the fd is available
	int nRet = select(m_fd + 1, NULL, &WriteFd, NULL, &waitTime);
	//if time out, return 0
	//if something errors, return 0
	//if the fd is not available, return 0
	if ( (nRet <= 0) ||
		 (0 == FD_ISSET(m_fd, &WriteFd) ) ){
		//printf("Protocol::sendData()  select ret=%d\n", nRet);
		return 0;
	}

	nRet = write(m_fd, pData, nLen);
#ifdef PRINTF_SEND_RECEIVE

	printf("<----[LX] send DATA Protocol::sendData (%d) :\n", nLen);

	for(int i = 0; i < nLen; i++)
    {
        printf("%02x ",*(unsigned char*)(pData + i));
    }
    printf("\n");
#endif

//	if( 0x0901 != *((short*)(pData + 10 )))
	{
		/*printf("Protocol::sendData()  length=%d CMD=0x%X Ret=0x%X \n",
			 nRet, *( (short*)(pData + 10 ) ), *( (short*)(pData + 12) ) );
			 */
	}
//	else
	{
//		printf("Protocol::sendData =0x%X 0x%X 0x%X 0x%X 0x%X 0x%X",
//				 pData[0], pData[1], pData[2], pData[3],
//				 pData[4], pData[5] );

	}
	
	return nRet;

}

// set the check sum into the data ,and save at the end of it
// pData	(in/out) the source data to get check sum and the destination to set checksum
// nLen		(in) the length of the data
int Protocol::setCheckSum(unsigned char* pData, int nLen)
{
	unsigned char sum = 0x00;
	while (nLen > 0) {
		sum += *pData++;

		nLen--;
	}
	pData[nLen - 1] = (unsigned char)((~sum) + 1);
	return 0;
}

// fd (in) the fd of the file
int Protocol::setFD(int fd)
{
	m_fd = fd;

	return RET_OK;
}

int Protocol::setSendQueue(SendMessageQueue * queue)
{
	if(queue == NULL)
	{
		printf("send queue can not be null");
		return RET_NG;
	}

	m_sendQueue = queue;
	return RET_OK;
}

Protocol::~Protocol()
{
#if 0
	pthread_mutex_destroy(&m_SendMutex);
	sem_destroy(&m_MsgAvailable);
	sem_destroy(&m_ListAvailable);
#endif
	//delete m_pReply;
}

