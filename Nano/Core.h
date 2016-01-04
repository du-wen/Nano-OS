/**
  ******************************************************************************
  * @file    Core.h
  * @author  no1wudi
  * @version V0.0.0
  * @date    07-April-2015
  * @brief   Header file of sheduler.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __CORE_H
#define __CORE_H

/* Includes ------------------------------------------------------------------*/
#include "OSConfig.h"
#include "Portable.h"

/* Exported types ------------------------------------------------------------*/

enum
{
    Nano_TaskIdle = 0,
    Nano_TaskReady,
    Nano_TaskRunning,
    Nano_TaskSuspend,
    Nano_TaskDeleted,
};

typedef struct Nano_TaskControlBlock
{
    Nano_STK OS_TaskStack;
    uint32_t OS_TickRemain;
    uint32_t OS_TaskStackSize;
    uint8_t OS_TaskStatus;
    uint32_t OS_TaskPriority;
    struct Nano_TaskControlBlock * Next;
} Nano_TaskControlBlock;


/* Exported variables --------------------------------------------------------*/

extern Nano_TaskControlBlock * Nano_TaskControlBlockCurrent,*Nano_TaskControlBlockNext;

/* Exported functions --------------------------------------------------------*/

extern bool Nano_Init(void);
extern bool Nano_CreateTask(void (*Task)(void),Nano_STK * Stack,uint32_t StackSize,uint32_t Priority); 
extern void Nano_Delay(uint32_t Ticks);

#endif
