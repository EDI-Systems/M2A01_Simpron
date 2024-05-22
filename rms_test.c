/******************************************************************************
Filename    : rms_test.c
Author      : pry
Date        : 04/05/2024
Description : Simple test for RMS. The correct output sequence is:
              Task 1: Waiting for start flag.
              Task 1: Return value is 2.
              Task 2: State 1 -> State 2.
              Task 1: Waiting for start flag.
              Task 1: Return value is 2.
              Task 2: State 2 -> State 1.
              Task 2: State 1 -> State 2.
              Task 1: Waiting for start flag.
              Task 1: Start flag received.
              Task 1: Return value is 1.
              Task 2: State 2 -> State 1.
              Task 2: State 1 -> State 2.
              Task 1: State 1 -> State 2.
              Task 1: State 2 -> State 1.
              Task 1: Return value is 0.
              Task 2: State 2 -> State 1.
              Task 2: State 1 -> State 2.
              Task 1: State 1 -> State 2.
              Task 1: State 2 -> State 1.
              ...
******************************************************************************/

/* Include *******************************************************************/
/* Include all your MCU related header here */
#include "stdio.h"
#include "time.h"
#include "rms.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Define all your task states here */
#define TASK1_CATCH         (0U)
#define TASK1_STATE1        (1U)
#define TASK1_STATE2        (2U)

#define TASK2_STATE1        (0U)
#define TASK2_STATE2        (1U)
/* End Define ****************************************************************/

/* Global Variable ***********************************************************/
/* Define all your flags here */
int Task1_Start=0;
/* End Global Variable *******************************************************/

/* Begin Function:Delay *******************************************************
Description : Delay for multiple seconds.
Input       : int Sec - Seconds to delay.
Output      : None.
Return      : None.
******************************************************************************/
void Delay(int Sec)
{
    clock_t Start;
    clock_t Time;
    
    Start=clock();
    Time=(clock_t)Sec*CLOCKS_PER_SEC;
    while((clock()-Start)<Time);
}
/* End Function:Delay ********************************************************/

/* Begin Function:Task1 *******************************************************
Description : The test task 1.
Input       : None.
Output      : None.
Return      : int - The current exit state.
******************************************************************************/
int Task1(void)
{
    RMS_BEGIN(char,TASK1_CATCH);

    /* User program */
    RMS_EXECUTE(TASK1_STATE1):
    {
        printf("Task 1: State 1 -> State 2.\n");
        fflush(stdout);
        Delay(1);
        RMS_STATE(TASK1_STATE2);
        RMS_CONTINUE();
    }

    RMS_EXECUTE(TASK1_STATE2):
    {
        printf("Task 1: State 2 -> State 1.\n");
        fflush(stdout);
        Delay(1);
        RMS_STATE(TASK1_STATE1);
        RMS_YIELD_RV(0);
    }

    RMS_CATCH():
    {
        printf("Task 1: Waiting for start flag.\n");
        fflush(stdout);
        Delay(1);
        RMS_WAIT_RV(Task1_Start!=0,2);
        printf("Task 1: Start flag received.\n");
        RMS_STATE(TASK1_STATE1);
        RMS_YIELD_RV(1);
    }

    RMS_END();
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
    RMS_BEGIN(char,TASK2_STATE1);
    
    /* User program */
    RMS_EXECUTE(TASK2_STATE1):
    {
        printf("Task 2: State 1 -> State 2.\n");
        fflush(stdout);
        Delay(1);
        RMS_STATE(TASK2_STATE2);
        RMS_YIELD();
    }

    RMS_EXECUTE(TASK2_STATE2):
    {
        printf("Task 2: State 2 -> State 1.\n");
        fflush(stdout);
        Delay(1);
        Task1_Start=1;
        RMS_STATE(TASK2_STATE1);
        RMS_CONTINUE();
    }

    RMS_CATCH():
    {
        RMS_STATE(TASK2_STATE1);
        RMS_YIELD();
    }

    RMS_END();
}
/* End Function:Task2 ********************************************************/

/* Begin Function:main ********************************************************
Description : Entry of the static table-driven scheduler.
Input       : None.
Output      : None.
Return      : int-dummy.
******************************************************************************/
int main(void)
{
    int Retval;
    
    /* Static table-driven scheduler */
    while(1)
    {
        Retval=Task1();
        printf("Task 1: Return value is %d.\n",Retval);
        fflush(stdout);
        Task2();
    }

    /* Never return */
    return(0);
}
/* End Function:main *********************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
