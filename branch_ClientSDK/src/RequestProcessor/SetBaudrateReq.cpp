/********************************************************************
* Name      :  SetBaudrateReq.cpp                                  
* Describ   :  the definaition of the class
* Author    :  liuxin                                          
* Date      :  2018-06-25                                                
********************************************************************/

#include <unistd.h>
#include <stdio.h>
#include "CommunicationAgent.h"
#include "LittleEndianIO.h"
#include "setBaudrateRequestion.h"
#include "SetBaudrateReq.h"
#include "FaceDetector.h"

#include <time.h>
#include <sys/time.h>






SetBaudrateReq::SetBaudrateReq():
	BaseReq( Requestion::SetBaudrate )
{

}



int SetBaudrateReq::processReq( Requestion *pReq )
{
	int ret32 = 0;
	setBaudrateRequestion *req = (setBaudrateRequestion *)pReq;
	UINT32 setBaud = req->getBaudRate();
	//printf("\n\nprocessReq---->getBaudRate = %d\n",setBaud);
	char sendData[4] ={0};
	short int retCode = 0;

	LittleEndianIO::writeInt32((char *)sendData,setBaud); 
	
	 //test
	/*UINT8 *p = sendData;
	for(int i = 0 ; i < UART_GET_FACE_VERSION_SEND_LEN; i++)
	{
		printf("%x ", *p);
		p++;
	}
	printf("\n");
	printf("sendMsg SetBaudrateReq ok\n");
	*/

	 
	CommunicationAgent::getInstance()->sendMsg(0,UART_SET_BAUDRATE_COMMAND,
								 0, (char *)sendData, 4, CommunicationAgent::SendToOuter);

	usleep(100 * 1000);//0.1s
	ret32 = Communication::getInstances()->setBautrate(setBaud);
	if(ret32 != 0)
	{
		printf("setBautrate ret32 = %d\n",ret32);
		return -1;
	}
	//波特率状态变化
	if(0 == FaceDetector::getInstance()->getFlagRecoverBarudrate())
	{
		//printf("set change to 1\n");
		FaceDetector::getInstance()->setflagRecoverBarudrate(SET_FPGA_TO_CHANGE_BARUDRATE);
		struct timeval tv;
		gettimeofday(&tv,NULL);
		long long newTime = (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000);

		FaceDetector::getInstance()->setRecoverBarudTime(newTime);
	}
	
	//printf("setBautrate ok wait \n\n\n");
	return ret32;
}

SetBaudrateReq::~SetBaudrateReq()
{
	
}

