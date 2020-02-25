/********************************************************************
 * Name      :  DetectFaceIF.cpp                                  
 * Describ   :  the defination of the IF. the interface bases on C language       
 * Author    :  Zhang Botong                                          
 * Date      :  2018-06-04                                                 
 ********************************************************************/
#include <stdio.h>
#include "Version.h"
#include "DetectFaceIF.h"
#include "ConfigManage.h"
#include "PthreadManage.h"
#include "ProcessReq.h"



#ifdef __cplusplus
extern "C"
{
#endif


//用户句柄保存，每次调用都要对该句柄进行判定，初始化获取由initDetectFaceIF获取
UINT32 *m_saveTHFhand = 0;



/*
  void *TFHandle  输出,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用
  DetectConfigInfo * pConfig 输入，指向配置变量的指针。
  int						   reture <0 表示出错。0,表示成功。
该接口用于用户对服务端进行初始化。初始化完成后即可进行业务通信。
*/
	int initDetectFaceIF( void **TFHandle, DetectConfigInfo * pConfig)
	{
#ifdef DEBUG_LOG
		printf("%d : %s\n", __LINE__, __func__);
#endif
		if( ( NULL == TFHandle ) || ( NULL == pConfig ) )
		{
			printf("ERR %d %s TFHandle=%p, pConfig=%x\n", __LINE__, __func__,
				   TFHandle, pConfig );
			return -1;
		}

		if((NULL == pConfig->SrcUserInfo) || (NULL == pConfig->SrcIFInfo)|| (NULL == pConfig->FrameInfo))
		{
			printf("ERR pConfig->SrcUserInfo = %x,pConfig->SrcIFInfo = %x,pConfig->FrameInfo = %x \n",
								pConfig->SrcUserInfo,pConfig->SrcIFInfo,pConfig->FrameInfo);
			return -1;
		}

		UINT8 getDetectType = pConfig->DetectType;
		#define MIN_DETECT_TYPE (0)
		#define MAX_DETECT_TYPE (1)
		if( (MIN_DETECT_TYPE > getDetectType) || (getDetectType > MAX_DETECT_TYPE))
		{
			printf("Err : get DetectType is not available，pConfig->DetectType = %x \n ",pConfig->DetectType);
			return -1;
		}

		UINT8 getTransferType = pConfig->TransferType;
		#define MIN_TRANSFER_TYPE (0)
		#define MAX_TRANSFER_TYPE (2)
		if( (MIN_TRANSFER_TYPE > getTransferType) || (getTransferType > MAX_TRANSFER_TYPE))
		{
			printf("Err : get TransferType is not available，pConfig->TransferType = %x \n ",pConfig->TransferType);
			return -1;
		}

		UINT8 getEncryptType = pConfig->SrcUserInfo->EncryptType;
		#define MIN_ENCRYPT_TYPE (0)
		#define MAX_ENCRYPT_TYPE (1)
		if( (MIN_ENCRYPT_TYPE > getEncryptType) || (MAX_ENCRYPT_TYPE < getEncryptType))
		{
			printf("ERR : get EncryptType is not available，pConfig->EncryptType = %x \n",getEncryptType);
			return -1;
		}

		UINT8 getSrcIFInfo = pConfig->SrcIFInfo->IPType;
		#define MIN_SRC_IF_INFO (0)
		#define MAX_SRC_IF_INFO (1)
		if( (getSrcIFInfo < MIN_SRC_IF_INFO) || ( getSrcIFInfo > MAX_SRC_IF_INFO))
		{
			printf("ERR : get IPType is not available，pConfig->SrcIFInfo->IPType = %x \n",getSrcIFInfo);
			return -1;
		}

		UINT8 getImgType = pConfig->FrameInfo->ImgType;
		#define MIN_IMG_TYPE (0)
		#define MAX_IMG_TYPE (6)
		if((getImgType < MIN_IMG_TYPE)||(getImgType > MAX_IMG_TYPE))
		{
			printf("ERR : get ImgType is not available，pConfig->FrameInfo->ImgType = %x \n",getImgType);
			return -1;
		}

		//buffsize
		int getBuffSize = pConfig->BuffSize;
		if(0 == getBuffSize)
		{
			printf("ERR: get BuffSize is not available, pConfig->BuffSize = %x\n",pConfig->BuffSize);
			return -1;
		}

		if(NULL != m_saveTHFhand)
		{
			printf("m_saveTHFhand is aready init!\n");
			return -1;
		}
		

		
		// create the only one configmanage module.
		ConfigManage * pHandle = ConfigManage::getInstance();

		//useName != \0
		char getNameChar  =  pConfig->SrcUserInfo->UserName[0];
		char getKeyChar   =  pConfig->SrcUserInfo->UserPWD[0];
		if( ('\0' == getNameChar)||('\0' == getKeyChar) )
		{
			printf("Err : useName  is not available,UserName[0] = %x,UserPWD[0] = %x \n",getNameChar,getKeyChar);
			return -1;
		}

		//端口不为0，端口0 保留
		UINT8 getPort = pConfig->SrcIFInfo->CtrlPort;
		if(0 == getPort)
		{
			printf("Err : CtrlPort  is not available, CtrlPort = %d\n",getPort);
			return -1;
		}
		//IP地址不为0
		UINT8 sumIp0 = 0;
		for(int i = 0; i < 16; i++ )
		{
			if(0 == pConfig->SrcIFInfo->IPArray[i])
			{
				sumIp0++;
			}
		}
		if(16 == sumIp0)// 全为0，该IP含意
		{
			printf("Err : IPArray  is not available\n");
			return -1;
		}

		// init all the module by initconfigmanage
		int nRet = pHandle->initConfigManage( pConfig );

		if(0 == nRet)
		{
			*TFHandle =  pHandle;
			m_saveTHFhand = (UINT32 *)pHandle;
			printf("[LX] init TFHandle = %x \n",TFHandle);
		}
		else
		{
			*TFHandle = NULL;
			delete pHandle;
			printf("[LX] ERR :init TFHandle = %x \n",TFHandle);
		}
		
		return nRet;
	}

/*
  void *TFHandle             输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
  CallBackInfo *pCallbackFunc 输入,提供回调函数的指针,回调函数在事件发生时会调用
  int							<0 表示出错。0,表示成功。
  该接口用于用户向服务端指定接收反馈结果时的接口。须在初始化完成后执行一次
*/
	int registerCallback( void *TFHandle, CallBackInfo *pCallbackFunc)
	{
		if( (NULL == TFHandle ) || ( NULL == pCallbackFunc ) )
		{
			printf("ERR : %d %s TFHandle=%x, pCallback=%x\n",
				   __LINE__, __func__, TFHandle, pCallbackFunc  );
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}

		ProcessReq* pProcReq = ProcessReq::getInstance();
		return pProcReq->registerCallback( pCallbackFunc );
	}


/*
  void *TFHandle             输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
  EncodeCallBackInfo *EncodeCallBackInfo 输入,提供回调函数的指针,回调函数在事件发生时会调用
  int							<0 表示出错。0,表示成功。
  该接口用于用户注册编码的函数，如果有注册编码函数，那么就使用用户注册的编码函数，否则就用默认函数
*/
	int registerEncodeCallback( void *TFHandle, EncodeCallBackInfo *pEncodeCallbackFunc)
	{
		if( NULL == TFHandle   )
		{
			printf("ERR : %d %s TFHandle=%x\n",
				   __LINE__, __func__, TFHandle  );
			return -1;
		}
		

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		
		if( NULL == pEncodeCallbackFunc )
		{
			return 0;
		}

		ProcessReq* pProcReq = ProcessReq::getInstance();
		return pProcReq->registerEncodeCallback( pEncodeCallbackFunc );
	}



/*
  void *TFHandle  输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
  int <0 表示出错。0,表示成功。
  该接口用于用户启动SDK。在用户配置好SDK后,调用该接口启动SDK。SDK进入业务处理状态。须在初始化完成后执行一次
*/
	int startSDK( void *TFHandle)
	{
		if( nullptr == TFHandle )
		{
			printf("ERR: %d %s TFHandle=%p\n", __LINE__, __func__,
				   TFHandle );
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}


		int ret32 =  ProcessReq::getInstance()->startPthread();
		
		UINT32 getSaveBaudRate = ConfigManage::getInstance()->getBaudRate();
		ret32 |= ConfigManage::getInstance()->setBaudrate(getSaveBaudRate);
		ret32 |= ConfigManage::getInstance()->initGetSoftVersion();
		return ret32;
		
	}


/*
  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
  int								<0 表示出错。0,表示成功。
  该接口用于用户停止SDK。停止SDK时,SDK停止处理业务。但不会释放申请的各种资源。								
*/
	int stopSDK( void *TFHandle )
	{
		if( nullptr == TFHandle )
		{
			printf("ERR: %d %s TFHandle=%p\n", __LINE__, __func__,
				   TFHandle );
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}

		// TODO: stop by processReq, do not stop the pthread only,
		// but also the communicate
		return PthreadManage::getInstance()->stopPthread();
	}


								
/*
void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
UINT64 *pReqID								输出,保存ReqID,每次请求都提供唯一的ID
RWIFInfo *pClientInfo								输入,保存要登录客户端的接口信息
RWUserInfo *pUserInfo								输入,保存登录时登录使用的账号信息
int								<0 表示出错。0,表示执行成功,但能否注册成功,还需要等待反馈结果。
该接口用于用户通过SDK将指定的客户端登录到服务中。本接口成功执行后,只是发起了登录的会话。是否登录成功,是在callbackRelpyClientInfo（)接口中获得的。								
*/								
	int registerClient(void *TFHandle, UINT64 *pReqID, RWIFInfo *pClientInfo,
					   RWUserInfo *pUserInfo)
	{
//		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}


	
		return 0;
	}

								
								
								
/*								
void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
UINT64 *pReqID								输出,保存ReqID,每次请求都提供唯一的ID
RWIFInfo *pClientInfo								输入,保存要注销客户端的接口信息
RWUserInfo *pUserInfo								输入,保存登录时使用的账号信息
int								<0 表示出错。0,表示执行成功,但能否注销成功,还需要等待反馈结果。
该接口用于用户通过SDK将指定的客户端从服务端注销。本接口成功执行后,只是发起了注销的会话。是否注销成功,是在callbackRelpyClientInfo（)接口中获得的。								
*/
	int cancalClient(void *TFHandle,UINT64 *pReqID, RWIFInfo *pClientInfo,
					 RWUserInfo *pUserInfo)
	{
		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return 0;
	}

								
								
/*
  UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
  UINT8 *pRet								输入,表示本次反馈的成功/失败结果
  UINT8 *pReplyData								输入,反馈信息携带的额外数据
  UINT32 *pDataLena								输入,额外数据的数据长度
  -								-
  "该接口用于SDK向用户反馈登录请求的执行结果。成功时和失败时反馈的额外数据不同。
  *pRet=0时,*pReplyData的数据结构为：
  RWIFInfo;
  RWUserInfo;
  *pRet!=0时,*pReplyData的数据结构为：
  RWErrorInfo；
  unsigned char[]"								
*/
	void callbackRelpyRegisterClient(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,
									 UINT32 *pDataLen )
	{
//		printf("%d : %s\n", __LINE__, __func__);
	
	}

								
								
/*
  UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
  UINT8 *pRet								输入,表示本次反馈的成功/失败结果
  UINT8 *pReplyData								输入,反馈信息携带的额外数据
  UINT32 *pDataLena								输入,额外数据的数据长度
  -								-
  "该接口用于SDK向用户反馈注销请求的执行结果。成功时和失败时反馈的额外数据不同。
  *pRet=0时,*pReplyData的数据结构为：
  RWIFInfo;
  RWUserInfo;
  *pRet!=0时,*pReplyData的数据结构为：
  RWErrorInfo；
  unsigned char[]"								
*/
	void callbackReplyCancalClient(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,
								   UINT32 *pDataLen )
	{
		printf("%d : %s\n", __LINE__, __func__);
	
	}

								
/*								
RWNotifyType *pNotifyType								输入,指明通知的类型,进而确定pExtData中的数据内容
UINT8 *pExtData								输入,保存着通知的数据内容,不同通知的数据内容,数据结构是不同的。
UINT32 *pDataLen								输入,说明pExtData的内容的长度。
-								-
"该接口用于SDK向用户通知客户端状态的变化,比如客户端主动登录/注销,离线等。
*pNotifyType 的值为NotifyState时,*pExtData的数据结构为：
RWClientState;
RWIFInfo;
RWUserInfo;
*pNotifyType 的值为NotifyVersion时,*pExtData的数据结构为：
char[]"								
*/
	void callbackNotifyServiceInfo( RWNotifyType *pNotifyType, UINT8 *pExtData,
									UINT32 *pDataLen)
	{
		printf("%d : %s\n", __LINE__, __func__);
	
	}

								
/*								
RWNotifyType *pNotifyType								输入,指明通知的类型,进而确定pExtData中的数据内容
UINT8 *pExtData								输入,保存着通知的数据内容,不同通知的数据内容,数据结构是不同的。
UINT32 *pDataLen								输入,说明pExtData的内容的长度。
-								-
该接口用于SDK向用户通知图片信息。如有多少图片待上传,上传图片的数据等。
*pNotifyType 的值为NotifyImgInfo时,*pExtData的数据结构为：
RWPicNotify;
UINT64[]
*pNotifyType 的值为NotifyImgData时,*pExtData的数据结构为：
RWFaceInfo;
RWImgInfo;
*pNotifyType的值为NotifyCountFace时,*pExtData的数据结构为：
int ComeIn；
int GetOut；								
*/
	void callbackNotifyFaceInfo( RWNotifyType *pNotifyType, UINT8 *pExtData,
								 UINT32 *pDataLen )
	{
		printf("%d : %s\n", __LINE__, __func__);
	
	}

								
/*								
void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
RWUserInfo *pUserInfo								输入,添加的账号信息
int								<0 表示出错。0,表示执行成功
该接口用于用户向SDK添加账号信息。该账号信息将在客户端登录时使用。客户端使用未添加的账号信息登录时会失败								
*/
	int addUserInfo( void *TFHandle, RWUserInfo *pUserInfo )
	{
		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return 0;
	}

								
/*								
								void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
								int								SDK已经添加的账号信息的总数
								该接口用于用户查询SDK已经添加的账号信息数量。								
*/
	int getUserInfoSum(void *TFHandle )
	{
		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return 0;
	}

								
/*								
								void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
								UINT8 *pUserName								输出, 用于保存用户名,每个用户名最长64字节。
								UINT32 *pLen								输入,pUserName的字节长度,
								int								返回实际的UserName的个数
								该接口用于用户查询SDK已经添加的账号名称。								
*/
	int getUserNameList(void *TFHandle, UINT8 *pUserName, UINT32 nLen )
	{
		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}

		return 0;
	}

								
/*								
								void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
								UINT8 *pUserName								输入, 用于保存用户名,每个用户名最长64字节。
								UINT32 *pLen								输入,pUserName的字节长度,
								int								< 0: 删除失败； 0 ： 删除成功
								该接口用于用户删除SDK指定的账号信息。								
*/
	int deleteUserInfo( void *TFHandle,UINT8 *pUserName, UINT32 nLen )
	{
		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return 0;
	}


/*
  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
  RWIFInfo *pServiceInfo								输出, 用于保存服务端的接口配置信息。
  int								< 0: 失败； 0 ： 成功
  该接口用于用户获取SDK配置的接口信息。								
*/
	int getIFInfo(  void *TFHandle, RWIFInfo *pServiceInfo )
	{
		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return 0;
	}


/*
  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
  RWIFInfo *pServiceInfo								输入, 用于修改服务端的接口信息。
  int								< 0: 失败； 0 ： 成功
  该接口用于用户配置SDK的接口信息。在SDK处于运行状态前对SDK进行配置。运行时无法修改端口信息。								
*/
	int motifyIFInfo(  void *TFHandle,RWIFInfo *pServiceInfo )
	{
		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return 0;
	}


	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  char *protocolVersion							输出，输出获取到的版本数据
	  int len										输出，输出长度
	  int								< 0： 失败, 0：成功
	  该接口用于用户获得服务端SDK的版本,,版本信息通过接口pReqID返回： 返回规则是 大版本号*10 + 小版本号
	*/
	int getServiceSDKVersion(void *TFHandle,  char * protocolVersion , int len)
	{
		
		if( (nullptr == TFHandle) || ( nullptr == protocolVersion ) || (len <= 0) )
		{
			printf("ERR: %d %s TFHandle=%p, pVersion=%p, len=%p\n",
				   __LINE__, __func__, TFHandle, protocolVersion );
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		
		return ProcessReq::getInstance()->getServiceSDKVersion( protocolVersion, len );
	}


	/*
	  void *TFHandle								输入,SDK句柄。在初始化时获得,在调用本接口时使用。
	  UINT8 *pVersion								输出,用于缓存SDK的字符串；
	  int nLen								输入,用于说明pVersion提供的buff的字节长度。
	  int								< 0： 失败, 0：成功
	  该接口用于用户获得客户端SDK的版本								
	*/
	int getClientSDKVersion( void *TFHandle, UINT8 *pVersion, int nLen )
	{
		if( (nullptr == TFHandle) || ( nullptr == pVersion )
			|| ( nLen <=0 ) )
		{
			printf("ERR: %d %s TFHandle=%p, pVersion=%p, len=%d\n",
				   __LINE__, __func__, TFHandle, nLen );
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}

		
		return ProcessReq::getInstance()->getClientVersion( pVersion, nLen );
		
		
	}

/*
	void *TFHandle	输入，SDK句柄。在初始化时获得，在调用本接口时使用。
	VersionInfo *pVersion	输出，用于缓存版本的字符串；
	int								< 0： 失败， 0：成功
	该接口用于用户获得人脸检测模块的版本
*/
	int getFaceDetectVersionInfo(  void *TFHandle, VersionInfo *pVersion)
	{
		if( (nullptr == TFHandle) || ( nullptr == pVersion ) )
		{
			printf("ERR: %d %s TFHandle=%p, pVersion=%p\n",
				   __LINE__, __func__, TFHandle );
			pVersion = NULL;
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}

		return ProcessReq::getInstance()->getFaceDetectVersionInfo( pVersion );
	}
	/*
	  void *TFHandle								输入，初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  int								< 0： 失败， 0：成功
	  该接口用于用户卸载服务端SDK。								
	*/
	int uninitDetectFaceIF( void *TFHandle )
	{
		if(  NULL == TFHandle  )
		{
			printf("ERR %d %s TFHandle=%x\n", __LINE__, __func__,
				   TFHandle );
			return -1;
		}
		
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}


		// init all the module by initconfigmanage
		int nRet = ConfigManage::getInstance()->uninitConfigManage();
		
		return nRet;
	}


/*
  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
  RWClientState state								输入,只获得state指定状态的客户端数据
  int								符合条件的客户端数量
  该接口用于用户查询符合指定状态的客户端数量。当不设定state,即state=StateMax时,则返回所有状态的客户端数量。								
*/
	int getServiceSum( void *TFHandle, RWClientState state )
	{
		printf("%d : %s\n", __LINE__, __func__);
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return 0;
	}



/*
  RWUserInfo *pUsrInfo								输出,输出客户端ID指定的用户登录信息。
  RWIFInfo *pIFInfo								输出,输出客户端ID指定的接口信息。
  RWClientState *pState								输出,输出客户端ID指定的客户端状态
  UINT64 clientID								输入,指定要查询的客户端的ID
  int								< 0： 失败, 0：成功数量
  该接口用于用户查询符合指定状态的客户端数量。								
*/
	int getServiceInfo( RWUserInfo *pUsrInfo,RWIFInfo *pIFInfo, RWClientState *pState,
						UINT64 clientID )
	{
		printf("%d : %s\n", __LINE__, __func__);
	
		return 0;
	}
	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  RWUserInfo *pUsrInfo								输出,输出服务端ID指定的用户登录信息。
	  RWIFInfo *pIFInfo								输出,输出服务端ID指定的接口信息。
	  RWClientState *pState								输出,输出服务端ID指定的服务端状态
	  UINT64 clientID								输入,指定要查询的客户端的ID
	  int								< 0： 失败, 0：成功数量
	  该接口用于用户查询符合指定状态的服务端数量。								
	*/
	int addFrame(void *TFHandle, UINT8 *pImage )
	{
//		printf("%d : %s\n", __LINE__, __func__);
		if( ( NULL == TFHandle ) || ( NULL == pImage ) )
		{
			printf("ERR %d %s TFHandle=%x, pConfig=%x\n", __LINE__, __func__,
				   TFHandle, pImage );
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return ProcessReq::getInstance()->addFrame( pImage );
	}

	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  DetectCfg *pCfg								输入输出,pCfg指明要查找的内容,并接收查找的结果
	  int								< 0： 失败, 0：成功
	  "该接口用于用户查看SDK人脸检测相关的配置参数。在调用时设置 ConfigMask 标识指明要查看的内容。调用成功后,参数将保存到pCfg中。
	  "	*/
	int getDetectConfig( void *TFHandle, DetectCfg *pCfg )
	{
		if( ( NULL == TFHandle ) || ( NULL == pCfg ) )
		{
			printf("ERR %d %s TFHandle=%x, pConfig=%x\n", __LINE__, __func__,
				   TFHandle, pCfg );
			return -1;
		}
		
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}


		return ProcessReq::getInstance()->getDetectConfig( pCfg );
	}

	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  DetectCfg *pCfg								输入,pCfg指明要查找的内容,并接收查找的结果
	  int								< 0： 失败, 0：成功
	  "该接口用于用户设置SDK人脸检测相关的配置参数。在调用时设置 ConfigMask 标识指明要设置的项目。调用成功后,设定项将被同步到SDK中。
	  "								
	*/
	int setDetectConfig(void *TFHandle, DetectCfg *pCfg )
	{
		if( ( NULL == TFHandle ) || ( NULL == pCfg ) )
		{
			printf("ERR %d %s TFHandle=%x, pConfig=%x\n", __LINE__, __func__,
				   TFHandle, pCfg );
			return -1;
		}
		
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		
		return ProcessReq::getInstance()->setDetectConfig( pCfg );
	}
	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  Point *pPoint								输入,指向一个多边形的沿顺时针或逆时针排列的定点
	  int nCount								输入,定点的个数,最大8个
	  int								<0 表示出错。0,表示执行成功
	  该接口用于用户向SDK添加检测区域。最多可添加4个检测区域,每个区域支持8个定点					*/
	int addDetectRegion( void *TFHandle, Point *pPoint, int nCount )
	{
		//printf("%d : %s\n", __LINE__, __func__);

		if( ( NULL == TFHandle ) || ( NULL == pPoint ) || (nCount == 0))
		{
			printf("ERR %d %s TFHandle=%p, pConfig=%P nCount =  %d\n", __LINE__, __func__,
				   TFHandle, pPoint, nCount);
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		return ProcessReq::getInstance()->addDetectRegion( pPoint,nCount);
		
	}

	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  Point *pPointo								输入,指向一个多边形的沿顺时针或逆时针排列的定点
	  int nCount								输入,定点的个数,最大8个
	  int								<0 表示出错。0,表示执行成功
	  该接口用于用户向SDK删除检测区域。检测区域支持8个定点								
	*/
	int deletetDetectRegion( void *TFHandle, Point *pPoint, int nCount )
	{
		//printf("%d : %s\n", __LINE__, __func__);

		if( ( NULL == TFHandle ) || ( NULL == pPoint ) || (nCount == 0))
		{
			printf("ERR %d %s TFHandle=%x, pConfig=%x nCount =  %d\n", __LINE__, __func__,
				   TFHandle, pPoint, nCount);
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		
		return ProcessReq::getInstance()->deletetDetectRegion( pPoint,nCount);
	}


	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  Point *pPointo								输入,指向一个多边形的沿顺时针或逆时针排列的定点
	  int nCount								输入,定点的个数,最大8个
	  int								<0 表示出错。0,表示执行成功
	  该接口用于用户向SDK添加屏蔽区域。最多可添加1个检测区域,每个区域支持8个定点					*/
	int addShieldRegion( void *TFHandle, Point *pPoint, int nCount )
	{
		if( (NULL == TFHandle) || ( NULL == pPoint ) || (0 == nCount) )
		{
			printf("ERR %d %s TFHandle=%x, pPoint=%x, nCount = %d\n", __LINE__, __func__,
				   TFHandle, pPoint,  nCount);
		}
		
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}

	
	    return ProcessReq::getInstance()->addShieldRegion( pPoint,nCount);
	}


	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  Point *pPointo								输入,指向一个多边形的沿顺时针或逆时针排列的定点
	  int nCount								输入,定点的个数,最大8个
	  int								<0 表示出错。0,表示执行成功
	  该接口用于用户向SDK删除屏蔽区域。屏蔽区域支持8个定点								
	*/
	int deletetShieldRegion( void *TFHandle, Point *pPoint, int nCount )
	{

		if( (NULL == TFHandle) || ( NULL == pPoint ) || (0 == nCount) )
		{
			printf("ERR %d %s TFHandle=%x, pPoint=%x, nCount = %d\n", __LINE__, __func__,
				   TFHandle, pPoint,  nCount);
		}
		
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}

		return ProcessReq::getInstance()->deletetShieldRegion( pPoint,nCount);
	}



	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
	  UpgradeDataInfo *pDataInfo								输入,要升级的文件信息
	  CallbackReplyFunc *pFunc								输入,用于反馈升级各个过程执行的结果
	  int								<0 表示出错。0,表示执行成功
	  该接口用于用户向SDK请求进入升级模式。默认执行结果通过callbackReplyUpgradeReq(）反馈			*/
	int startUpgrade( void *TFHandle, UINT64 *pReqID, UpgradeDataInfo *pDataInfo,CallbackReplyFunc pFunc )//=callbackReplyUpgradeReq )
	{

		if( (NULL == TFHandle)||(NULL == pReqID)||(NULL == pDataInfo) )
		{
			printf("ERR %d %s TFHandle=%x,pReqID = %x,pDataInfo = %x \n", __LINE__, __func__,TFHandle,pReqID,pDataInfo);
		}
		
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}

		CallbackReplyFunc  tempCallFun;
		if(NULL == pFunc)
		{
			tempCallFun = callbackReplyUpgradeReq;
		}
		else
		{
			tempCallFun = pFunc;
		}
		
		return ProcessReq::getInstance()->startUpgrade( pReqID, pDataInfo, tempCallFun);
	}


	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
	  UINT8 *pData								输入,指向升级文件数据
	  UINT32 nLen								输入,升级文件大小
	  UINT32 nOffset								输入,本次发送的数据在整个文件中的偏移
	  int								<0 表示出错。0,表示执行成功
	  该接口用于用户向SDK发送文件数据。执行结果通过callbackReplyUpgradeReq(）反馈				*/
	int sendUpgradeData( void *TFHandle, UINT64 *pReqID, UINT8 *pData, UINT32 nLen,UINT32 nOffset )
	{
		if( (NULL == TFHandle) || (NULL == pReqID ) || (NULL == pData) )
		{
			printf("ERR %d %s TFHandle=%x,pReqID = %x,pData = %x\n", __LINE__, __func__,TFHandle, pReqID, pData);
		}
		
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
	
		return 0;
	}


	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
	  UpgradeDataInfo *pDataInfo								输入,要升级的文件信息
	  int								<0 表示出错。0,表示执行成功
	  该接口用于用户向SDK停止升级文件。执行结果通过callbackReplyUpgradeReq(）反馈				*/
	int stopUpgrade( void *TFHandle, UINT64 *pReqID, UpgradeDataInfo *pDataInfo )
	{

		if( (NULL == TFHandle) || (NULL == pReqID) )
		{
			printf("ERR %d %s TFHandle=%x,pReqID = %x,pDataInfo = %x\n", __LINE__, __func__,TFHandle,pReqID,pDataInfo);
		}
		
		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
	
		return 0;
	}


	/*
	  UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
	  UINT8 *pRet								输入,表示本次反馈的成功/失败结果
	  UINT8 *pReplyData								输入,反馈信息携带的额外数据
	  UINT32 *pDataLena								输入,额外数据的数据长度
	  -								-
	  "该接口用于SDK向用户反馈普通异步请求的执行结果。成功时和失败时反馈的额外数据不同。
	  *pRet=0时,*pReplyData的数据结构为：
	  UpgradeReqType;
	  *pRet!=0时,*pReplyData的数据结构为：
	  RWErrorInfo；
	  unsigned char[]"								
	*/
	void callbackReplyUpgradeReq(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,
								 UINT32 *pDataLen )
	{
		printf("%d : %s\n", __LINE__, __func__);
	
	}


	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  Line *pLine								输入,用于人流计数的参考线。可设置多条
	  CallbackNotifyFunc *pFunc								输入,在反馈结束结果时使用的回掉函数
	  int								<0 表示出错。0,表示执行成功
	  该接口用于SDK向用户反馈双向人流计数结果。默认执行结果通过callbackNotifyFaceInfo(）反馈。当设置多条时,跨越任意一条都产生结果								
	*/
	int setLine( void *TFHandle, Line *pLine,CallbackNotifyFunc pFunc )
	{
		//printf("%d : %s\n", __LINE__, __func__);
		if( (NULL == TFHandle) || ( NULL == pLine ) )
		{
			printf("%d : %s,TFHandle = %x,pLine = %x\n", __LINE__, __func__,TFHandle,pLine);
			return -1;
		}
		
		

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
		//CallbackNotifyFunc  temp;
		if(NULL == pFunc)
		{
			return ProcessReq::getInstance()->setLine(  pLine, callbackNotifyFaceInfo ); 
		}
		else
		{
			return ProcessReq::getInstance()->setLine(  pLine, pFunc );
		}
	
	}


	/*
	  void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	  int								<0 表示出错。0,表示执行成功
	  该接口用于用户清零SDK中的人流统计结果,清除参考线,清除回掉函数								*/
	int resetCounter( void *TFHandle )
	{
		if(NULL == TFHandle)
		{
			return -1;
		}

		if( m_saveTHFhand != ((UINT32 *)TFHandle) )
		{
			printf("ERR: %d %s ,TFHandle is not right! TFHandle = %x,m_saveTHFhand = %x\n ", __LINE__, __func__,TFHandle,m_saveTHFhand);
			return -1;
		}
	
		return ProcessReq::getInstance()->resetCounter( );
	}



#ifdef __cplusplus
}
#endif
