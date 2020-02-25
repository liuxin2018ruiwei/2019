#ifndef STUBPROTOCOL_H
#define STUBPROTOCOL_H

#include "Protocol.h"

class StubProtocol
{
public:
  static StubProtocol* getInstance();
	StubProtocol();
	// check the data if the data is corrent
	// pData	(in) the data want to check
	// nLen		(in) the length of the data
	// return	(out) 
	//			READ_DATA_CHECK_OK 
	//			READ_DATA_cHECK_HEADER_NG -1
	//			READ_DATA_CHECK_SUM_NG -2
	int Check(unsigned char* pData, int nLen);
	// check the SUM to make sure the data is correct
	// return(out) RET_OK / RET_NG
	//int CheckSum(unsigned char* pData, int nLen);
	// return (out) the fd of the file
	virtual int getFD(); 
	// sData(out) get the data to send which will save in sData
	//virtual int getDataToSend( std::string& sData);
	virtual int init( void * priData );
	// init the protocol obj before start to use it
	virtual int init();
	// return (out) the length have received data 
	//virtual int receiveData()=0;
	// pBuffer (out) the pointer of the buffer, if received data ,save into it
	// nLen (in) the length of the buffer
	// return (out) the length of the data be saved
	virtual int receiveData( unsigned char* pBuffer, int nLen );
	// clean up the buffer hold by m_fd
	// return  (out) the result of the last read
	virtual int cleanupFD();
	// return (out) the length have send data
	//virtual int sendData();
	// pData (in) the pointer of the data to send
	// nLen (in) the length of the data
	// return (out) the data have send
	virtual int sendData( unsigned char* pData, int nLen);
	// fd (in) the fd of the file
	virtual int setFD(int fd);
	// pData (in) the pointer of the data  to send ;
	// nDataLen (in) the length of the data
	virtual int saveData(unsigned char* pData, int nDataLen);

	virtual int setSendQueue(SendMessageQueue * queue);
	// before closing the work, terminate must be call to free some resource
	//virtual int terminate()=0;

	virtual ~StubProtocol();

protected:
	// pBuffer (out) the buffer to save the data read
	// nBuffLen (in) the length want to read
	// nReadLen (out) the length actually read
	// return (out)RET_NG time out or error happened
	//				RET_OK read ok
	virtual int readFD(unsigned char* pBuffer, int nBufferLen, int& nReadLen);
	// set the check sum into the data ,and save at the end of it
	// pData	(in/out) the source data to get check sum and the destination to set checksum
	// nLen		(in) the length of the data
	int setCheckSum(unsigned char* pData, int nLen);
	// reply the message to notify it with error code
	// ErrorCode	(in) the code to reply the message
	// pData		(in) the pointer of the data which have some mistake, and have to reply
	// nLen			(in) the length of the data
	int replyError(short ErrorCode, unsigned char* pData, int nLen);
		
private:
	static StubProtocol* m_this;
};

#endif
