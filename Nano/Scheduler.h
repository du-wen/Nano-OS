/**
  ******************************************************************************
  * @file    Scheduler.h
  * @author  no1wudi
  * @version V0.0.0
  * @date    09-April-2015
  * @brief   Header file of sheduler.
  ******************************************************************************
  */
  
  
#ifndef __SCHEDULER_H
#define __SCHEDULER_H

/* Includes ------------------------------------------------------------------*/
#include "OSConfig.h"
#include "Core.h"

/* Exported functions --------------------------------------------------------*/

extern bool Nano_SchedulerBuildTable(Nano_TaskControlBlock * Header);
extern bool Nano_SchedulerDetermine(Nano_TaskControlBlock ** Current,Nano_TaskControlBlock ** Next);


#endif
