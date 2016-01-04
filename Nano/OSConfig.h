/**
  ******************************************************************************
  * @file    OSConfig.h
  * @author  no1wudi
  * @version V0.0.0
  * @date    07-April-2015
  * @brief   Header file of Configuration.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __OSCONFIG_H
#define __OSCONFIG_H

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Exported macro ---------------------------------------------------------*/

#define Nano_STK                uint32_t
#define Nano_MaxTaskNumber      2
#define Nano_IdleTaskStackSize  32

#endif
