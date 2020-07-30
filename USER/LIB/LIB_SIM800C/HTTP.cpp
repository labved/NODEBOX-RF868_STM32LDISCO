#include "stm32l1xx.h" 
#include "Settings.h" 
#include "Pinouts.h"
#include "Shortcodes.h"
#include "Variables.h"

#include "Functions.h"
#include "Conversions.h"

#include "HAL_SIM800C.h"
#include "LIB_SIM800C.h"

#ifdef LIB_SIM800C_H

#if(_USE_GSM==1)

extern HAL_SIM800C              sim800c;
extern Sim80x_t                 Sim80x;

#if (_SIM80X_USE_BLUETOOTH==1)

/****************************************************************
*FUNCTION NAME:httpInit
*FUNCTION     :httpInit
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool LIB_SIM800C::httpInit(void)
{  
    uint8_t answer;

    answer = sim800c.sendAtCommand("AT+HTTPINIT\r\n",200, 0);

    debugTerminal("Sim80x_httpInit");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;
}

/****************************************************************
*FUNCTION NAME:httpTerminate
*FUNCTION     :httpTerminate
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool LIB_SIM800C::httpTerminate(void)
{
  
    uint8_t answer;

    answer = sim800c.sendAtCommand("+++\r\n",200, 0);

    debugTerminal("Sim80x_httpTerminate");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;

}


/****************************************************************
*FUNCTION NAME:getHttpParam
*FUNCTION     :getHttpParam
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::getHttpParam(void)
{
    uint8_t answer;

    answer = sim800c.sendAtCommand("AT+HTTPPARA\r\n",200, 0);

    debugTerminal("Sim80x_getHttpParam");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;
}

/****************************************************************
*FUNCTION NAME:getHttpData
*FUNCTION     :getHttpData
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::getHttpData(void)
{
    uint8_t answer;

    answer = sim800c.sendAtCommand("AT+HTTPDATA\r\n",200, 0);

    debugTerminal("Sim80x_getHttpData");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;

}

/****************************************************************
*FUNCTION NAME:getHttpAction
*FUNCTION     :getHttpAction
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::getHttpAction(void)
{
    uint8_t answer;

    answer = sim800c.sendAtCommand("AT+HTTPACTION\r\n",200, 0);

    debugTerminal("Sim80x_getHttpAction");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;

}

/****************************************************************
*FUNCTION NAME:getHttpRead
*FUNCTION     :getHttpRead
*INPUT        :void
*OUTPUT       :void
****************************************************************/
bool LIB_SIM800C::getHttpRead(void)
{
    uint8_t answer;

    answer = sim800c.sendAtCommand("AT+HTTPREAD\r\n",200, 0);

    debugTerminal("Sim80x_getHttpRead");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;

}

/****************************************************************
*FUNCTION NAME:getHttpContext
*FUNCTION     :getHttpContext
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::getHttpContext(void)
{
    uint8_t answer;

    answer =  sim800c.sendAtCommand("AT+HTTPCONT\r\n",200, 0);

    debugTerminal("Sim80x_getHttpContext");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;

}

/****************************************************************
*FUNCTION NAME:getHttpStatus
*FUNCTION     :getHttpStatus
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::getHttpStatus(void)
{
    uint8_t answer;

    answer =  sim800c.sendAtCommand("AT+HTTPSTATUS\r\n",200, 0);

    debugTerminal("Sim80x_getHttpStatus");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;

}

/****************************************************************
*FUNCTION NAME:getHttpHeader
*FUNCTION     :getHttpHeader
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::getHttpHeader(void)
{
    uint8_t answer;

    answer = sim800c.sendAtCommand("AT+HTTPHEAD\r\n",200, 0);

    debugTerminal("Sim80x_getHttpHeader");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;

}


/****************************************************************
*FUNCTION NAME:setHttpParam
*FUNCTION     :setHttpParam
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::setHttpParam(void)
{
    uint8_t answer;

    answer =  sim800c.sendAtCommand("AT+HTTPPARA\r\n",200, 0);

    debugTerminal("Sim80x_setHttpParam");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;
}

/****************************************************************
*FUNCTION NAME:setHttpData
*FUNCTION     :setHttpData
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::setHttpData(void)
{
    uint8_t answer;

    answer =  sim800c.sendAtCommand("AT+HTTPDATA\r\n",200, 0);
    
    debugTerminal("Sim80x_setHttpData");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;
}

/****************************************************************
*FUNCTION NAME:setHttpAction
*FUNCTION     :setHttpAction
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::setHttpAction(void)
{  
    uint8_t answer;

    answer = sim800c.sendAtCommand("AT+HTTPACTION\r\n",200, 0);
    
    debugTerminal("Sim80x_setHttpAction");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;
}

/****************************************************************
*FUNCTION NAME:setHttpRead
*FUNCTION     :setHttpRead
*INPUT        :void
*OUTPUT       :bool
****************************************************************/
bool LIB_SIM800C::setHttpRead(void)
{
  
    uint8_t answer;
    
    answer = sim800c.sendAtCommand("AT+HTTPREAD\r\n",200, 0);
    
    debugTerminal("Sim80x_setHttpRead");

    memset(Sim80x.AtCommand.SendCommand,0,sizeof(Sim80x.AtCommand.SendCommand));

    if(answer == 1)
        return true;
    else
        return false;

}


#endif
#endif
#endif
