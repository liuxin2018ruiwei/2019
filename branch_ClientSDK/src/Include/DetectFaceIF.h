/********************************************************************
 * Name      :  DetectFaceIF.h                                  
 * Describ   :  the defination of the interface users will call                   
 * Author    :  Zhang Botong                                          
 * Date      :  2018-06-04                                                 
 ********************************************************************/

#ifndef DETECTFACEIF_H
#define DETECTFACEIF_H

#include "DetectFaceIFCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif
//test
/*

//Driver02_10_140 test use   this  ;close FaceDataManage.h  struct FaceNode
  typedef struct _strFaceNode
  {
	  UINT64 frameNumber;//图片编号
	  UINT64 TrackNo; //跟踪号
	  UINT32 RectX; // 人脸框左上角的横坐标
	  UINT32 RectY; //人脸框左上角的纵坐标
	  UINT32 RectWidth; // 人脸框的宽
	  UINT32 RectHeight; // 人脸框的高
	  UINT8  RectType; //人脸框类型： 1：头肩框， 2：半身照框，3：全身照框， 4：人脸框
	  float  QScore;//图片质量分
	  UINT64 InsertTime; //创建时间；
	  UINT64 RefreshTime;//更新时间；
	  UINT64 SelectTime;//抓拍时间
  }FaceNode;


  int testsSelectFace(RWFaceInfo *pExtData);
  int testFilterFace(RWFaceInfo *pExtData);
  int testRefreshAfterFilter(RWFaceInfo *pExtData);
  int getModuleProtocolVersion( char *pVersion, int nLen );
  int setModuleProtocolVersion( char * protocolVersion, int nLen );
  
  int getModuleSoftwareVersion( char *pVersion, int nLen );
  int setModuleSoftwareVersion( char * softwareVersion, int nLen );
  
  int getModuleFirmwareVersion( char *pVersion, int nLen );
  int setModuleFirmwareVersion( char * firmwareVersion, int nLen );
  int testHaisi();
  int testZmode(char *pathAndFileName );
 // int testFaceImgName( char *outName, int *outLen,int inputLen, FaceNode * faceNode );
 // int testBackImgName( char *outName, int *outLen,int inputLen, FaceNode * faceNode );
 */
//test
 
  /*
	void **TFHandle             输出,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用							
	TransportConfigInfo * pConfig     输入，提供配置SDK的必要参数
	int						   reture <0 表示出错。0,表示成功。									 					
	该接口用于用户对服务端进行初始化。初始化完成后即可进行业务通信。
  */
  int initDetectFaceIF( void **TFHandle, DetectConfigInfo * pConfig);

  /*
	void *TFHandle             输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	CallBackInfo *pCallbackFunc 输入,提供回调函数的指针,回调函数在事件发生时会调用
	int							<0 表示出错。0,表示成功。
	该接口用于用户向服务端指定接收反馈结果时的接口。须在初始化完成后执行一次
  */
  int registerCallback( void *TFHandle, CallBackInfo *pCallbackFunc);


  /*
	void *TFHandle			   输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	CallBackInfo *pCallbackFunc 输入,提供回调函数的指针,回调函数在事件发生时会调用
	int 						  <0 表示出错。0,表示成功。
	该接口用于用户注册编码的函数，如果有注册编码函数，那么就使用用户注册的编码函数，否则就用默认函数
  */
  int registerEncodeCallback( void *TFHandle, EncodeCallBackInfo *pEncodeCallbackFunc);

  /*
	void *TFHandle  输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	int <0 表示出错。0,表示成功。
	该接口用于用户启动SDK。在用户配置好SDK后,调用该接口启动SDK。SDK进入业务处理状态。须在初始化完成后执行一次
  */
  int startSDK( void *TFHandle);

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	int								<0 表示出错。0,表示成功。
	该接口用于用户停止SDK。停止SDK时,SDK停止处理业务。但不会释放申请的各种资源。								
  */
  int stopSDK( void *TFHandle );

								
  /* 
	 void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	 UINT64 *pReqID								输出,保存ReqID,每次请求都提供唯一的ID
	 RWIFInfo *pClientInfo								输入,保存要登录客户端的接口信息
	 RWUserInfo *pUserInfo								输入,保存登录时登录使用的账号信息
	 int								<0 表示出错。0,表示执行成功,但能否注册成功,还需要等待反馈结果。
	 该接口用于用户通过SDK将指定的客户端登录到服务中。本接口成功执行后,只是发起了登录的会话。是否登录成功,是在callbackRelpyClientInfo()接口中获得的。								
  */								
  int registerClient(void *TFHandle, UINT64 *pReqID, RWIFInfo *pClientInfo,
					 RWUserInfo *pUserInfo);
  /*void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	UINT64 *pReqID								输出,保存ReqID,每次请求都提供唯一的ID
	RWIFInfo *pClientInfo								输入,保存要注销客户端的接口信息
	RWUserInfo *pUserInfo								输入,保存登录时使用的账号信息
	int								<0 表示出错。0,表示执行成功,但能否注销成功,还需要等待反馈结果。
	该接口用于用户通过SDK将指定的客户端从服务端注销。本接口成功执行后,只是发起了注销的会话。是否注销成功,是在callbackRelpyClientInfo()接口中获得的。								
  */
  int cancalClient(void *TFHandle,UINT64 *pReqID, RWIFInfo *pClientInfo, RWUserInfo *pUserInfo);
								
								
  
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
								   UINT32 *pDataLen );
  
  
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
								 UINT32 *pDataLen );
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
  void callbackNotifyServiceInfo( RWNotifyType *pNotifyType, UINT8 *pExtData, UINT32 *pDataLen);
  /*
	RWNotifyType *pNotifyType								输入,指明通知的类型,进而确定pExtData中的数据内容
	UINT8 *pExtData								输入,保存着通知的数据内容,不同通知的数据内容,数据结构是不同的。
	UINT32 *pDataLen								输入,说明pExtData的内容的长度。
	-								-
	"该接口用于SDK向用户通知图片信息。如有多少图片待上传,上传图片的数据等。
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
							   UINT32 *pDataLen );
  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	RWUserInfo *pUserInfo								输入,添加的账号信息
	int								<0 表示出错。0,表示执行成功
	该接口用于用户向SDK添加账号信息。该账号信息将在客户端登录时使用。客户端使用未添加的账号信息登录时会失败								
  */
  int addUserInfo( void *TFHandle, RWUserInfo *pUserInfo );
  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	int								SDK已经添加的账号信息的总数
	该接口用于用户查询SDK已经添加的账号信息数量。								
  */
  int getUserInfoSum(void *TFHandle );
  
  /*								
 void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
 UINT8 *pUserName								输出, 用于保存用户名,每个用户名最长64字节。
 UINT32 *pLen								输入,pUserName的字节长度,
 int								返回实际的UserName的个数
 该接口用于用户查询SDK已经添加的账号名称。								
  */
  int getUserNameList(void *TFHandle, UINT8 *pUserName, UINT32 nLen );
								
  /*								
 void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
 UINT8 *pUserName								输入, 用于保存用户名,每个用户名最长64字节。
 UINT32 *pLen								输入,pUserName的字节长度,
 int								< 0: 删除失败； 0 ： 删除成功
 该接口用于用户删除SDK指定的账号信息。								
  */
  int deleteUserInfo( void *TFHandle,UINT8 *pUserName, UINT32 nLen );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	RWIFInfo *pServiceInfo								输出, 用于保存服务端的接口配置信息。
	int								< 0: 失败； 0 ： 成功
	该接口用于用户获取SDK配置的接口信息。								
  */
  int getIFInfo(  void *TFHandle, RWIFInfo *pServiceInfo );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	RWIFInfo *pServiceInfo								输入, 用于修改服务端的接口信息。
	int								< 0: 失败； 0 ： 成功
	该接口用于用户配置SDK的接口信息。在SDK处于运行状态前对SDK进行配置。运行时无法修改端口信息。								
  */
  int motifyIFInfo(  void *TFHandle,RWIFInfo *pServiceInfo );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
	int								< 0： 失败, 0：成功
	该接口用于用户获得服务端SDK的版本,,版本信息通过接口callbackNotifyClientInfo(）通过到用户
  */
  int getServiceSDKVersion(void *TFHandle,  char * protocolVersion , int len);

  /*
	void *TFHandle								输入,SDK句柄。在初始化时获得,在调用本接口时使用。
	UINT8 *pVersion								输出,用于缓存SDK的字符串；
	int nLen								输入,用于说明pVersion提供的buff的字节长度。
	int								< 0： 失败, 0：成功
	该接口用于用户获得客户端SDK的版本								
  */
  int getClientSDKVersion( void *TFHandle, UINT8 *pVersion, int nLen );

  	/*
void *TFHandle	输入，SDK句柄。在初始化时获得，在调用本接口时使用。
VersionInfo *pVersion	输出，用于缓存版本的字符串；
int								< 0： 失败， 0：成功
该接口用于用户获得人脸检测模块的版本
	*/
  int getFaceDetectVersionInfo(  void *TFHandle, VersionInfo *pVersion);

  /*
 void *TFHandle								输入，初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
int								< 0： 失败， 0：成功
该接口用于用户卸载服务端SDK。								
  */
  int uninitDetectFaceIF( void *TFHandle );
  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	RWClientState state								输入,只获得state指定状态的客户端数据
	int								符合条件的客户端数量
	该接口用于用户查询符合指定状态的客户端数量。当不设定state,即state=StateMax时,则返回所有状态的客户端数量。								
  */
  int getServiceSum( void *TFHandle, RWClientState state );// = StateMax );

  /*
	RWUserInfo *pUsrInfo								输出,输出客户端ID指定的用户登录信息。
	RWIFInfo *pIFInfo								输出,输出客户端ID指定的接口信息。
	RWClientState *pState								输出,输出客户端ID指定的客户端状态
	UINT64 clientID								输入,指定要查询的客户端的ID
	int								< 0： 失败, 0：成功数量
	该接口用于用户查询符合指定状态的客户端数量。								
  */
  int getServiceInfo( RWUserInfo *pUsrInfo,RWIFInfo *pIFInfo, RWClientState *pState,
					  UINT64 clientID );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	RWUserInfo *pUsrInfo								输出,输出服务端ID指定的用户登录信息。
	RWIFInfo *pIFInfo								输出,输出服务端ID指定的接口信息。
	RWClientState *pState								输出,输出服务端ID指定的服务端状态
	UINT64 clientID								输入,指定要查询的客户端的ID
	int								< 0： 失败, 0：成功数量
	该接口用于用户查询符合指定状态的服务端数量。								
  */
  int addFrame(void *TFHandle, UINT8 *pImage );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	DetectCfg *pCfg								输入输出,pCfg指明要查找的内容,并接收查找的结果
	int								< 0： 失败, 0：成功
	"该接口用于用户查看SDK人脸检测相关的配置参数。在调用时设置 ConfigMask 标识指明要查看的内容。调用成功后,参数将保存到pCfg中。
	"	*/
  int getDetectConfig( void *TFHandle, DetectCfg *pCfg );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	DetectCfg *pCfg								输入,pCfg指明要查找的内容,并接收查找的结果
	int								< 0： 失败, 0：成功
	"该接口用于用户设置SDK人脸检测相关的配置参数。在调用时设置 ConfigMask 标识指明要设置的项目。调用成功后,设定项将被同步到SDK中。
	"								
  */
  int setDetectConfig(void *TFHandle, DetectCfg *pCfg );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	Point *pPoint								输入,指向一个多边形的沿顺时针或逆时针排列的定点
	int nCount								输入,定点的个数,最大8个
	int								<0 表示出错。0,表示执行成功
	该接口用于用户向SDK添加检测区域。最多可添加4个检测区域,每个区域支持8个定点					*/
  int addDetectRegion( void *TFHandle, Point *pPoint, int nCount );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	Point *pPointo								输入,指向一个多边形的沿顺时针或逆时针排列的定点
	int nCount								输入,定点的个数,最大8个
	int								<0 表示出错。0,表示执行成功
	该接口用于用户向SDK删除检测区域。检测区域支持8个定点								
  */
  int deletetDetectRegion( void *TFHandle, Point *pPoint, int nCount );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	Point *pPointo								输入,指向一个多边形的沿顺时针或逆时针排列的定点
	int nCount								输入,定点的个数,最大8个
	int								<0 表示出错。0,表示执行成功
	该接口用于用户向SDK添加屏蔽区域。最多可添加1个检测区域,每个区域支持8个定点					*/
  int addShieldRegion( void *TFHandle, Point *pPoint, int nCount );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	Point *pPointo								输入,指向一个多边形的沿顺时针或逆时针排列的定点
	int nCount								输入,定点的个数,最大8个
	int								<0 表示出错。0,表示执行成功
	该接口用于用户向SDK删除屏蔽区域。屏蔽区域支持8个定点								
  */
  int deletetShieldRegion( void *TFHandle, Point *pPoint, int nCount );


  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
	UpgradeDataInfo *pDataInfo								输入,要升级的文件信息
	CallbackReplyFunc *pFunc								输入,用于反馈升级各个过程执行的结果
	int								<0 表示出错。0,表示执行成功
	该接口用于用户向SDK请求进入升级模式。默认执行结果通过callbackReplyUpgradeReq(）反馈			*/
  int startUpgrade( void *TFHandle, UINT64 *pReqID, UpgradeDataInfo *pDataInfo,CallbackReplyFunc pFunc );//=callbackReplyUpgradeReq );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
	UINT8 *pData								输入,指向升级文件数据
	UINT32 nLen								输入,升级文件大小
	UINT32 nOffset								输入,本次发送的数据在整个文件中的偏移
	int								<0 表示出错。0,表示执行成功
	该接口用于用户向SDK发送文件数据。执行结果通过callbackReplyUpgradeReq(）反馈				*/
  int sendUpgradeData( void *TFHandle, UINT64 *pReqID, UINT8 *pData, UINT32 nLen,UINT32 nOffset );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	UINT64 *pReqID								输入,表示本次反馈的ReqID指定请求的结果,ReqID具有唯一性
	UpgradeDataInfo *pDataInfo								输入,要升级的文件信息
	int								<0 表示出错。0,表示执行成功
	该接口用于用户向SDK停止升级文件。执行结果通过callbackReplyUpgradeReq(）反馈				*/
  int stopUpgrade( void *TFHandle, UINT64 *pReqID, UpgradeDataInfo *pDataInfo );

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
							   UINT32 *pDataLen );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	Line *pLine								输入,用于人流计数的参考线。可设置多条
	CallbackNotifyFunc *pFunc								输入,在反馈结束结果时使用的回掉函数
	int								<0 表示出错。0,表示执行成功
	该接口用于SDK向用户反馈双向人流计数结果。默认执行结果通过callbackNotifyFaceInfo(）反馈。当设置多条时,跨越任意一条都产生结果								
  */
  int setLine( void *TFHandle, Line *pLine,CallbackNotifyFunc pFunc );//=callbackNotifyFaceInfo );

  /*
	void *TFHandle								输入,初始化成功后返回的句柄。用户要保留此Handle。接口调用时会使用。
	int								<0 表示出错。0,表示执行成功
	该接口用于用户清零SDK中的人流统计结果,清除参考线,清除回掉函数								*/
  int resetCounter( void *TFHandle );

#ifdef __cplusplus
}
#endif

#endif
