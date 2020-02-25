/********************************************************************
* Name      :  UpgradeRequestion.cpp                                  
* Describ   :  the definaition of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#include "UpgradeRequestion.h"
#include<cstring>


UpgradeRequestion::UpgradeRequestion( ReqID id, UpgradeDataInfo dataInfo ):Requestion( id )
{
	memcpy(&m_dataInfo, &dataInfo,sizeof(UpgradeDataInfo));
}



UpgradeDataInfo&  UpgradeRequestion::getUpgradeDataInfo()
{
	return m_dataInfo;
}

/*UINT64 UpgradeRequestion::getUserFlag()
{
	return ;
}
*/

UpgradeRequestion::~UpgradeRequestion()
{
	
}
