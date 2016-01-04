/**
  ******************************************************************************
  * @file    Protable.c
  * @author  no1wudi
  * @version V0.0.0
  * @date    07-April-2015
  * @brief   Detail implemention of object system.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "Portable.h"

__ASM void PendSV_Handler(void)
{
    CPSID I;
    
    LDR R0, = __cpp(&Nano_TaskControlBlockCurrent);
    LDR R0,[R0];                    //Get PSP Pointer
    LDR R1,[R0];                    //Get the PSP
    
    MRS R3,MSP;
    
    MRS R2,PSP;
    MSR MSP,R2;                     //Switch psp to msp
    
    PUSH {R4-R11};
    
    MRS R2,MSP;
    STR R2,[R0];                    //Save the new stack
    
    LDR R0, = __cpp(&Nano_TaskControlBlockNext);
    LDR R0,[R0];                    //Get PSP Pointer
    LDR R1,[R0];                    //Get the PSP
    
    MSR MSP,R1;
    
    POP {R4-R11};
    
    MRS R1,MSP;
    MSR PSP,R1;                     //Switch psp to msp
    
    MSR MSP,R3;
    
    LDR R0, = __cpp(&Nano_TaskControlBlockCurrent);
    LDR R1, = __cpp(&Nano_TaskControlBlockNext);
    
    LDR R1,[R1];
    STR R1,[R0];
    
    CPSIE I;
    BX LR;
}
