# Proto Machine (RMS)

Click **[HERE](README.md)** for English version.

&emsp;&emsp;**RMS** 是一个超级简单的基于状态机的协程库。它是为4位和8位微控制器设计的。
- 构成控制流由用宏包裹的C语言构成
- 在4位和8位微控制器上实用
- 含有少于50行代码，接近0的协程切换开销
- 完全无需编写汇编或者移植
- 可提升系统的灵活性和功能
- 无需为每个协程准备一个单独的栈
- 无需单独的定时器资源

&emsp;&emsp;本软件以MIT协议授权。

## 快速演示
### 常规协程
```C
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
```

### 中断协程
```C
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
```

## 新手上路

&emsp;&emsp;拷贝本文件到你的工程文件夹中，并使用它作为主文件即可。只要你有一个C编译器，就可以无需移植而使用它。提供的“内核”文件包含了例子程序，它会用printf打印出当前的两个任务的状态。我们提供了一个 _Visual Studio_ 的工程作为示例；如果想要使用gcc，只需要在命令行键入_gcc -o RMS.elf kernel.c_ 即可。在批量产品中使用本系统应该是非常简单的。本系统没有额外的文档，因为它足够简单易懂。  
&emsp;&emsp;在使用时，应当注意本系统是为4位和8位单片机设计的，而不是高性能的单片机或者应用处理器。当然，在高端处理器上运行本软件也是可行的。但是，这样会很浪费高端处理器的额外资源。对于16和32位的微控制器，可以考虑使用 [M5P1_MuProkaron](https://github.com/EDI-Systems/M5P1_MuProkaron) _小型实时系统_ ；对于高端的32位单片机或者应用处理器， [M7M1_MuEukaron](https://github.com/EDI-Systems/M7M1_MuProkaron) _实时多核心微内核_ 可能是一个更好的选择.

## EDI 工程信息
&emsp;&emsp;突越 - 太古 - 原初 (M2A1 R3T1)
