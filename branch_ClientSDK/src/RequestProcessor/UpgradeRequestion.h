/********************************************************************
* Name      :  UpgradeRequestion.h                                  
* Describ   :  the class is used to save some common requestion from user
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#ifndef UPGRADEQUESTION_H
#define UPGRADEQUESTION_H

#include "Requestion.h"

#define   APP_CFG_UPGRADE_NAME  "app.cfg"
#define   APP_PKG_UPGRADE_NAME "app.pkg"
#define	  CAMERA_QSPI_IMAGE_UPGRADE_NAME "Camera-QSPI-image.bin"
typedef enum _upgradeType
{
	APP_CFG_TYPE = 0,
	APP_PKG_TYPE = 1,
	CAMERA_QSPI_IMAGE_TYPE = 2,
}UpgradeType;
#define MAX_UPGRADE_NAME_LEN (64)


class UpgradeRequestion:public Requestion
{
public: 
  UpgradeRequestion( ReqID id,   UpgradeDataInfo pDataInfo );

 // UINT64 getUserFlag();
  UpgradeDataInfo&  getUpgradeDataInfo();
  virtual ~UpgradeRequestion();
 protected:
 private:
  UpgradeDataInfo m_dataInfo;
};
#endif
