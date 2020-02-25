/********************************************************************
* Name      :  Driver02_10_170.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef Driver02_10_170_H
#define Driver02_10_170_H

#include "Driver.h"


class Driver02_10_170:public Driver
{
	public: 
		Driver02_10_170( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();

		virtual ~Driver02_10_170();
		void initRegion();
	protected: 
	private:
		

};
#endif
