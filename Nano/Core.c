/**
  ******************************************************************************
  * @file    Core.c
  * @author  no1wudi
  * @version V0.0.0
  * @date    07-April-2015
  * @brief   Detail of sheduler.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Core.h"
#include "Scheduler.h"

/* Exported variables --------------------------------------------------------*/
Nano_TaskControlBlock * Nano_TaskControlBlockCurrent = NULL, * Nano_TaskControlBlockNext = NULL, * Head = NULL;

/* Private functions ---------------------------------------------------------*/

static void Nano_CoreIdleTask(void);
static void Nano_CoreTaskNormal(void);
static void Nano_CoreTaskInit(void);
static __ASM void Nano_CoreChangSP(void);


/* Private variables ---------------------------------------------------------*/

static Nano_STK Nano_CoreIdleStack[Nano_IdleTaskStackSize] = { 0 };
static void (*Nano_CoreProcess)(void)  = Nano_CoreTaskInit;

/* Exported functions ---------------------------------------------------------*/

bool Nano_Init(void)
{
    Nano_CreateTask(Nano_CoreIdleTask,Nano_CoreIdleStack,sizeof(Nano_CoreIdleStack),0xFFFFFFFF);
    NVIC_SetPriority(PendSV_IRQn, 0xffff);
    return true;
}

bool Nano_CreateTask(void (*Task)(void), Nano_STK * Stack, uint32_t StackSize, uint32_t Priority)
{
    static Nano_TaskControlBlock * Tail = NULL;
    
    Nano_SystemEnterCritical();
    
    Stack = (Nano_STK *)((uint32_t)Stack + StackSize);

    if(NULL == Head)
    {
        Head = (Nano_TaskControlBlock *)malloc(sizeof(Nano_TaskControlBlock));
        Tail = Head;
    }
    else
    {
        Tail->Next = (Nano_TaskControlBlock *)malloc(sizeof(Nano_TaskControlBlock));
        if(NULL == Tail->Next)
        {
            return false;
        }
        Tail = Tail->Next;
    }

    Tail->OS_TaskStackSize = StackSize;
    Tail->OS_TaskStatus = Nano_TaskReady;
    Tail->OS_TickRemain = 0;
    Tail->OS_TaskPriority = Priority;
    Tail->Next = NULL;


    *(--Stack) = (Nano_STK)0x01000000;      //xPSR
    *(--Stack) = (Nano_STK)Task;            //PC
    *(--Stack) = (Nano_STK)NULL;            //LR
    *(--Stack) = (Nano_STK)0x12;            //R12
    *(--Stack) = (Nano_STK)0x3;             //R3
    *(--Stack) = (Nano_STK)0x2;             //R2
    *(--Stack) = (Nano_STK)0x1;             //R1
    *(--Stack) = (Nano_STK)0x0;             //R0

    *(--Stack) = (Nano_STK)0x11;             //R11
    *(--Stack) = (Nano_STK)0x10;             //R10
    *(--Stack) = (Nano_STK)0x9;              //R9
    *(--Stack) = (Nano_STK)0x8;              //R8
    *(--Stack) = (Nano_STK)0x7;              //R7
    *(--Stack) = (Nano_STK)0x6;              //R6
    *(--Stack) = (Nano_STK)0x5;              //R5
    *(--Stack) = (Nano_STK)0x4;              //R4



    Tail->OS_TaskStack = (Nano_STK )(Stack);
    
    Nano_SystemExitCritical();
    
    return true;
}


/* Private functions ---------------------------------------------------------*/

static void Nano_CoreIdleTask(void)
{
    for(;;);
}


static __ASM void Nano_CoreChangSP(void)
{
    MRS R0, PSP;
    MRS R1, MSP;

    MSR MSP, R0;                //Set MSP as PSP to pop task stack in handler mode
    
    POP {R4 - R11};

    LDR R2, = __cpp(&Head);
    LDR R2, [R2];

    MRS R3, MSP;
    STR R3, [R2];               //Save the adjusted stack

    MRS R0, MSP;
    MSR PSP, R0;
    
    MSR MSP, R1;                //Restore msp
    LDR LR, = 0xFFFFFFFD;       //Return to thread mode
    BX LR;
}

static void Nano_CoreTaskInit(void)
{
    
    Nano_CoreProcess = Nano_CoreTaskNormal;
    Nano_SchedulerBuildTable(Head);
    Nano_TaskControlBlockCurrent = Head;
    Nano_TaskControlBlockNext = NULL;
    __set_PSP(Head->OS_TaskStack);
    __set_CONTROL(0x2);
    Nano_CoreChangSP();
}

static void Nano_CoreTaskNormal(void)
{
    
    if(Nano_SchedulerDetermine(&Nano_TaskControlBlockCurrent,&Nano_TaskControlBlockNext))
    {
        Nano_SystemCallSwitchContext();
    }
    
}

/* Exported functions ---------------------------------------------------------*/

void SysTick_Handler(void)
{
    Nano_CoreProcess();
}

void Nano_Delay(uint32_t Ticks)
{
    Nano_SystemEnterCritical();
    
    Nano_TaskControlBlockCurrent->OS_TickRemain = Ticks;
    Nano_TaskControlBlockCurrent->OS_TaskStatus = Nano_TaskSuspend;
    
    Nano_SystemExitCritical();
}
