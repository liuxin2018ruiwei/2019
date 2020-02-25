/********************************************************************
* Name      :  ST_1_initDetectFaceIF.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef ST_1_initDetectFaceIF_H
#define ST_1_initDetectFaceIF_H

#include "Driver.h"


class ST_1_initDetectFaceIF:public Driver
{
	public: 
		ST_1_initDetectFaceIF( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();

		virtual ~ST_1_initDetectFaceIF();
		void initRegion();

		int init_pConfig(DetectConfigInfo **pCfgInfo);
	protected: 
	private:
	DetectConfigInfo cfgInfo;
	RWImgInfo imgInfo;
	SerialPortInfo serialInfo;
	RWIFInfo ifInfo;
	RWUserInfo userInfo;

};
#endif
