/*
 * LIB_SDMMC.h
 *
 *  Created on: Sep 21, 2015
 *      Author: Engineeridea
 */
#include "stm32l1xx.h"
#include "Settings.h"

#ifndef _USE_SDMMC
  #define _USE_SDMMC
#endif

#if(_USE_SDMMC==1)

#include "ff.h"

extern FATFS xfat;                                            	/* File system object */
extern FRESULT errCode;                                        /* Error code object  */
extern DIR directory;                                         /* Directory object   */
extern FIL fil_log;                                            /* Opened file object - LOG */
extern FIL fil_ex;                                             /* Opened file object - EX */

class SD_MMC    
{
  public :
    void Init(void);
    FRESULT makeVol(void);
    FRESULT makeDir(char const *dir);
    FRESULT format(void);
    FRESULT dirOpen(char const *dir);
    FRESULT filOpen(FIL fil_obj, char *fil_path, char option);
    FRESULT filClose(FIL fil_obj);
    FRESULT filWrite(FIL fil_obj, char *fil_path, char *buffer);
    FRESULT filRead(FIL fil_obj, char *fil_path, char *buffer);
    FRESULT filburstWrite(FIL fil_obj, char const *fil_path, char *buffer);
    FRESULT filburstRead(FIL fil_obj, char const *fil_path, char *buffer);
    void    clr_Buffer(char *buffer, unsigned int len);

    private:
    
};

extern SD_MMC sdmmc;

#endif