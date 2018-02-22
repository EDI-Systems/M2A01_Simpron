/******************************************************************************
Filename    : kernel.c
Author      : pry
Date        : 15/11/2014                
Description : The kernel of the FSM-Simpron proto-RTOS.
              This FSM, or barely an RTOS, has only a single stack, and might be the
              simplest RTOS made ever.

The system scheduler module is one that:
1> Supports 120 number of threads (not processes and pseudo-processes), and the
   implemented number is only limited by the available RAM of the MCU;
2> Support dynamic thread management including deletion and setup
3> Does not require a system timer.
4> DOES NOT support priority.
5> The system also does not support zombie thread and thread return value.
6> When used, all task slots must be filled. An empty task slot will cause
   undefined behavior.
7> Switch-case statement cannot be used in any task function. Use if-else instead.
   
In very tiny places, these features have advantages as follows:
1> Save a system timer, which is especially important in the 8051 or AVR systems where
   the system timer is a rare resource;
2> This OS does not involve stack manipulation, so it can readily run on PIC MCUs.
3> Quick in thread context switching.
4> Makes extremely rapid simple system development possible, especially when the system
   is simple.
******************************************************************************/

/* Includes ******************************************************************/
/* Put all MCU related code here */
#include "iom8.h"
#include "ina90.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Configuration definitions */
#define SYS_TASK_NUM         2

/* System definitions */
#define TASK_STATE           TCB[Cur_TID]
#define SYS_TASK_BEGIN()     while(1)switch(TCB[Cur_TID]){
/* If possible, do not use wait */
/* #define SYS_WAIT_TRUE(X)     case __LINE__:if((X)==0){TCB[Cur_TID]=__LINE__;Cur_TID=(Cur_TID+1)%SYS_TASK_NUM;return;} */
#define SYS_YIELD()          {Cur_TID=(Cur_TID+1)%SYS_TASK_NUM;return;}
#define SYS_TASK_END()       };
                     
#define INT_TASK_BEGIN(X)    switch(X){
#define INT_YIELD()          return;
#define INT_TASK_END()       };
/* End Defines ***************************************************************/

/* Typedefs ******************************************************************/
typedef unsigned char u8;
typedef unsigned int u16;
/* End Typedefs **************************************************************/

/* Global Variables **********************************************************/
u8 Cur_TID;
u16 TCB[SYS_TASK_NUM];
/* End Global Variables ******************************************************/

/* Begin Function:Task1 *******************************************************
Description : The test task 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Task1(void)
{
    /* The program between the header and the end is run repeatedly. 
     * No extra while() is needed.
     */
    SYS_TASK_BEGIN();

    /* User program */

    SYS_YIELD();

    SYS_TASK_END();
}
/* End Function:Task1 ********************************************************/

/* Begin Function:Task2 *******************************************************
Description : The test task 2.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Task2(void)
{
    /* The program between the header and the end is run repeatedly. 
     * No extra while() is needed.
     */
    SYS_TASK_BEGIN();
    
    /* User program */

    SYS_YIELD();

    SYS_TASK_END();
}
/* End Function:Task2 ********************************************************/

/* Begin Function:main ********************************************************
Description : The entrance of the FSM/RTOS.
Input       : None.
Output      : None.
Return      : int-dummy.
******************************************************************************/
int main(void)    	                                                    
{    
    /* Initialize the scheduler */
    for(Cur_TID=0;Cur_TID<SYS_TASK_NUM;Cur_TID++)
        TCB[Cur_TID]=0;
    Cur_TID=0;

    /* Call all your task functions here one by one,sequentially, no more, no less */
    while(1)
    {
        Task1();
        Task2();
    }       
           
    /* Never return */                         	    	    	    	    	    	               
    return(0);  
}
/* End Function:main *********************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) 2011-2013 Evo-Devo Instrum. All rights reserved *************/

