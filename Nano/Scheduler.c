/**
  ******************************************************************************
  * @file    Scheduler.c
  * @author  no1wudi
  * @version V0.0.0
  * @date    09-April-2015
  * @brief   Detail of sheduler.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "Scheduler.h"

static Nano_TaskControlBlock * Nano_SchedulerTable[Nano_MaxTaskNumber] = { NULL };

/* Private functions ---------------------------------------------------------*/

static void Nano_SchedulerTaskDelay(void);



static void Nano_SchedulerTaskDelay(void)
{
    uint16_t cnt;
    for(cnt = 0; cnt < Nano_MaxTaskNumber; cnt++)
    {
        if(NULL != Nano_SchedulerTable[cnt])
        {
            if(Nano_SchedulerTable[cnt]->OS_TickRemain)
            {
                Nano_SchedulerTable[cnt]->OS_TickRemain--;
                if(0 == Nano_SchedulerTable[cnt]->OS_TickRemain)
                {
                    Nano_SchedulerTable[cnt]->OS_TaskStatus = Nano_TaskReady;
                }
            }
        }
    }
}

/* Exported functions --------------------------------------------------------*/

bool Nano_SchedulerBuildTable(Nano_TaskControlBlock * Header)
{
    if(NULL == Header)
    {
        return false;
    }

    for(;;)
    {
        if(0xFFFFFFFF == Header->OS_TaskPriority)
        {
            Nano_SchedulerTable[Nano_MaxTaskNumber - 1] = Header;
        }
        else
        {
            if((Nano_MaxTaskNumber) <= Header->OS_TaskPriority)
            {
                return false;
            }

            Nano_SchedulerTable[Header->OS_TaskPriority] = Header;
        }

        if(NULL == Header->Next)
        {
            return true;
        }
        else
        {
            Header = Header->Next;
        }
    }

    return true;
}


bool Nano_SchedulerDetermine(Nano_TaskControlBlock ** Current, Nano_TaskControlBlock ** Next)
{

    Nano_TaskControlBlock * tmp = NULL;
    uint32_t cnt = 0;
    
    Nano_SchedulerTaskDelay();

    for(cnt = 0; cnt < Nano_MaxTaskNumber; cnt++)
    {
        if(NULL != Nano_SchedulerTable[cnt])
        {

            if(NULL == tmp)
            {
                if(Nano_TaskReady == Nano_SchedulerTable[cnt]->OS_TaskStatus)
                    tmp = Nano_SchedulerTable[cnt];
            }
        }
    }

    if(tmp == *Current)
    {
        return false;
    }
    else
    {
        *Next = tmp;
    }

    return true;
}

