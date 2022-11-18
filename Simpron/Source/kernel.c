/******************************************************************************
Filename    : kernel.c
Author      : pry
Date        : 15/11/2014                
Description : The kernel of the FSM-Simpron proto-RTOS.
              This FSM, or barely an RTOS, has only a single stack, and might be the
              simplest RTOS made ever, yet still fully preemptive.
              The characteristics of this finite state machine include:
              1> The number of threads is only limited by the available memory;
              2> Does not require a system timer, or even require working interrupts.
              3> Different priorities are implemented with hardware interrupt priorities.
******************************************************************************/

/* Includes ******************************************************************/
/* Include all your MCU related header here */
#include "stdio.h"
#include "time.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Configuration definitions */
#define SYS_TASK_NUM         2

/* System definitions */
#define TASK_STATE           TCB[Cur_TID]
#define SYS_TASK_BEGIN()     while(1)switch(TCB[Cur_TID]){
#define SYS_YIELD()          {Cur_TID=(Cur_TID+1)%SYS_TASK_NUM;return;}
#define SYS_TASK_END()       };
                     
#define INT_TASK_BEGIN(X)    switch(X){
#define INT_YIELD()          return;
#define INT_TASK_END()       };

/* Define all your task states here */
#define TASK1_STATE1         0
#define TASK1_STATE2         1

#define TASK2_STATE1         0
#define TASK2_STATE2         1
/* End Defines ***************************************************************/

/* Typedefs ******************************************************************/
/* Define this according to your compiler settings */
typedef unsigned char u8;
/* End Typedefs **************************************************************/

/* Global Variables **********************************************************/
u8 Cur_TID;
u8 TCB[SYS_TASK_NUM];
/* End Global Variables ******************************************************/

/* User supplied delay functions for demo */
void Delay(int Sec)
{
    clock_t Start;
    clock_t Time;
    
    Start=clock();
    Time=(clock_t)Sec*CLOCKS_PER_SEC;
    while((clock()-Start)<Time);
}

/* Begin Function:Task1 *******************************************************
Description : The test task 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Task1(void)
{
    SYS_TASK_BEGIN();

    /* User program */
    case TASK1_STATE1:
    {
        TASK_STATE=TASK1_STATE2;
        printf("Task 1: State 1 -> State 2\n");
        Delay(1);
        SYS_YIELD();
    }

    case TASK1_STATE2:
    {
        TASK_STATE=TASK1_STATE1;
        printf("Task 1: State 2 -> State 1\n");
        Delay(1);
        SYS_YIELD();
    }

    default:
    {
        SYS_YIELD();
    }

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
    SYS_TASK_BEGIN();
    
    /* User program */
    case TASK2_STATE1:
    {
        TASK_STATE=TASK2_STATE2;
        printf("Task 2: State 1 -> State 2\n");
        Delay(1);
        SYS_YIELD();
    }

    case TASK2_STATE2:
    {
        TASK_STATE=TASK2_STATE1;
        printf("Task 2: State 2 -> State 1\n");
        Delay(1);
        SYS_YIELD();
    }

    default:
    {
        SYS_YIELD();
    }

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

    /* Call all your task functions here one by one, sequentially, no more, no less */
    while(1)
    {
        Task1();
        Task2();
    }

    /* Never return */
    return(0);

/* For interrupt tasks, call your tasks sequentially in the interrupt handler, and
   declare a variable to store its state.The interrupt handler which contains two
   tasks that respectively switch toggle their own states should look like:

#define INT1_STATE_1         0
#define INT1_STATE_2         1

#define INT2_STATE_1         0
#define INT2_STATE_2         1

u8 Int_State_1=0;
u8 Int_State_2=0;

void Int_Task_1(void)
{
    INT_TASK_BEGIN(Int_State_1);

    case INT1_STATE_1:
    {
        Int_State_1=INT1_STATE_2;
        INT_YIELD();
    }

    case INT1_STATE_2:
    {
        Int_State_1=INT1_STATE_1;
        INT_YIELD();
    }

    INT_TASK_END();
}

void Int_Task_2(void)
{
    INT_TASK_BEGIN(Int_State_2);

    case INT2_STATE_1:
    {
        Int_State_2=INT2_STATE_2;
        INT_YIELD();
    }

    case INT2_STATE_2:
    {
        Int_State_2=INT2_STATE_1;
        INT_YIELD();
    }

    INT_TASK_END();
}

void Handler(void)
{
    Int_Task_1();
    Int_Task_2();
}
*/
}
/* End Function:main *********************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

