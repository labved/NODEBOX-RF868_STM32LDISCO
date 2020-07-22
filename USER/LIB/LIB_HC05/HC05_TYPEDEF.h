#include "stm32l1xx.h"
#include "Settings.h"

#ifdef LIB_HC05_H

#if (_USE_BT == 1)

enum HC05_Result
{
  HC05_OK = 0xFF,
  HC05_FAIL = 0xFE,
  HC05_ERR_TIMEOUT = 0xFD,
  HC05_ERR_ARGUMENT = 0xFC,

  HC05_ERR_DISC_LINK_LOSS = 0xFB,
  HC05_ERR_DISC_NO_SLC = 0xFA,
  HC05_ERR_DISC_TIMEOUT = 0xF9,
  HC05_ERR_DISC_ERROR = 0xF8,

  HC05_ERR_AT_COMMAND = 0x00,
  HC05_ERR_DEFAULT_RESULT = 0x01,
  HC05_ERR_PSKEY_WRITE = 0x02,
  HC05_ERR_DEVICE_NAME_TOO_LONG = 0x03,
  HC05_ERR_NO_DEVICE_NAME = 0x04,
  HC05_ERR_NAP_TOO_LONG = 0x05,
  HC05_ERR_UAP_TOO_LONG = 0x06,
  HC05_ERR_LAP_TOO_LONG = 0x07,
  HC05_ERR_NO_PIO_MASK = 0x08,
  HC05_ERR_NO_PIO_NUMBER = 0x09,
  HC05_ERR_NO_DEVICE_TYPE = 0x0A,
  HC05_ERR_DEVICE_TYPE_TOO_LONG = 0x0B,
  HC05_ERR_NO_IAC = 0x0C,
  HC05_ERR_IAC_TOO_LONG = 0x0D,
  HC05_ERR_INVALID_IAC = 0x0E,
  HC05_ERR_NO_PASSWORD = 0x0F,
  HC05_ERR_PASSWORD_TOO_LONG = 0x10,
  HC05_ERR_INVALID_MODULE_ROLE = 0x11,
  HC05_ERR_INVALID_BAUD_RATE = 0x12,
  HC05_ERR_INVALID_STOP_BITS = 0x13,
  HC05_ERR_INVALID_PARITY_BITS = 0x14,
  HC05_ERR_DEVICE_NOT_IN_LIST = 0x15,
  HC05_ERR_SPP_NOT_INITIALIZED = 0x16,
  HC05_ERR_SPP_REINIT = 0x17,
  HC05_ERR_INQUIRY_MODE = 0x18,
  HC05_ERR_INQ_TIMEOUT_TOO_LONG = 0x19,
  HC05_ERR_NO_BLUETOOTH_ADDRESS = 0x1A,
  HC05_ERR_SECURITY_MODE = 0x1B,
  HC05_ERR_ENCRYPTION_MODE = 0x1C
};

enum
{
  HC05_DEFAULT_TIMEOUT = 200,
  HC05_INQUIRY_DEFAULT_TIMEOUT = 10000,
  HC05_PAIRING_DEFAULT_TIMEOUT = 10000,
  HC05_PASSWORD_MAXLEN = 16,
  HC05_PASSWORD_BUFSIZE = HC05_PASSWORD_MAXLEN + 1,
  HC05_NAME_MAXLEN = 32,
  HC05_NAME_BUFSIZE = HC05_NAME_MAXLEN + 1,
  HC05_ADDRESS_MAXLEN = 14,
  HC05_ADDRESS_BUFSIZE = HC05_ADDRESS_MAXLEN + 1,
};



enum HC05_Role { HC05_ROLE_SLAVE = 0, HC05_ROLE_MASTER = 1, HC05_ROLE_SLAVE_LOOP = 2 };
enum HC05_InquiryMode { HC05_INQUIRY_STANDARD, HC05_INQUIRY_RSSI };
enum HC05_Parity { HC05_NO_PARITY, HC05_PARITY_ODD, HC05_PARITY_EVEN };
enum HC05_Connection { HC05_CONNECT_BOUND, HC05_CONNECT_ANY, HC05_CONNECT_SLAVE_LOOP };
enum HC05_Security { HC05_SEC_OFF, HC05_SEC_NON_SECURE,
  HC05_SEC_SERVICE, HC05_SEC_LINK, HC05_SEC_UNKNOWN };
enum HC05_Encryption { HC05_ENC_OFF, HC05_ENC_PTP, HC05_ENC_PTP_BROADCAST };

enum HC05_State
{
  HC05_INITIALIZED,
  HC05_READY,
  HC05_PAIRABLE,
  HC05_PAIRED,
  HC05_INQUIRING,
  HC05_CONNECTING,
  HC05_CONNECTED,
  HC05_DISCONNECTED,
  HC05_UNKNOWN
};

enum HC05_Mode { HC05_MODE_DATA = 0, HC05_MODE_COMMAND = 1 };

extern HC05_Result m_errCode;

#endif

#endif