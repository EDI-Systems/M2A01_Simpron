<h1 align="center">
	<img width="300" src="/logo.png" alt="logo">
</h1>

# RMS Feather-weight State Machine

点击 **[这里](README_CN.md)** 查看中文版。

&ensp;&ensp;**RMS** is a feather-weight state machine library for budget 4- and 8-bit microcontrollers.
- Multi-project proven OS for budget 4- and 8-bit MCUs
- Tolerates any esoteric architecture with a C compiler
- No separate stacks for different coroutines required
- Completely free from assembly coding or porting
- Extreme simplicity with near zero memory overheads
- Explicitly exposed task states force formal thinking

## How to use this RMS "kernel"?
&ensp;&ensp;In RMS, all tasks are `void task(void)` or `int task(void)`, and they share __a single thread stack__. Multiple states exist for each task. Upon entry of each task, RMS jumps to the point correspoding to the current state and starts execution. When writing the task function, define the states of it, and make sure the state machine body is enclosed by `RMS_BEGIN(...);` and `RMS_END();`, while using `RMS_EXECUTE():{...}` for execution of each state.

&ensp;&ensp;Call `RMS_YIELD();` when a yield to other tasks is desired, and call `RMS_CONTINUE();` to execute the current task for another round. `RMS_YIELD();` or `RMS_CONTINUE();` must be called before the end of any task execution or an error would occur. RMS also provides `RMS_CATCH():{...}` to catch wild or invalid states, and  `RMS_WAIT(...);` to wait for a certain condition.

&ensp;&ensp;When the task is `int task(void)` instead of `void task(void)`, `RMS_YIELD_RV(...);` must be used instead of `RMS_YIELD();`, and `RMS_WAIT_RV(...,...);` must be used instead of `RMS_WAIT(...);`.

## Possible programming styles with RMS?
&ensp;&ensp;With this set of macros, the programmer is free to write the application in any style, including __event-triggered scheduling__, __priority-driven scheduling__, or just __table-driven scheduling__. All tasks executions are run-to-completion hence preemptions within a single thread are impossible; however, it is possible to write the __interrupt handlers__ as a task (or even a thread hosting multiple tasks) to circumvent this limitation, and multiple preemption priorities may be reached through hardware interrupt nesting.

&ensp;&ensp;For interrupt activation detections, it is recommended to reserve a flag for each of them, and detect if this flag is set in the appropriate task execution. If a flag being waited on is not set, the task can yield immediately to allow execution of other tasks; if set, then the task can clear it and proceed to interrupt processing. Delays that do not spin (but still poll the flags less frequently) can be achieved in this way.

&ensp;&ensp;Another limitation is that the task yielding is only possible within the task function but not their subfunctions. This limitation is due to the fact that all tasks share the same stack, and is somewhat lesser of a problem because all tasks in low-end systems are fairly simple.

## Why not a more complete, "bloated" framework?
&ensp;&ensp;In low-end microcontroller programming, every single bit count. You typically run out of RAM first, then Flash, and finally CPU, as opposed to higher-end platforms where CPU is the bottleneck. RAM typically depends on how many variables you declare thus can be easily controlled. __Flash however, is different__; the endless bug fixes and new features will chew through reserves slowly but stadily. If you adopt a bloated framework from the beginning and find yourself out of Flash for bug fixing towards the end, you're probably beyond salvation. The same goes for timers, etc, as 8051 only have two timers; if the system monopolizes one then you only have a single one left. Taking these into account, the RMS is designed without mandating many features, while being flexible enough to allow the programmer to implement them in a simple, __project-specific__, and __memory-efficient__ way.

## How's this not a copycat of Protothreads?
&ensp;&ensp;Despite the technical similarities, RMS choose to __expose the internal states of the tasks__, forcing the programmer to think __formally__ about the system and separate different states into __individual executions__. This is very useful when designing industrial products that require strict certification, where you need to translate a predefined state transition diagram into code. In addition, this design (1) doesn't suffer from surprise conflicts with the possible switch-cases inside each execution, (2) doesn't need compiler-specific expensions, and (3) only needs one byte of RAM as opposed to two for each task.

## Quick Demo
### Just copy and past this into a C file and compile with any C compiler
```C
#include "stdio.h"
#include "time.h"
#include "rms.h"

#define TASK1_CATCH         (0U)
#define TASK1_STATE1        (1U)
#define TASK1_STATE2        (2U)

#define TASK2_STATE1        (0U)
#define TASK2_STATE2        (1U)

int Task1_Start=0;

void Delay(int Sec)
{
    clock_t Start;
    clock_t Time;
    
    Start=clock();
    Time=(clock_t)Sec*CLOCKS_PER_SEC;
    while((clock()-Start)<Time);
}

int Task1(void)
{
    RMS_BEGIN(TASK1_CATCH);

    /* User program */
    RMS_EXECUTE(TASK1_STATE1):
    {
        printf("Task 1: State 1 -> State 2.\n");
        fflush(stdout);
        Delay(1);
        RMS_STATE=TASK1_STATE2;
        RMS_CONTINUE();
    }

    RMS_EXECUTE(TASK1_STATE2):
    {
        printf("Task 1: State 2 -> State 1.\n");
        fflush(stdout);
        Delay(1);
        RMS_STATE=TASK1_STATE1;
        RMS_YIELD_RV(0);
    }

    RMS_CATCH():
    {
        printf("Task 1: Waiting for start flag.\n");
        fflush(stdout);
        Delay(1);
        RMS_WAIT_RV(Task1_Start!=0,2);
        printf("Task 1: Start flag received.\n");
        RMS_STATE=TASK1_STATE1;
        RMS_YIELD_RV(1);
    }

    RMS_END();
}

void Task2(void)
{
    RMS_BEGIN(TASK2_STATE1);
    
    /* User program */
    RMS_EXECUTE(TASK2_STATE1):
    {
        printf("Task 2: State 1 -> State 2.\n");
        fflush(stdout);
        Delay(1);
        RMS_STATE=TASK2_STATE2;
        RMS_YIELD();
    }

    RMS_EXECUTE(TASK2_STATE2):
    {
        printf("Task 2: State 2 -> State 1.\n");
        fflush(stdout);
        Delay(1);
        Task1_Start=1;
        RMS_STATE=TASK2_STATE1;
        RMS_CONTINUE();
    }

    RMS_CATCH():
    {
        RMS_STATE=TASK2_STATE1;
        RMS_YIELD();
    }

    RMS_END();
}

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
```

## Getting Started

&ensp;&ensp;Include the `rms.h` into your C file and you're done. This project does not have documents associated with it; it should be self-explanatory. No porting needed as long as you have a C compiler. Use `gcc rms_test.c` to compile the example, and the correct output sequence should be:
```
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
```

&ensp;&ensp;Keep in mind that this is designed for 4- and 8-bit MCUs, not for high-performance MCUs or application processors, though it is possible to run such "RTOS" on high-end processors as well. However, doing so will be a huge wastage of their potentials. For 16-or 32-bit MCUs, consider [M5P01_Prokaron](https://github.com/EDI-Systems/M5P01_Prokaron) __Real-Time Minikernel__ instead; for high-end 32-bit MCUs or application processors, consider [M7M01_Eukaron](https://github.com/EDI-Systems/M7M01_Eukaron) __Real-Time Multi-Core Microkernel__ instead.

## EDI Project Information
- M2A01 R4T1
