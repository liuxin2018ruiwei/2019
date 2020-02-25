#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "UDPProtocol.h"
//#include "FaceDoorError.h"
//#include "log.h"


UDPProtocol* UDPProtocol::m_Inst = NULL;

UDPProtocol::UDPProtocol():m_pBuffer(NULL)
{
}



// clean up the buffer hold by m_fd
// return  (out) the result of the last read
int UDPProtocol::cleanupFD()
{
	int nRet = 0;
	unsigned char buffer[20]={'\0'};
	
	read_socket(buffer, sizeof(buffer), nRet, MSG_DONTWAIT);
	
	return nRet;
}

void UDPProtocol::close_port(int fd)
{
	if (fd < 0) {
		return;
	}
	close(fd);
}

// return (out) the fd have created; 
//when failed , return <= 0
// when success, return > 0
int UDPProtocol::createSocketPort()
{
	// create the socket 
	int nSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (nSocketFd < 0) {
		printf("UDPProtocol::createSocketPort socket error");
		return RET_NG;
	}

	struct sockaddr_in SAddr;
	memset(&SAddr, 0, sizeof(SAddr));
	SAddr.sin_family = AF_INET;
	SAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	SAddr.sin_port = htons(UDP_SERVER_PORT);

	int nRet = bind(nSocketFd, (struct sockaddr *)&SAddr, sizeof(SAddr));
	if (nRet != 0)
	{
		printf("UDPProtocol::createSocketPort bind error 0x%X", nRet);
		return RET_NG;
	}

	setFD(nSocketFd);

	// set the socket as unblock mode
	fcntl(nSocketFd, F_SETFL, O_NONBLOCK);
	memset(&m_CAddr, 0, sizeof(m_CAddr));
	m_CAddr.sin_family = AF_INET;
	m_CAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_CAddr.sin_port = htons(UDP_CLIENT_PORT); 

	
	return nSocketFd;

}

// return the pointer of the class
UDPProtocol* UDPProtocol::getInstance()
{
	if (NULL == m_Inst)
	{
		m_Inst = new UDPProtocol;
	}

	return m_Inst;
}

// init the protocol obj before start to use it
int UDPProtocol::init()
{
	// init the protocol first
	 Protocol::init();

	 int nRet = createSocketPort();
	 if (nRet <= 0)
	 {
		 printf("UDPProtocol::init createSocketPort error 0x%X", nRet);
	 }
	m_pBuffer = new unsigned char[UDP_READ_BUFFER_SIZE];
	if( NULL == m_pBuffer )
	{
		printf("UDPProtocol::init new buffer erro");
		return RET_NG;
	}

	return RET_OK;

}

	// pBuffer (out) the buffer to save the data read
	// nBuffLen (in) the length want to read
	// nReadLen (out) the length actually read
	// return (out)RET_NG time out or error happened
	//				RET_OK read ok
int UDPProtocol::readFD(unsigned char* pBuffer,	int nBufferLen, int& nReadLen)
{
	return read_socket(pBuffer, nBufferLen, nReadLen, MSG_DONTWAIT);
}
// pBuffer (out) the buffer to save the data read
// nBuffLen (in) the length of data want to read
// nReadLen (out) the length of data actually read
// return (out)RET_NG time out or error happened
//				RET_OK read ok
int UDPProtocol::read_socket(unsigned char* pBuffer, \
						int nBufferLen, \
						int& nReadLen,\
						unsigned int flag)
{
	int nRemain = nBufferLen;
	int nFD = getFD();
	socklen_t caddr_len = sizeof(m_CAddr);

	nReadLen = 0;
	do
	{
		fd_set ReadFdList;
              
		FD_ZERO(&ReadFdList);
              
		FD_SET(nFD , &ReadFdList);
		struct timeval waitTime = { 0, SELECT_WAIT_TIME };
              
		// selecte the data and wait until the fd is available
		int nRet = select(nFD + 1, &ReadFdList, NULL, NULL, &waitTime);
              
		// if time out , return 0;
		// if something error return 0
		// if the ReadFd is not ok , return 0
		if ( (nRet <= 0) ||
			 (0 == FD_ISSET(nFD, &ReadFdList) ) )
		{
			return 0;
		}

		int pos = nBufferLen - nRemain;
		// read data into pRecvBuf
		memset(pBuffer + pos, 0, nRemain);
		
//		struct sockaddr_in tmpaddr;

		// read the header
		nRet = recvfrom(nFD, \
						pBuffer + pos,\
						nRemain,\
						flag,\
						NULL,\
						0);
		//						(sockaddr*)&tmpaddr,
		//				&caddr_len);
		if (nRet < 0)
		{
			printf("UDPProtocol::readFD read head error 0x%X ", nRet);
			return 0;
		}

		nReadLen += nRet;

		nRemain -= nRet;
	} while (nRemain > 0);
	//printf("nRemain=%d",nRemain);

	return nBufferLen - nRemain;

}

// return (out) the length have received data
// receive data from uart port, and it will notify the object about the data
int UDPProtocol::receiveData()
{
	int nDataLen = receiveData(m_pBuffer, UDP_READ_BUFFER_SIZE);
	if (nDataLen <= 0) {
		return 0;
	}
	// check the header and  read the remain data
	int nRet = Check(m_pBuffer, nDataLen);
	if (READ_DATA_CHECK_OK == nRet)
	{
		ProtocolMsg protoMsg = {m_pBuffer, UDP_Protocol};
		nRet = notifyObserver((unsigned char *)&protoMsg);
		if (nRet != 0)
		{
//			replyError(ERR_PACKET_CMD_NOT_EXIST, m_pBuffer, nDataLen);
		}
	}
	else if (READ_DATA_CHECK_HEADER_NG == nRet)
	{
		//reply error 
//		replyError(ERR_PACKET_PROTOCOL_FAIL, m_pBuffer, nDataLen);
	}
	else if (READ_DATA_CHECK_SUM_NG == nRet)
	{
//		replyError(ERR_PACKET_CHECK_FAIL, m_pBuffer, nDataLen);
	}
	else
	{
		// no operate
	}

		
	return nDataLen;

}

// pBuffer (out) the pointer of the buffer, if received data ,save into it
// nLen (in) the length of the buffer
// return (out) the length of the data be saved
int UDPProtocol::receiveData( unsigned char* pBuffer, int nLen )
{
	if( (NULL == pBuffer) ||
		(nLen <= READ_HEADER_SIZE) )
	{
		return 0;
	}

	int nTotolLen = 0;

	memset(pBuffer, 0, nLen);
	read_socket( pBuffer, READ_HEADER_SIZE, nTotolLen, MSG_PEEK |MSG_DONTWAIT );

	if( nTotolLen <READ_HEADER_SIZE )
	{
		if(0 != nTotolLen)
		{
            // clean up  the buffer of file, and return the data read
			int nClr = cleanupFD();
		}
		return nTotolLen;
	}
	int nRet = Check( pBuffer, READ_HEADER_SIZE);
	if( nRet == READ_DATA_CHECK_HEADER_NG )
	{
		// clean up  the buffer of file, and return the data read
		nRet = cleanupFD();
		return nTotolLen;

	}
	// get the data total length
	int nDataLen = 0;
	memcpy(&nDataLen,
		   pBuffer+ READ_HEADER_LENGTH_INDEX,
		   READ_HEADER_LENGTH_SIZE);
  
	if( nDataLen > nLen )
	{
		nRet = cleanupFD();
		return READ_HEADER_SIZE;
	}
	// read the body
	if (nDataLen > READ_HEADER_SIZE)
	{
		nRet = 0;
		read_socket(pBuffer, nDataLen, nRet, MSG_DONTWAIT);
		nTotolLen = nRet;

	}
	return nTotolLen;
}

// pData (in) the pointer of the data to send
// nLen (in) the length of the data
// return (out) the data have send
int UDPProtocol::sendData( unsigned char* pData, int nLen)
{
	if( (NULL == pData) ||
		(nLen <= 0) )
	{
		printf( "UDPProtocol::sendData Param Invalid length=%d" , nLen );
		return 0;
	}

	int nFD = getFD();
	socklen_t caddr_len = sizeof(m_CAddr);

	fd_set WriteFd;
	FD_ZERO(&WriteFd);
	
	FD_SET(nFD, &WriteFd);

	struct timeval waitTime = { 0, 100000 };

	// selecte the data and wait until the fd is available
	int nRet = select(nFD + 1, NULL, &WriteFd, NULL, &waitTime);
	//if time out, return 0
	//if something errors, return 0
	//if the fd is not available, return 0
	if ( (nRet <= 0) ||
		 (0 == FD_ISSET(nFD, &WriteFd) ) )
	{
		printf("UDPProtocol::sendData()  select ret=%d\n", nRet);
		return 0;
	}

	nRet = sendto(nFD,\
				  pData,\
				  nLen,	\
				  MSG_DONTWAIT,\
				  (sockaddr*)&m_CAddr,\
				  caddr_len);
	//printf("UDPProtocol::sendData()  length=%d CMD=0x%X\n", nRet, *((short*)(pData + 10 )) );

	return nRet;

}

// return (out) the length have send data
//virtual int sendData();
// before closing the work, terminate must be call to free some resource
int UDPProtocol::terminate()
{
	close_port(getFD());

	delete[]m_pBuffer;
	m_pBuffer = NULL;

	return RET_OK;
}

UDPProtocol::~UDPProtocol()
{
	UDPProtocol::m_Inst = NULL;

}


