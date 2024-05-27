<h1 align="center">
	<img width="300" src="/logo.png" alt="logo">
</h1>

# RMS 羽量级状态机

Click **[HERE](README.md)** for English version.

&emsp;&emsp;**RMS** 是一个为4位和8位经济型微控制器设计的羽量级状态机协程库。
- 的经多个项目验证的4位和8位机专用系统
- 容忍任何使用C编译器的古怪架构
- 无需为每个协程准备单独的运行栈
- 完全无需编写汇编或者移植代码
- 极度简单并几乎不消耗任何内存
- 显式暴露任务状态以鼓励形式化思考

## 如何使用此RMS“内核”？

&emsp;&emsp;在RMS中，所有任务都是`void task(void)`或`int task(void)`，并且它们共享**单个线程堆栈**。每个任务都存在多个状态。进入每个任务后，RMS跳转到与当前状态对应的位置并开始执行。在编写任务函数时，需定义其状态，并确保状态机主体以`RMS_BEGIN(...);`开始，以`RMS_END();`结束，使用`RMS_EXECUTE():{...}`执行每个状态。

&emsp;&emsp;当需要让出其他任务时，调用`RMS_YIELD();`。执行当前任务的另一个循环时，调用`RMS_CONTINUE();`。必须在任何任务执行结束之前调用`RMS_YIELD();`或`RMS_CONTINUE();`，否则将会发生错误。RMS还提供了`RMS_CATCH():{...}`来捕获异常或无效状态，以及`RMS_WAIT(...);`来等待特定条件。

&emsp;&emsp;当任务为`int task(void)`而不是`void task(void)`时，须使用`RMS_YIELD_RV(...);`代替`RMS_YIELD();`，使用 `RMS_WAIT_RV(...,...);`代替`RMS_WAIT(...);`。

## RMS可能的编程风格？

&emsp;&emsp;通过这组宏，程序员可以自由地以任何风格编写应用程序，包括**事件触发的调度**、**优先级驱动的调度**，或**仅使用表驱动的调度**。所有任务执行都是从运行到完成，因此单个线程内不可能发生抢占。但是，可以将**中断处理程序**编写为任务（甚至是托管多个任务的线程；关于此用法，请参见**2009年的RTSS文章**“*Sloth: Threads as Interrupts*”）来规避此限制，并且通过硬件中断嵌套实现多个抢占优先级。

&emsp;&emsp;对于中断激活检测，建议为每个中断保留一个标志，并在相应的任务执行中检测此标志是否被设置。如果未设置正在等待的标志，任务可以立即让出，以允许执行其他任务；如果设置了，则任务可以清除它并继续进行中断处理。通过这种方式可以实现不旋转（但仍低频率地轮询标志）的延迟。

&emsp;&emsp;另一个限制是任务让出只能在任务函数内部进行，而不能在其子函数中进行。这个限制是因为所有任务共享同一个栈，这在低端系统中，所有任务都相对简单，这个问题就不那么严重。

## 为什么不实现一个更完整、“臃肿”的框架？

&emsp;&emsp;在低端微控制器编程中，每一个bit都很重要。通常首先是RAM用尽，然后是Flash，最后是CPU。这与高端平台不同，在高端平台中CPU通常是瓶颈。RAM通常取决于你声明了多少变量，因此可以很容易控制。然而，Flash则不同，无休止的错误修复和新功能会缓慢而稳定地消耗储备。如果你一开始就采用一个臃肿的框架，然后发现在最后修复错误时Flash空间不足，你可能就无药可救了。定时器等也是同理，如8051只有两个定时器，如果系统垄断了一个，那么你就只剩下一个定时器了。考虑到这些因素，RMS被设计为不强制要求许多功能，同时又足够灵活，允许程序员以一种简单、特定于项目且内存效率高的方式实现这些功能。

## 这难道不是山寨版Protothreads吗？

&emsp;&emsp;尽管技术上有相似之处，RMS选择公开任务的内部状态，迫使程序员**形式化**地思考系统并将不同状态分离到各自的执行中。这在设计需要严格认证的工业产品时非常有用，因为你需要将预定义的状态转换图转化为代码，而这种状态转换图的正确性可以很方便地用UPPAAL等工具验证。此外，这种设计(1)不会受到执行中可能存在的switch-cases的意外冲突的影响，(2)不需要编译器特定的扩展，(3)每个任务最少只需要1字节（在编译器允许的状况下，甚至少于1字节）的RAM，相比之下Protothreads需要两字节或更多。

## 快速演示
### 只需将其复制并粘贴到C文件中，然后使用任何C编译器进行编译
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

## 新手上路

&emsp;&emsp;只需将rms.h包含进你的C文件中即可。这个项目没有相关的文档——它应该是不言自明的。只要你有一个C编译器，就可以无需移植的使用它。使用gcc rms_test.c来编译示例，正确的输出顺序应该是：

```text
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

&emsp;&emsp;在使用时，应当注意本系统是为4位和8位单片机设计的，而不是高性能的单片机或者应用处理器。当然，在高端处理器上运行本软件也是可行的。但是，这样会很浪费高端处理器的额外资源。对于16和32位的微控制器，可以考虑使用 [M5P1_MuProkaron](https://github.com/EDI-Systems/M5P01_Prokaron) _小型实时系统_ ；对于高端的32位单片机或者应用处理器， [M7M1_MuEukaron](https://github.com/EDI-Systems/M7M01_Eukaron) _实时多核心微内核_ 可能是一个更好的选择.

## EDI 工程信息
- M2A01 R4T1
