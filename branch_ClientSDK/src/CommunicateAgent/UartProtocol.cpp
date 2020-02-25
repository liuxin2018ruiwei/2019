#include<sys/time.h>
#include<unistd.h>
#include <stdio.h>
//#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
//#include <sys/select.h>
#include <errno.h>
#include "UartProtocol.h"
//#include "log.h"
//#include "FaceDoorError.h"

#include "Version.h"
#include "LittleEndianIO.h"

#include "log.h"

#ifdef FUNCTION_TEST
#include "StubUartProtocol.h"
#endif

UartProtocol* UartProtocol::m_Inst = NULL;
UartProtocol::UartProtocol(): m_pBuffer(NULL)
{
}

int UartProtocol::addDataToIdleBuff( PkgItor &it, unsigned char *pBuffer,
									int nDataLen )
{
	//printf("addDataToIdleBuff nDataLen = %d,it = %x \n",nDataLen,it);
	if(nDataLen > MAX_RECEIVE_UART_DATA_LEN)
	{
		LX_ERR("ERR : nDataLen > MAX_RECEIVE_UART_DATA_LEN\n");
		return -1;
	}
	
	if( nDataLen < SizePkgLen )
	{
		it->Offset = nDataLen;
		return 0;
	}
	
	int ret32 = 0; 
	unsigned char *pRecordBuffer =  it->Buffer;//save buffer
	for(int i = 0; i < nDataLen - 1; i++)
	{
		if((0xFB == pBuffer[i])&&(0x71 == pBuffer[i+1]))//0xFB, 0x71
		{
			nDataLen = nDataLen - i;
			memcpy( pRecordBuffer, pBuffer + i, nDataLen );	//pBuffer = pBuffer + i;
			pBuffer = pRecordBuffer;
			break;
		}
		
		if(i == nDataLen -2)
		{
			LX_ERR("ERR: can not find 0xFB 0x71\n");
			return -1;
		}
	}
	
	memcpy( &(it->ReqLen), pBuffer + READ_HEADER_LENGTH_INDEX,
		   READ_HEADER_LENGTH_SIZE);
	it->ReqLen += READ_HEADER_SIZE; // because ReqLen = len + headsize
	if( it->ReqLen <= SizePkgLen )
	{
		return -1;
	}

	if( it->ReqLen > MaxPkgBufferLen )
	{
		printf("idle buff bad len=%d", it->ReqLen );
		return -1;
	}
	
	if( it->ReqLen < nDataLen )
	{
		it->Offset = it->ReqLen;
		pBuffer += it->ReqLen;
		nDataLen -= it->ReqLen;
		
		PkgItor freeIt;
		getAvaildPkgBuff( freeIt, it );
		if( freeIt != m_pkgList.end() )
		{
			memcpy( freeIt->Buffer, pBuffer, nDataLen );
			freeIt->TimeOut = it->TimeOut;
			addDataToIdleBuff( freeIt, pBuffer, nDataLen );
		}
		else	
		{
			printf("add to idle no buffer\n");
		}
	}
	else
	{
		it->Offset = nDataLen;
	}

	
	return 0;
	
}

int UartProtocol::addDataToWaitSizeBuff( PkgItor &it, unsigned char *pBuffer,
										int nDataLen)
{
	//printf("addDataToWaitSizeBuff nDataLen = %d,it = %x \n",nDataLen,it);
	if( (it->Offset + nDataLen) < SizePkgLen )
	{
		memcpy( it->Buffer + it->Offset, pBuffer, nDataLen );
		it->Offset += nDataLen;
		return 0;
	}

	//printf("it->ReqLen = %d\n",it->ReqLen);
	if( 0 == it->ReqLen )
	{
		memcpy( it->Buffer + it->Offset, pBuffer, SizePkgLen - it->Offset );
		it->Offset = SizePkgLen;
		pBuffer += (SizePkgLen - it->Offset);
		nDataLen -= (SizePkgLen - it->Offset);

		memcpy( &(it->ReqLen), it->Buffer + READ_HEADER_LENGTH_INDEX,
				READ_HEADER_LENGTH_SIZE);

		it->ReqLen += READ_HEADER_SIZE;
	}

	if( it->ReqLen > MaxPkgBufferLen )
	{
		printf("waitsize buff bad len=%d", it->ReqLen );
		return -1;
	}

	//printf(" it->ReqLen = %d ,nDataLen = %d, it->Offset = %d\n", it->ReqLen, nDataLen,  it->Offset);
	if( it->ReqLen < ( nDataLen + it->Offset ) )
	{
		memcpy( it->Buffer + it->Offset, pBuffer, it->ReqLen - it->Offset );
		it->Offset = it->ReqLen;
		pBuffer += (it->ReqLen - it->Offset);
		nDataLen -= (it->ReqLen - it->Offset);
		
		
		PkgItor freeIt;
		getAvaildPkgBuff( freeIt, it );
		if( freeIt != m_pkgList.end() )
		{
			memcpy( freeIt->Buffer, pBuffer, nDataLen );\
			freeIt->TimeOut = it->TimeOut;
			addDataToIdleBuff( freeIt, pBuffer, nDataLen );
		}
	else	
	{
		printf("add to idle no buffer\n");
	}
	}
	else
	{
		memcpy( it->Buffer + it->Offset, pBuffer, nDataLen );
		it->Offset += nDataLen;
	}


	return 0;
}

int UartProtocol::addDataToAppendBuff( PkgItor &it, unsigned char *pBuffer,
									  int nDataLen )
{

	//printf("addDataToAppendBuff nDataLen = %d,it = %x \n",nDataLen,it);
	if( it->ReqLen > MaxPkgBufferLen )
	{
		printf("appdend buff bad len=%d", it->ReqLen );
		return -1;
	}
	// printf("UartProtocol::addDataToAppendBuff req=%d, off=%d, len=%d\n",
	// 	   it->ReqLen, it->Offset, nDataLen );
	if( it->ReqLen < ( nDataLen + it->Offset ) )
	{
		int cpLen = it->ReqLen - it->Offset;
		memcpy( it->Buffer + it->Offset, pBuffer, it->ReqLen - it->Offset );
		it->Offset = it->ReqLen;
		pBuffer += cpLen;//(it->ReqLen - it->Offset);
		nDataLen -= cpLen;//(it->ReqLen - it->Offset);
		
		
		PkgItor freeIt;
		getAvaildPkgBuff( freeIt, it );
		if( freeIt != m_pkgList.end() )
		{
			memcpy( freeIt->Buffer, pBuffer, nDataLen );
			freeIt->TimeOut = it->TimeOut;
			addDataToIdleBuff( freeIt, pBuffer, nDataLen );
		}
		
	else	
	{
		printf("add to idle no buffer\n");
	}
	}
	else
	{
		memcpy( it->Buffer + it->Offset, pBuffer, nDataLen );
		it->Offset += nDataLen;
	}

	return 0;
	
}

int UartProtocol::addDataToTimeOutBuff( PkgItor &it, unsigned char *pBuffer,
									   int nDataLen )
{

	//printf("addDataToTimeOutBuff nDataLen = %d,it = %x \n",nDataLen,it);
	if( nDataLen < SizePkgLen )
	{
		it->Offset = nDataLen;
		return 0;
	}
	
	memcpy( &(it->ReqLen), pBuffer + READ_HEADER_LENGTH_INDEX,
		   READ_HEADER_LENGTH_SIZE);

	if( it->ReqLen < nDataLen )
	{
		it->Offset = it->ReqLen;
		pBuffer += it->ReqLen;
		nDataLen -= it->ReqLen;
		
		PkgItor freeIt;
		getAvaildPkgBuff( freeIt, it );
		if( freeIt !=  m_pkgList.end() )
		{
			memcpy( freeIt->Buffer, pBuffer, nDataLen );
			freeIt->TimeOut = it->TimeOut;
			addDataToIdleBuff( freeIt, pBuffer, nDataLen );
		}
		
	else	
	{
		printf("add to idle no buffer\n");
	}
	}
	else
	{
		it->Offset = nDataLen;
	}

	
	return 0;
	
}


void UartProtocol::close_port(int fd)
{
	if (fd < 0) {
		return;
	}
	close(fd);
}



speed_t UartProtocol::convert_baudrate(int baudrate)
{
	switch (baudrate) {
//	case 0: return B0;
//	case 50: return B50;
//	case 75: return B75;
//	case 110: return B110;
//	case 134: return B134;
//	case 150: return B150;
//	case 200: return B200;
//	case 300: return B300;
//	case 600: return B600;
//	case 1200: return B1200;
//	case 1800: return B1800;
//	case 2400: return B2400;
//	case 4800: return B4800;
	case 9600: return B9600;
//	case 19200: return B19200;
//	case 38400: return B38400;
//	case 57600: return B57600;
	case 115200: return B115200;
	case 230400: return B230400;
//	case 380400: return B380400;
	case 460800: return B460800;
//	case 500000: return B500000;
	case 576000: return B576000;
	case 921600: return B921600;
	case 1000000: return B1000000;
//	case 1152000: return B1152000;
	case 1500000: return B1500000;
//	case 2000000: return B2000000;
//	case 2500000: return B2500000;
//	case 3000000: return B3000000;

//#if !defined(__CYGWIN__)
//	case 3500000: return B3500000;
//	case 4000000: return B4000000;
//#endif
	default:  return -1;
	}
}

unsigned char *UartProtocol::getBuff()
{
	return m_pBuffer;
}

int UartProtocol::getAvaildPkgBuff( PkgItor &it, PkgItor diffItr )
{
	PkgState state = PkgBuffIdle;
	PkgItor itor = m_pkgList.begin();
	it = m_pkgList.end();
	for( ; itor != m_pkgList.end(); itor++ )
	{
		state = getPkgState( itor );
		if( (state != PkgBuffComplete) && ( diffItr != itor ) )
		{
				it = itor;
				break;
		}
	}


	return 0;
}


UartProtocol::PkgState UartProtocol::getPkgState( PkgItor &it )
{

	if( (0 == it->ReqLen) && ( 0 == it->Offset)  )
	{
		//printf("[LX]it->ReqLen = %d, it->Offset = %d\n",it->ReqLen,it->Offset);
		return PkgBuffIdle;
	}

	struct timeval startTime;
	gettimeofday( &startTime, nullptr );
//Bug:	if( (startTime.tv_usec - it->TimeOut) > WaitTimeOut )
	if( (startTime.tv_sec - it->TimeOut) > WaitTimeOut )
	{
		//test
		//printf("UartProtocol::getPkgState time out");
		return PkgBuffTimeOut;
	}
	// Header + Package = 1 + 4 = 5
	if( (0 == it->ReqLen) && ( it->Offset < SizePkgLen ) )
	{
		printf("UartProtocol::getPkgState waitsize l=%d,%X, %X %X %X %X ",
			 it->Offset, it->Buffer[0],
			 it->Buffer[1],
			 it->Buffer[2],
			 it->Buffer[3],
			 it->Buffer[4]);
		return PkgBuffWaitSize;
	}

	if( (it->ReqLen > 0) && ( it->Offset != it->ReqLen ) )
	{
		return PkgBuffAppend;
	}

	if( (it->ReqLen > 0 ) && ( it->Offset == it->ReqLen ) )
	{
		return PkgBuffComplete;
	}

	return UnknownState;
}

// return the pointer of the class, it's forbiden to new class directly,must call getInstence()
UartProtocol* UartProtocol::getInstance()
{
	if (NULL == UartProtocol::m_Inst) {
		UartProtocol::m_Inst = new UartProtocol;
	}

	return UartProtocol::m_Inst;
}

// init the communication obj before start to use it
int UartProtocol::init(void* priData)
{
	
#ifndef SETUP_MODEL_OPEN
    if( NULL != priData )
    {
		m_pAppContxt = (AppContexts*)priData;
    }
#endif
    SerialPortInfo* pPortInfoPtr = (SerialPortInfo*) priData;
    Protocol::init();

    int nRet = initPort(pPortInfoPtr);
	
    if( RET_NG == nRet )
    {
        return RET_NG;
    }
    
    m_pBuffer = new unsigned char[RECEIVE_BUFF_SIZE];
    if (NULL == m_pBuffer)
	{
    	printf("UartProtocol::init new m_pBuffer fail\n");
    	return RET_NG;
    }

	initBuff( 4, MaxPkgBufferLen );
    return 0;
}

int UartProtocol::initBuff( int buffCnt, int buffLen )
{
	if( buffLen <= 0 )
	{
		return -1;
	}
	
	PkgBuff buff;
	buff.BuffLen = buffLen;
	buff.ReqLen = 0;
	buff.Offset = 0;
	buff.TimeOut = 0;
	for( int nCnt = 0; nCnt < buffCnt; nCnt++ )
	{
		buff.Buffer = new unsigned char[buffLen];
		m_pkgList.push_back( buff );
	}

	return 0;
}

int UartProtocol::reInitPort(SerialPortInfo* pPortInfoPtr)
{
	return initPort( pPortInfoPtr );
}


int UartProtocol::initPort(SerialPortInfo* pPortInfoPtr)
{
	if( (NULL == pPortInfoPtr)||(NULL == pPortInfoPtr->DevPathPtr) || (0 == pPortInfoPtr->PathLen))
	{
		printf("ERR! pPortInfoPtr == NULL\n");
		return -1;
	}
	char *devName = (char *)pPortInfoPtr->DevPathPtr;
	UINT16 devNameLen = pPortInfoPtr->PathLen;


	int nUartFd = open_port( devName, devNameLen );
	if (nUartFd < 0)
	{
		printf("UartProtocol::init open_port error 0x%X\n", nUartFd);
		return RET_NG;
	}

	int nBaudrate = pPortInfoPtr->Baudrate;

	int nRet = set_port(nUartFd, nBaudrate, 8, 'N', 1);
	if (nRet != 0)
	{
		printf("UartProtocol::init set_port error 0x%X\n", nRet);
		return RET_NG;
	}
	setFD(nUartFd);

    return RET_OK;
}

int UartProtocol::setBaudrate(int setBaudrateData)
{
	//printf("setBaudrate getFD() = %d,setBaudrateData = %d\n",getFD(),setBaudrateData);
	int nRet = set_port(getFD(), setBaudrateData, 8, 'N', 1);
	if (nRet != 0)
	{
		printf("setBaudrateset_port error 0x%X\n", nRet);
		return RET_NG;
	}
	printf("nRet UartProtocol::setBaudrate nRet = %d\n",nRet);
	return nRet;
}



int UartProtocol::open_port(char *devName, UINT16 devNameLen)
{
	char devpath[MAX_PATH_NUM];
	int fd;

	
	if( (NULL == devName)||(devNameLen > MAX_PATH_NUM) )
	{
		printf("ERR!! devName = %x,devNameLen = %d\n",devName,devNameLen);
		return -1;
	}

	
	memcpy(&devpath, devName, devNameLen);

	fd = open(devpath, O_RDWR | O_NOCTTY);
	if (fd < 0)
	{
		printf("Open '%s' failed with '%s'!", devName, strerror(errno));
		
	}

	//printf("Open ok!! fd = %d \n ", fd);

	return fd;
}

// return (out) the length have received data
// receive data from uart port, and it will notify the object about the data
int UartProtocol::receiveData()
{
#if 1
	PkgItor it = m_pkgList.end();
	getAvaildPkgBuff( it, m_pkgList.end() );

	if( it == m_pkgList.end() )
	{
		return 0;
	}
	
	int nDataLen = receiveData( it, RECEIVE_BUFF_SIZE);

	PkgItor pkgIt = m_pkgList.begin();
	for(; pkgIt != m_pkgList.end(); pkgIt++ )
	{
		//int packetLen = 0;
		PkgState state = getPkgState( pkgIt );
		//printf("UartProtocol::receiveData state=%d\n",state );
		if( state != PkgBuffComplete )
		{
			//printf("illegal packet: header error, off=%d", pkgIt->Offset);
			continue;
		}
		//printf("UartProtocol::receiveData buff=%p,state=%d, off=%d\n",
		// 	   pkgIt->Buffer, state, pkgIt->Offset );
		
		unsigned char *pBuffer = pkgIt->Buffer;

		//printf("pkgIt->Offset = %d\n",pkgIt->Offset);
		int nRet = Check( pBuffer, pkgIt->Offset);
		//printf("nRet = %d\n",nRet);
		if (READ_DATA_CHECK_OK == nRet)
		{
			//printf("notify start!\n");
			ProtocolMsg protoMsg = { pBuffer, UART_Protocol};
			nRet = notifyObserver((unsigned char *)&protoMsg);
			if (nRet != 0)
			{
//				replyError(ERR_PACKET_CMD_NOT_EXIST, pBuffer, pkgIt->Offset);
			}
		}
		else if (READ_DATA_CHECK_HEADER_NG == nRet)
		{
			//reply error 
//			replyError(ERR_PACKET_PROTOCOL_FAIL, pBuffer, pkgIt->Offset);
		}
		else if (READ_DATA_CHECK_SUM_NG == nRet)
		{
//			replyError(ERR_PACKET_CHECK_FAIL, pBuffer, pkgIt->Offset);
		}
		else
		{
			// no operate
		}

		pkgIt->ReqLen = 0;
		pkgIt->Offset = 0;
		

	
#ifdef PRINTF_SEND_RECEIVE
		int len = LittleEndianIO::readInt16((const char*)pBuffer+4) + 8;
		printf("---->[lx] GET DATA receiveData--> len=%d \n", len );
		 for(int i = 0; i<len ; i++ )
		 {
			printf("%02X ", *(unsigned char*)(pBuffer+i) );
		 }
		 printf("\n");
#endif		
		
	}
#else
	int nDataLen = Protocol::receiveData( m_pBuffer, RECEIVE_BUFF_SIZE);

	
	if (nDataLen <= 0) {
		return 0;
	}

	if( nDataLen < READ_HEADER_SIZE ){
		replyError(ERR_PACKET_PROTOCOL_FAIL, m_pBuffer, nDataLen);
		return nDataLen;
	}
	//printf(" UartProtocol::receiveData() read Len =%d \n", nDataLen);
	// check the header and  read the remain data
	int nRet  = Check(m_pBuffer, nDataLen);
	if (READ_DATA_CHECK_OK == nRet)
	{
		ProtocolMsg protoMsg = {m_pBuffer, UART_Protocol};
		nRet = notifyObserver((unsigned char *)&protoMsg);
		if (nRet != 0)
		{
			replyError(ERR_PACKET_CMD_NOT_EXIST, m_pBuffer, nDataLen);
		}
	}
	else if (READ_DATA_CHECK_HEADER_NG == nRet)
	{
		//reply error 
		replyError(ERR_PACKET_PROTOCOL_FAIL, m_pBuffer, nDataLen);
	}
	else if (READ_DATA_CHECK_SUM_NG == nRet)
	{
		replyError(ERR_PACKET_CHECK_FAIL, m_pBuffer, nDataLen);
	}
	else
	{
		// no operate
	}
#endif		
	return nDataLen;

}

int UartProtocol::receiveData( PkgItor &it, int nLen )
{

	PkgState state = getPkgState( it );

	unsigned char *pBuffer = m_pBuffer;
	if( (PkgBuffIdle == state) || ( PkgBuffTimeOut == state ) )
	{
//		printf("UartProtocol::receiveData state=%d\n", state );
		pBuffer = it->Buffer;
	}
#ifdef FUNCTION_TEST
	int nDataLen = StubUartProtocol::getInstance()->selectRead( pBuffer, nLen );
#else
	fd_set ReadFdList;

	int fd = getFD();
	FD_ZERO(&ReadFdList);
	FD_SET( fd , &ReadFdList);
	struct timeval waitTime = { 0, SELECT_WAIT_TIME };

	// selecte the data and wait until the fd is available
	int nRet = select( fd + 1, &ReadFdList, NULL, NULL, &waitTime);

	// if time out , return 0;
	// if something error return 0
	// if the ReadFd is not ok , return 0
	if ( (nRet <= 0) || (0 == FD_ISSET( fd, &ReadFdList) ) )
	{
		////printf("UartProtocol::receiveData()  select ret=%d\n", nRet);
		return 0;
	}
	
	// read the header
	int nDataLen = read( fd, pBuffer, nLen );
	 
	if( nDataLen <= 0 )
	{
		return 0;
	}
#endif			
	// printf("UartProtocol::receiveData len=%d, head=%02X, code=0x%02X%02X end=%02X \n",
	//  	 nDataLen, pBuffer[0], (nDataLen > 11) ? pBuffer[11] : 0xFF,
	//  	 (nDataLen > 10 ) ? pBuffer[10]: 0xFF,
	//  	 pBuffer[nDataLen - 1]);
	struct timeval curTime;
	gettimeofday( &curTime, nullptr );
	it->TimeOut = curTime.tv_sec;

	switch( state )
	{
		case PkgBuffIdle:
			addDataToIdleBuff( it, pBuffer, nDataLen );
			break;
		case PkgBuffWaitSize:
			addDataToWaitSizeBuff( it, pBuffer, nDataLen );
			break;
		case PkgBuffAppend:
			addDataToAppendBuff( it, pBuffer, nDataLen );
			break;
		case PkgBuffTimeOut:
			addDataToTimeOutBuff( it, pBuffer, nDataLen );
			break;
		default:
			break;
	}
	
	return nDataLen;
}


// before closing the work, terminate must be call to free some resource
int UartProtocol::terminate()
{

	close_port(getFD());

	delete[]m_pBuffer;
	m_pBuffer = NULL;

	uninitBuff();
	return RET_OK;
}

int UartProtocol::set_baudrate(struct termios *tio, int ibaudrate)
{
	speed_t baudrate;

	baudrate = convert_baudrate(ibaudrate);
	if (baudrate < 0) {
		return -1;
	}

//	printf("UartProtocol::set_baudrate b = %X", baudrate);

	cfsetispeed(tio, baudrate);
	cfsetospeed(tio, baudrate);
	return 0;
}

int UartProtocol::set_datawidth(struct termios *tio, int idatawidth)
{
	switch (idatawidth) {
	case 7:
		tio->c_cflag |= CS7;
		break;
	case 8:
		tio->c_cflag |= CS8;
		break;
	default:
		return -1;
	}

	return 0;
}

int UartProtocol::set_parity(struct termios *tio, char cparity)
{
	switch (cparity) {
	case 'N':
		tio->c_cflag &= (~PARENB);
		break;
	case 'O':
		tio->c_cflag |= PARENB;
		tio->c_cflag |= PARODD;
		tio->c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E':
		tio->c_cflag |= PARENB;
		tio->c_cflag &= (~PARODD);
		tio->c_iflag |= (INPCK | ISTRIP);
		break;
	default:
		return -1;
	}

	return 0;
}

int UartProtocol::set_port(int fd, int ibaudrate, int idatawidth, char cparity, int istopbit)
{
	struct termios oldtio, newtio;
	int ret;
	// speed_t baudrate;


	ret = tcgetattr(fd, &oldtio);
	if (ret < 0) {
		return -1;
	}

	//bzero(&newtio, sizeof(newtio));
	memset(&newtio, 0, sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD;

	if (set_baudrate(&newtio, ibaudrate) < 0) {
		printf("ERR set_baudrate\n\n");
		return -1;
	}

	if (set_datawidth(&newtio, idatawidth) < 0) {
		return -1;
	}

	if (set_parity(&newtio, cparity) < 0) {
		return -1;
	}

	if (set_stopbit(&newtio, istopbit) < 0) {
		return -1;
	}

	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd, TCIFLUSH);

	ret = tcsetattr(fd, TCSANOW, &newtio);//reset_terminal_mode
	if (ret) {
		return -1;
	}

	return 0;
}

int UartProtocol::set_stopbit(struct termios *tio, int istopbit)
{
	switch (istopbit) {
	case 1:
		tio->c_cflag &= (~CSTOPB);
		break;
	case 2:
		tio->c_cflag |= CSTOPB;
		break;
	default:
		return -1;
	}

	return 0;
}

int UartProtocol::uninitBuff()
{
	PkgItor it = m_pkgList.begin();
	while( it != m_pkgList.end() )
	{
		delete [](it->Buffer);
		m_pkgList.erase( it++ );
	}

	return 0;
	
}
	

UartProtocol::~UartProtocol()
{
	UartProtocol::m_Inst = NULL;
}
