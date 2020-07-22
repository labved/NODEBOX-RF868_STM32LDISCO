#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_BT
#define _USE_BT
#endif

#if (_USE_BT == 1)

#include "HC05_TYPEDEF.h"



typedef uint8_t BluetoothAddress[6];
typedef void (*InquiryCallback)(const BluetoothAddress &address);


class LIB_HC05
{
public:
  void Init(void);
  void PoweronReset(void);
  void RegConfigSettings(void);
  void setAtMode(void);
  void setDataMode(void);
  void debugTerminal(char *msg);

public:
  //HardwareSerial *m_uart;
  unsigned long m_timeout;
  unsigned long m_ticksAtStart;
 

  // AT COMMANDS METHODS
  void testCommand(void);
  bool softReset(unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool probe(unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  
  bool getVersion(char *buffer, size_t buffer_size, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool restoreDefaults(unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getAddress(BluetoothAddress &address, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getName(char *buffer, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setName(const char *name, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getRemoteDeviceName(const BluetoothAddress &address,
    char *buffer, size_t buffer_size,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getRole(HC05_Role &role, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setRole(HC05_Role role, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getDeviceClass(uint32_t &device_class, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setDeviceClass(uint32_t device_class, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getInquiryAccessCode(uint32_t &iac, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setInquiryAccessCode(uint32_t iac, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getInquiryMode(HC05_InquiryMode &inq_mode, int16_t &max_devices,
    uint8_t &max_duration, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setInquiryMode(HC05_InquiryMode inq_mode, int16_t max_devices,
    uint8_t max_duration, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getPassword(char *buffer, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setPassword(const char *password, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getSerialMode(uint32_t &speed, uint8_t &stop_bits,
    HC05_Parity &parity, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setSerialMode(uint32_t speed, uint8_t stop_bits,
    HC05_Parity parity, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getConnectionMode(HC05_Connection &connection_mode,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setConnectionMode(HC05_Connection connection_mode,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool bind(const BluetoothAddress &address, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getAddressBound(BluetoothAddress &address,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getLeds(bool &led_status, bool &led_connection, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setLeds(bool led_status, bool led_connection, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setPortState(uint8_t port_num, uint8_t port_state, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getMultiplePorts(uint16_t &port_states, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setMultiplePorts(uint16_t port_states, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getInquiryAndPagingParams(uint16_t &inquiry_interval, uint16_t &inquiry_duration,
    uint16_t &paging_interval, uint16_t &paging_duration,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setInquiryAndPagingParams(uint16_t inquiry_interval, uint16_t inquiry_duration,
    uint16_t paging_interval, uint16_t paging_duration,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getSniffParams(uint16_t &max_time, uint16_t &min_time,
    uint16_t &retry_interval, uint16_t &sniff_timeout,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setSniffParams(uint16_t max_time, uint16_t min_time,
    uint16_t retry_interval, uint16_t sniff_timeout,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool enterSniffMode(unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool exitSniffMode(unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getSecurityAndEncryption(HC05_Security &security,
    HC05_Encryption &encryption, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool setSecurityAndEncryption(HC05_Security security,
    HC05_Encryption encryption, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool deleteDeviceFromList(const BluetoothAddress &address,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool deleteAllDevicesFromList(unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool findDeviceInList(const BluetoothAddress &address,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool countDevicesInList(uint8_t &device_count,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getLastAuthenticatedDevice(BluetoothAddress &address,
    unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool getState(HC05_State &state, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool initSerialPortProfile(unsigned long timeout = HC05_DEFAULT_TIMEOUT);

  bool inquire(InquiryCallback callback, unsigned long timeout = HC05_INQUIRY_DEFAULT_TIMEOUT);
  bool cancelInquiry(unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool pair(const BluetoothAddress &address, unsigned long timeout = HC05_PAIRING_DEFAULT_TIMEOUT);
  bool connect(const BluetoothAddress &address, unsigned long timeout = HC05_DEFAULT_TIMEOUT);
  bool disconnect(unsigned long timeout = HC05_DEFAULT_TIMEOUT);

  // DATA METHODS
  bool sendData(byte *msg);
  bool getData(byte *buffer, size_t buffer_size);
  
  

};

#endif

