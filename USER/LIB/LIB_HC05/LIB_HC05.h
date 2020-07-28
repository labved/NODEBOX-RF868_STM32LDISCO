#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HC05_TYPEDEF.h"

#ifndef LIB_HC05_H
  #define LIB_HC05_H

#if (_USE_BT == 1)



typedef uint8_t BluetoothAddress[6];
typedef void (*InquiryCallback)(const BluetoothAddress &address);

class LIB_HC05
{
private:
  void                    powerOnReset(void);
  void                    regConfigSettings(void);
  void                    setAtMode(void);
  void                    setDataMode(void);
  void                    debugTerminal(char const *msg);


public:
  //HardwareSerial *m_uart;
  unsigned long m_timeout;
  unsigned long m_ticksAtStart;

  void                    test(void);    // to be deleted 
  void                    Init(void);
  bool                    softReset(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    probe(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT  //need to check

  // AT                    COMMANDS METHODS
  bool                    setFactoryDefault(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT   // need to check
  bool                    getVersion(char *buffer, size_t buffer_size, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getAddress(BluetoothAddress &address, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT//need to check
  bool                    getName(char *buffer, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT   //need to check
  bool                    setName(const char *name, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getRemoteDeviceName(const BluetoothAddress &address,
                                                    char *buffer, size_t buffer_size, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT //need to check
  bool                    getRole(HC05_Role &role, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setRole(HC05_Role role, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getDeviceClass(uint32_t &device_class, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setDeviceClass(uint32_t device_class, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getInquiryAccessCode(uint32_t &iac, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setInquiryAccessCode(uint32_t iac, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getInquiryMode(HC05_InquiryMode &inq_mode, int16_t &max_devices,
                                                    uint8_t &max_duration, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setInquiryMode(HC05_InquiryMode inq_mode, int16_t max_devices,
                                                    uint8_t max_duration, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getPassword(char *buffer, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setPassword(const char *password, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getSerialMode(uint32_t &speed, uint8_t &stop_bits,
                                                    HC05_Parity &parity, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setSerialMode(uint32_t speed, uint8_t stop_bits,
                                                    HC05_Parity parity, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getConnectionMode(HC05_Connection &connection_mode,
                                                    unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setConnectionMode(HC05_Connection connection_mode,
                                                    unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    bind(const BluetoothAddress &address, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT  //need to check
  bool                    getAddressBound(BluetoothAddress &address,
                                                    unsigned long timeout);     //HC05_DEFAULT_TIMEOUT  //need to check
  bool                    getLeds(bool &led_status, bool &led_connection, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setLeds(bool led_status, bool led_connection, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setPortState(uint8_t port_num, uint8_t port_state, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getMultiplePorts(uint16_t &port_states, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setMultiplePorts(uint16_t port_states, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getInquiryAndPagingParams(uint16_t &inquiry_interval, uint16_t &inquiry_duration, uint16_t &paging_interval, uint16_t &paging_duration, unsigned long timeout);

  bool                    setInquiryAndPagingParams(uint16_t inquiry_interval, uint16_t inquiry_duration,
                                                    uint16_t paging_interval, uint16_t paging_duration,
                                                    unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getSniffParams(uint16_t &max_time, uint16_t &min_time,
                                                    uint16_t &retry_interval, uint16_t &sniff_timeout,
                                                    unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setSniffParams(uint16_t max_time, uint16_t min_time,
                                                    uint16_t retry_interval, uint16_t sniff_timeout,
                                                    unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    enterSniffMode(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    exitSniffMode(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getSecurityAndEncryption(HC05_Security &security, HC05_Encryption &encryption,
                                                   unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    setSecurityAndEncryption(HC05_Security security, HC05_Encryption encryption,
                                                   unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    deleteDeviceFromList(const BluetoothAddress &address, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT   //need to check
  bool                    deleteAllDevicesFromList(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    findDeviceInList(const BluetoothAddress &address,unsigned long timeout);     //HC05_DEFAULT_TIMEOUT //need to check
  bool                    countDevicesInList(uint8_t &device_count, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    getLastAuthenticatedDevice(BluetoothAddress &address, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT //need to check
  bool                    getState(HC05_State &state, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    initSerialPortProfile(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    inquire(InquiryCallback callback, unsigned long timeout = HC05_INQUIRY_DEFAULT_TIMEOUT); //need to check
  bool                    cancelInquiry(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT
  bool                    pair(const BluetoothAddress &address, unsigned long timeout = HC05_PAIRING_DEFAULT_TIMEOUT);    //need to check
  bool                    connect(const BluetoothAddress &address, unsigned long timeout);     //HC05_DEFAULT_TIMEOUT      //need to check
  bool                    disconnect(unsigned long timeout);     //HC05_DEFAULT_TIMEOUT   //need to check
  
  //new
  bool processVersion(uint8_t answer, char *buffer, size_t buffer_size, uint16_t addrs);//need to check 
  bool processRemoteDeviceName(const BluetoothAddress &address,
  char *buffer, size_t buffer_size, uint16_t addrs);
  bool processDeviceClass(uint32_t &device_class, uint16_t addrs);
  bool processInquiryAccessCode( uint32_t &iac, uint16_t addrs);
  bool processInquiryMode(HC05_InquiryMode &inq_mode,
  int16_t &max_devices, uint8_t &max_duration, uint16_t addrs);
  bool processPassword(char *buffer, uint16_t addrs);
  bool processSerialMode(uint32_t &speed, uint8_t &stop_bits,
  HC05_Parity &parity, uint16_t addrs);
  bool processConnectionMode(
  HC05_Connection &connection_mode, uint16_t addrs);
  bool processLed(bool &led_status,
  bool &led_connection,uint16_t addrs);
  bool processPortState(uint8_t port_num,
  uint8_t port_state, uint16_t addrs);
  bool processMultiPortState(uint16_t &port_states, uint16_t addrs);
  bool processGetInquiryAndPagingParams(uint16_t &inquiry_interval, 
uint16_t &inquiry_duration, uint16_t &paging_interval, uint16_t &paging_duration, uint16_t addrs);
  bool processSecurityAndEncryption(HC05_Security &security,
  HC05_Encryption &encryption, uint16_t addrs);
  bool processState(HC05_State &state, uint16_t addrs);      //need to check
  bool processPair(uint16_t addrs);      //need to check
  bool processDisconnect(uint16_t addrs);    //need to check
  bool processCountDevicesInList(uint8_t &device_count, uint16_t addrs);
 // bool processDeleteAllDevicesFromList( uint16_t addrs);
  bool processSniffParams(uint16_t max_time, uint16_t min_time,
  uint16_t retry_interval, uint16_t sniff_timeout, uint16_t addrs);
  bool processRole(HC05_Role &role, uint16_t addrs);       //need to check
  bool processName(char *buffer, uint16_t addrs);
  bool processHttpData();
  bool processHttpAction();
  bool  processHttpRead();
  bool processHttpContent();

  
  
  //

};


extern LIB_HC05 bluetooth;

#endif

#endif