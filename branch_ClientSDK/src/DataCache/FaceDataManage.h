/********************************************************************
* Name      :  FaceDataManage.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-08                                                 
********************************************************************/

#ifndef FACEDATAMANAGE_H
#define FACEDATAMANAGE_H

#include "DetectFaceIF.h"
#include <vector>
using std :: vector;


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



typedef vector<FaceNode>  FaceNodeVec;

typedef FaceNodeVec::iterator  FaceNodeVecItor;

#define TIME_DELETE_CAPTURE	(2 * 1000)

class FaceDataManage
{
public: 
	#define  GET_LIST_STRATEGE (FaceDetector::getInstance()->getListStratege())

	FaceDataManage();

	virtual ~FaceDataManage();

	int findNode(FaceNode *faceNode);

	
	int deleteNode(UINT64 key);

	int refreshAllFace(RWFaceInfo *pExtData);


	int  refreshFaceNode(RWFaceInfo *pExtData);

	UINT64 getNowTimeMS();

	int  updataFaceNode(FaceNode * pFaceNode, RWFaceInfo *pREFaceInfo);

	int getFaceNodeByFaceInfo(FaceNode * pFaceData, RWFaceInfo *pREFaceInfo);

	int dealALLNode( FaceNode* node );
		
	static FaceDataManage* getInstance();
	vector<FaceNode>& getFaceNodeVec();

	int releaseData();
	//test
	//void pftFaceVec();
	
	int initMsgProcessor( void * priData );
	void uninitMsgProcessor( );
	
protected:
	
private:

	static FaceDataManage* m_this;

	vector<FaceNode> m_faceNodeVec;
};
#endif
