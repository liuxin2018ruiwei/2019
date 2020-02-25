/********************************************************************
* Name      :  ZmodeUpgrade.h                                  
* Describ   :  
* Author    :  liuxin                                       
* Date      :  2018-08-13                                                 
********************************************************************/

#ifndef ZmodeUpgradeHEAD_H
#define ZmodeUpgradeHEAD_H


typedef unsigned char UINT8;


class ZmodeUpgrade
{
 public: 

  void set_conio_terminal_mode();

  int test_UART(int uart_handle);

  const char *fname_skip_path(const char *fname);
  
  int start_install_QSPI(int uart_handle, char *fname);

  int initUart(const char *uart_dev, int *uart_handle);

  int send_file(int uart_handle, char *fname);
  	
  static ZmodeUpgrade* getInstance();
  
  int initZmodeUpgrade(void *priData );
  
  int uninitZmodeUpgrade();

  int startZmod( char *pathAndFileName );
  
  virtual ~ZmodeUpgrade();
 protected:
  ZmodeUpgrade();
  
 private:
  static ZmodeUpgrade* m_this;

};
#endif
