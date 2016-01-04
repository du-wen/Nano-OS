/**
  ******************************************************************************
  * @file    Protable.h
  * @author  no1wudi
  * @version V0.0.0
  * @date    07-April-2015
  * @brief   Header of portable source code.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __PORTABLE_H
#define __PORTABLE_H

/* Includes ------------------------------------------------------------------*/
#include "OSConfig.h"
#include "core.h"
#include "stm32f10x.h"      //Provide NVIC interrupt channel defination
#include "core_cm3.h"

/* Exported functions --------------------------------------------------------*/

#define Nano_SystemEnterCritical() __disable_irq()
#define Nano_SystemExitCritical() __enable_irq()
#define Nano_SystemCallSwitchContext()  (*((uint32_t *)0xE000ED04) = 0x10000000)

#endif
