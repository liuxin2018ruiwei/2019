/********************************************************************
* Name      :  Driver02_10_156.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef DRIVER02_10_156_H
#define DRIVER02_10_156_H

#include "Driver.h"


class Driver02_10_156:public Driver
{
	public: 
		Driver02_10_156( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();
		void writeInt32(UINT8* data, int value);
		UINT64 getNowTimeMS();
		virtual ~Driver02_10_156();
		void initRegion();
	protected: 
	private:
		void lxTest();

};
#endif
