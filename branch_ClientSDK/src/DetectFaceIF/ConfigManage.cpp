/********************************************************************
* Name      :  ConfigManage.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-05                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <FrameManage.h>

#include "DetectFaceIFCommon.h"
#include "ConfigManage.h"
#include "DetectFaceIF.h"
#include "MemoryManager.h"
#include "DataCache.h"
#include "RequestProcessor.h"
#include "Communication.h"
#include "CommunicationAgent.h"
#include "PthreadManage.h"
#include "MsgProcessor.h"
#include "FaceDetector.h"
#include "ProcessReq.h"
#include "Requestion.h"
#include "UserCommonRequestion.h"
#include "CountInterface.h"
#include "FaceDataManage.h"
#include "SetBaudrateReq.h"
#include "setBaudrateRequestion.h"


#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "ZmodeUpgrade.h"
#include "mysem.h"
#include "UartProtocol.h"
#include "UpgradeRequestion.h"
#include "HeartBeatInfo.h"


ConfigManage *ConfigManage::m_this =  nullptr;

ConfigManage * ConfigManage::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new ConfigManage;
	}

	return m_this;
}



int ConfigManage:: sendToUpdata(int uartHand , int type,char *fileName )
{
	pid_t pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork()");
		fprintf(stderr, "Failed to fork new process for file sending!\n");
		return -1;
	}
	int status = 0;
	if (pid2 == 0)//子进程
	{
		//printf("uartHand = %d\n",uartHand);
		dup2(uartHand, 0);
		dup2(uartHand, 1);
		//ok
		char *argv[5];
		argv[0] = (char *)"/home/lsz";
		if(CAMERA_QSPI_IMAGE_TYPE == type)
		{
			argv[1] = (char *)"-vv";
			argv[2] = (char *)"-b";
		}
		else if(APP_PKG_TYPE == type)
		{
			argv[1] = (char *)"-vv";
			argv[2] = (char *)"-b";
		}
		else if(APP_CFG_TYPE == type)
		{
			argv[1] = (char *)"-vv";
			argv[2] = (char *)"-b";
		}
		argv[3] = fileName;//(char *)"/home/Camera-QSPI-image.bin";
		argv[4] = NULL;

		execvp(argv[0], argv);
		perror("execvp()");

		status = -2;
		
		
		//uint8_t dummy = 0;
     	//write(uartHand, &dummy, 1);
		exit(0);
	 }

	//printf("forkNew --> litter father wait....\n");
	waitpid(pid2, &status, 0);
	//printf("forkNew ----->status = %d\n",status);

	 uint8_t dummy = 0;
     write(uartHand, &dummy, 1);
	
	return 0;

}

int  ConfigManage::forkNew()
{
   //信号量1
   m_semid = create_sems(10); // 创建一个包含10个信号量的信号集
   init_sems(m_semid, 0, 0);  // 初始化编号为 0 的信号量值为0


    //int fd[2];//fd[0]指向管道的读端,fd[1]指向管道的写端。fd[1]的输出是fd[0]的输入
	int ret = pipe(fd);
	if (ret == -1)
	{
	  perror("pipe error\n");
	  return 1;
	}
	
   pid_t id = fork(); // 创建子进程
   if( id < 0)
   {
	   perror("fork");
	   return -1;
   }
   else if (0 == id)
   {// child 
	   int sem_id = get_sems(); // 信号量1

		close(fd[1]);  
	    char msg[100]; 
		while (1)
	    {
			P(sem_id, 0); // 对该信号量集中的0号信号  做P操作,进来就堵塞该进程
			memset(msg,'\0',sizeof(msg));  
			ssize_t s = read(fd[0], msg, sizeof(msg));  
		    if (s>0)  
		    {  
			   msg[s - 1] = '\0';  
		    } 
		//	printf("get  s = %d, msg = %s\n",s,msg);
			int uartHand;
			char getUartHand = msg[0];
			uartHand = getUartHand - '0';
			int type = msg[1] - '0';
			//printf("forkNew  --->uartHand = %d, type = %d\n",uartHand, type);
			int len = (msg[2] - '0')*10 + msg[3] - '0';
			//printf("len = %d\n",len);

			char *fileName = (char *)&msg[4];
		//	printf("fileName = %s\n",fileName);

			sendToUpdata(uartHand, type, fileName);

			V(sem_id, 0); //开主进程
		}
   }

   //father
}

int ConfigManage::updateApp(char * data1, int type)
{
   //关心跳
	HeartBeatInfo::getInstance()->setUpState(1);//UPGREAD_START = 1;

   if (true == Communication::getInstances()->isRunning())
   {
	   Communication::getInstances()->stop();
	   usleep(1 * 100 *1000);//关完以后，确定stop可以
   }


	//填充数据发送
	int sem_id = get_sems();
	close(fd[0]);  
	char data[100];
    char *sendToUp = data;
	int uart_handle = UartProtocol::getInstance()->getFD();
	//printf("updateApp uart_handle = %d\n",uart_handle);


	
	//数据结构是 = 1个字节的串口号 + 1个字节的类型 + 2个字节的文件长度 + 升级文件地址 + 长度加结束符 
	sprintf(data, "%01d%01d%02d%s", uart_handle, type, strlen(data1), data1); 
	
	//printf("sendToUp = %s\n", sendToUp);
 	write(fd[1], sendToUp, strlen(sendToUp) + 1); 
	

	V(sem_id, 0); //信号量加一

	
	//printf("@dusha..\n");
	P(sem_id, 0);// 堵塞主进程
	//printf("\n\n@get ok\n");

	//重启串口
	reOpenUart();
	Communication::getInstances()->start();
	//printf("Communication: start ok \n");
	
	return 0;
}

int ConfigManage::reOpenUart()
{
	int uart_handle = UartProtocol::getInstance()->getFD();
	 
	//printf("\nreOpenUart uart_handle = %d\n",uart_handle);
	 if (uart_handle < 0)
    {
       //printf("reOpenUart start!!!!\n");
       SerialPortInfo reSetDate;
	   reSetDate.Baudrate = ConfigManage::getInstance()->getBaudRate();
	   //printf("reSetDate.Baudrate = %d\n",reSetDate.Baudrate);
	   reSetDate.DevPathPtr = (UINT8 *)ConfigManage::getInstance()->getUartDevPtr();
	   reSetDate.PathLen = ConfigManage::getInstance()->getUartDevLen();
	   //printf("reSetDate.DevPathPtr = %s,reSetDate.PathLen = %d\n",reSetDate.DevPathPtr,reSetDate.PathLen);
	   int ret =  UartProtocol::getInstance()->reInitPort(&reSetDate);
	   if(ret != 0)
	   {
			printf("reOpenUart fail \n");
			return -1;
	   }
	 //  printf("reOpenUart!!!!\n");
	   usleep(1 * 100 *1000); // 0.1s
    }
	 return 0;
}


ConfigManage::~ConfigManage()
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif	

}


int ConfigManage::initConfigManage( DetectConfigInfo* pConfig )
{	
	if( nullptr == pConfig )
	{
		printf("ERR: %d %s pConfig=%P\n", __LINE__, __func__,
			   pConfig );
		
		return -1;
	}
	
	


	
	int nRet = 0 ;

	//CommunicateAgent 
	//注意初始化的顺序，先要Communication再CommunicationAgent，不然会影响添加观察者
	if(pConfig->DetectType == DETECT_TYPE_LOAD_SDK)//使用该模式串口要有数据
	{
		if(pConfig->SerialPortInfoPtr == NULL) 
		{
			printf("ERR DetectType = 1, pConfig->SerialPortInfoPtr = NULL\n");
			nRet = -1;
			return nRet;
		}
		if(pConfig->SerialPortInfoPtr->DevPathPtr == NULL)
		{
			printf("ERR DetectType = 1, pConfig->SerialPortInfoPtr->DevPathPtr = NULL\n");
			nRet = -1;
			return nRet;
		}

		if((pConfig->SerialPortInfoPtr->Baudrate == 0)||(pConfig->SerialPortInfoPtr->PathLen == 0) )
		{
			printf("ERR DetectType = 1, pConfig->SerialPortInfoPtr->Baudrate = %d,pConfig->SerialPortInfoPtr->PathLen = %d\n",
									pConfig->SerialPortInfoPtr->Baudrate,pConfig->SerialPortInfoPtr->PathLen);
			nRet = -1;
			return nRet;
		}
		
		SerialPortInfo *pPortInfoPtr = pConfig->SerialPortInfoPtr;
		SerialPortInfo SerialInfoTemp;
		SerialInfoTemp.Baudrate = DEFAULT_BAUDRATE_FPGA;
		SerialInfoTemp.DevPathPtr = pConfig->SerialPortInfoPtr->DevPathPtr;
		SerialInfoTemp.PathLen = pConfig->SerialPortInfoPtr->PathLen;
		printf("SerialInfoTemp.Baudrate = %d; SerialInfoTemp.DevPathPtr = %s,\n",
			SerialInfoTemp.Baudrate, SerialInfoTemp.DevPathPtr,SerialInfoTemp.PathLen);
		
		nRet |=Communication::getInstances()->init( &SerialInfoTemp );

		forkNew();//开启升级进程，因为需要串口初始化，加再这
	}
	
	
	
	m_detectType = pConfig->DetectType;
	m_transferType = pConfig->TransferType;
	m_cameraID = pConfig->CameraID;

	memset( &m_moduleVersion, 0, sizeof( VersionInfo ) );
	
	//DetectFaceIF
	ProcessReq * pProcReq = ProcessReq::getInstance();
	nRet = pProcReq->initProcessReq( pProcReq );
//printf("init ProcessReq \n");



	//MemoryManage
	MemoryManager* pMemManger = MemoryManager::getInstance();
	nRet |= pMemManger->initMemory();
//printf("init MemoryManager \n");



	//DataCache
	DataCache * pDataCache = DataCache::getInstance();
	//printf("pConfig->FrameInfo = %x;width = %d, height = %d \n",pConfig->FrameInfo, pConfig->FrameInfo->Width, pConfig->FrameInfo->Height);
	nRet |= pDataCache->initDataCache( pConfig->FrameInfo);
//printf("init DataCache \n");

	
	
	m_DetectType = pConfig->DetectType ;
	m_baudrate = pConfig->SerialPortInfoPtr->Baudrate;


	
	CommunicationAgent::getInstance();
	//printf("ConfigManage -> setSerialPortInfo\n");
//printf("****init CommunicationAgent  \n");


	
	//PthreadManage
	PthreadManage* pPtheadManage = PthreadManage::getInstance();
	nRet |= pPtheadManage->initPthreadManage( pPtheadManage );
//printf("init PthreadManage \n");
	

	//MsgProcessor
	MsgProcessor* pMsgProcessor = MsgProcessor::getInstance();
	MsgProcessor::InitInfo msgProcInifo;
	nRet |= pMsgProcessor->initMsgProcessor( &msgProcInifo );
//printf("init MsgProcessor \n");

	

	//RequestProcessor
	RequestProcessor* pReqProcessor = RequestProcessor::getInstance();
	RequestProcessor::InitInfo reqInitInfo;
	nRet |= pReqProcessor->initRequestProcessor( &reqInitInfo );
//printf("init RequestProcessor \n");

	
	//FaceDetector
	if( DetectType_ON == m_detectType )
	{
		FaceDetector* pFaceDetector = FaceDetector::getInstance();
		nRet |= pFaceDetector->initFaceDetector( pFaceDetector );// dummy pridata
//printf("init FaceDetector \n");
	}


	
	
	
	if(pConfig->SerialPortInfoPtr->PathLen < 64)
	{
		memcpy( m_DevData, pConfig->SerialPortInfoPtr->DevPathPtr,pConfig->SerialPortInfoPtr->PathLen);
		//printf("save m_DevData = %s \n",m_DevData);
		m_devLen = pConfig->SerialPortInfoPtr->PathLen;
	}
	
	return nRet;
}
UINT32 ConfigManage::getBaudRate()
{
	return m_baudrate;
}

char*  ConfigManage::getUartDevPtr()
{
	return (char*)m_DevData;
}
int ConfigManage::getUartDevLen()
{
	return m_devLen;
}


int ConfigManage::setBaudrate(UINT32 baudrateData)
{
	//printf("set baudrate  = %d,m_DetectType = %d\n", baudrateData,m_DetectType);
	int ret32Test = -1;
	if(DEFAULT_BAUDRATE_FPGA == baudrateData)
	{
		return 0;
	}

	if(m_DetectType == DETECT_TYPE_LOAD_SDK)//使用该模式串口要有数据
	{
		UINT32 tempBaudrate = baudrateData;
		Requestion *pSetbaudrateReq = new setBaudrateRequestion( tempBaudrate,Requestion::SetBaudrate );
		ret32Test =  CommunicationAgent::getInstance()->sendReq( pSetbaudrateReq );
		if(ret32Test != 0)
		{
			printf("sendReq ret32Test = %d\n",ret32Test);
		}
	}
	return ret32Test;
}

int ConfigManage::initGetSoftVersion()
{
	int ret32 = 0;
	//send req to get Version info
	Requestion *rq = new Requestion(Requestion::GetFaceDetecteVersionReqID);
	ret32 = CommunicationAgent::getInstance()->sendReq( rq );
	//printf("init sendReq  0x0101\n");

	UINT64 tempReqID = 0x01;//unuser
	Requestion *pReq = new UserCommonRequestion( tempReqID,Requestion::GetServiceVisionReqID );
	ret32 |= CommunicationAgent::getInstance()->sendReq( pReq );
	//printf("init sendReq  0x0105\n");
	return ret32;
}

int ConfigManage::uninitConfigManage()
{
	int nRet = 0;
	//DetectFaceIF
	ProcessReq::getInstance()->uninitProcessReq();
//printf("uninit DetectFaceIF \n");

	
	//MemoryManage
	MemoryManager::getInstance()->uninitMemory();
//printf("uninit uninitMemory \n");

	//DataCache
	DataCache::getInstance()->uninitDataCache();
//printf("uninit uninitDataCache \n");

	//CommunicateAgent
	CommunicationAgent::getInstance()->uninit();
//printf("****out<<<<	CommunicationAgent  \n");

	//PthreadManage
	PthreadManage::getInstance()->uninitPthreadManage();
//printf("uninitConfigManage	PthreadManage  \n");	

	//MsgProcessor
	MsgProcessor::getInstance()->uninitMsgProcessor();
//printf("uninitConfigManage	MsgProcessor  \n");	

	//RequestProcessor
	RequestProcessor::getInstance()->unintRequestProcessor();
//printf("uninitConfigManage	RequestProcessor  \n");

	//FaceDetector
	FaceDetector::getInstance()->uninitFaceDetector( );// dummy pridata
//printf("uninitConfigManage	FaceDetector  \n");

	
	return nRet;
}


int ConfigManage::getModuleProtocolVersion( char *pVersion, int nLen )
{
	if( ( nullptr == pVersion ) || ( nLen < 0 ) )
	{
		return -1;
	}

	int cpLen = ( nLen < VERSION_SIZE ) ? nLen : VERSION_SIZE;
	memcpy( pVersion, m_moduleVersion.ProtocolVersion, cpLen );

	return 0;
}

int ConfigManage::setModuleProtocolVersion( char * protocolVersion, int nLen )
{
	if( ( nullptr == protocolVersion ) || ( nLen <= 0 ) )
	{
		return -1;
	}

	int cpLen = ( nLen < VERSION_SIZE ) ? nLen : VERSION_SIZE;
	memcpy( m_moduleVersion.ProtocolVersion, protocolVersion, cpLen );

	return 0;
}


int ConfigManage::getModuleSoftwareVersion( char *pVersion, int nLen )
{
	if( ( nullptr == pVersion ) || ( nLen < 0 ) )
	{
		return -1;
	}

	int cpLen = ( nLen < VERSION_SIZE ) ? nLen : VERSION_SIZE;
	memcpy( pVersion, m_moduleVersion.SoftwareVersion, cpLen );

	return 0;
}
int ConfigManage::setModuleSoftwareVersion( char * softwareVersion, int nLen )
{
	if( (nullptr == softwareVersion)|| ( nLen <= 0 ) )
	{
		return -1;
	}
	int cpLen = ( nLen < VERSION_SIZE ) ? nLen : VERSION_SIZE;
	memcpy( m_moduleVersion.SoftwareVersion, softwareVersion, cpLen );
}

int ConfigManage::getModuleFirmwareVersion( char *pVersion, int nLen )
{
	if( ( nullptr == pVersion ) || ( nLen < 0 ) )
	{
		return -1;
	}

	int cpLen = ( nLen < VERSION_SIZE ) ? nLen : VERSION_SIZE;
	memcpy( pVersion, m_moduleVersion.FirmwareVersion, cpLen );

	return 0;	
}

int ConfigManage::setModuleFirmwareVersion( char * firmwareVersion, int nLen )
{
	if( (nullptr == firmwareVersion)|| ( nLen <= 0 ) )
	{
		return -1;
	}
	int cpLen = ( nLen < VERSION_SIZE ) ? nLen : VERSION_SIZE;
	memcpy( m_moduleVersion.FirmwareVersion, firmwareVersion, cpLen );
}

UINT64 ConfigManage::getCameraID()
{
	return m_cameraID;
}

  

ConfigManage::ConfigManage()
{
}
