#include <stdio.h>
#include "StubUartProtocol.h"
#include "CaseCommonInfo.h"
#include "TestDriverFactory.h"

StubUartProtocol* StubUartProtocol::m_Inst = NULL;
StubUartProtocol::StubUartProtocol():m_seq(0)
{
}


int StubUartProtocol::selectRead(unsigned char * pData, int nLen )
{
	//return 0;
	
	int ret = 0;
	switch( CaseCommonInfo::getInstance()->getCaseID() )
	{
		case TestDriverFactory::Case_Sample:
			//break; !!!!NOTICE
	   // TODO: many fill data
		default:
			ret = fillDummyData_default( pData, nLen, m_seq );
			break;
	}

	printf("STUB UartProtocol::selectRead (%d) :\n", ret);
	for(int i = 0; i < ret; i++)
    {
        printf("%02x ",*(unsigned char*)(pData + i));
    }
    printf("\n");
	

	return ret;
}

int StubUartProtocol::fillDummyData_default( unsigned char* pData, int nLen,
											 unsigned int &seq )
{
	if( seq > 2 )
	{
		return 0;
	}
	seq++;
	
	pData[0] = 0xFB;
	pData[1] = 0x71;
	pData[2] = 0x00;
	pData[3] = 0x00;
	pData[4] = 0x0C;
	pData[5] = 0x00;
	pData[6] = 0x00;
	pData[7] = 0x00;
	pData[8] = 0x05;
	pData[9] = 0x01;
	pData[10] = 0x80;
	pData[11] = 0x00;

	return 12;
	
}
// return the pointer of the class, it's forbiden to new class directly,must call getInstence()
StubUartProtocol* StubUartProtocol::getInstance()
{
	if( nullptr == m_Inst )
	{
		m_Inst = new StubUartProtocol;
	}
	
	return StubUartProtocol::m_Inst;
}

#if 0
// init the communication obj before start to use it
int StubUartProtocol::init(void* priData)
{
    return 0;
}

int StubUartProtocol::addDataToIdleBuff( PkgItor &it, unsigned char *pBuffer,
									int nDataLen )
{
	
	return 0;
	
}

int StubUartProtocol::addDataToWaitSizeBuff( PkgItor &it, unsigned char *pBuffer,
										int nDataLen)
{

	return 0;
}

int StubUartProtocol::addDataToAppendBuff( PkgItor &it, unsigned char *pBuffer,
									  int nDataLen )
{

	return 0;
	
}

int StubUartProtocol::addDataToTimeOutBuff( PkgItor &it, unsigned char *pBuffer,
									   int nDataLen )
{
	return 0;
	
}


void StubUartProtocol::close_port(int fd)
{
}



speed_t StubUartProtocol::convert_baudrate(int baudrate)
{
	return 0;
}

unsigned char *StubUartProtocol::getBuff()
{
	
	return 1;
}

int StubUartProtocol::getAvaildPkgBuff( PkgItor &it, PkgItor diffItr )
{

	return 0;
}


StubUartProtocol::PkgState StubUartProtocol::getPkgState( PkgItor &it )
{

	return UnknownState;
}



int StubUartProtocol::initBuff( int buffCnt, int buffLen )
{

	return 0;
}



int StubUartProtocol::open_port(int portno)
{

	return 0;
}

// return (out) the length have received data
// receive data from uart port, and it will notify the object about the data
int StubUartProtocol::receiveData()
{
	return 0;

}

int StubUartProtocol::receiveData( PkgItor &it, int nLen )
{
		
	return 0;
}


// before closing the work, terminate must be call to free some resource
int StubUartProtocol::terminate()
{
	return RET_OK;
}

int StubUartProtocol::set_baudrate(struct termios *tio, int ibaudrate)
{

	return 0;
}

int StubUartProtocol::set_parity(struct termios *tio, char cparity)
{
	return 0;
}

int StubUartProtocol::set_port(int fd, int ibaudrate, int idatawidth, char cparity, int istopbit)
{
	return 0;
}

int StubUartProtocol::set_stopbit(struct termios *tio, int istopbit)
{

	return 0;
}

int StubUartProtocol::uninitBuff()
{
	return 0;
	
}
#endif

int StubUartProtocol::initPort()
{
    return RET_OK;
}

StubUartProtocol::~StubUartProtocol()
{
}
