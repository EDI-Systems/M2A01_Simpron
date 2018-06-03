# Proto Machine (RMS)

点击 **[这里](README_CN.md)** 查看中文版。

**RMS** is a super-simple state machine-based coroutine library targeting 4- and 8-bit microcontrollers.
- Macro-packaged C constructs as the body of the control flow
- Practically useful OS for 4- and 8-bit MCUs
- Contains only 50 lines of code and have almost zero context switch overhead
- Completely free from assembly coding or porting
- Boost system flexibility and functionality
- No separate stacks for different coroutines required
- No system tick timer required

This software is licensed under the MIT license.

## Quick Demo
### Normal coroutines
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

### Interrupt coroutines 
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

## Getting Started

Copy the single file into your project and use it as the main file. No porting needed as long as you have a C compiler. The "kernel" file supplied contains the example program itself, and it will use printf to output the current state of the two example tasks. A _Visual Studio_ project is provided for illustration purposes; if you want to use gcc, simply type _gcc -o RMS.elf kernel.c_ in shell. Deploying this into mass-produced products should be very easy. This product does not have documents associated with it; it should be self-illustrating.
Keep in mind that this is designed for 4- and 8-bit MCUs, not for high-performance MCUs or application processors, though it is possible to run such "RTOS" on high-end processors as well. However, doing so will be a huge wastage of their resources. For 16-or 32-bit MCUs, consider [M5P1_MuProkaron](https://github.com/EDI-Systems/M5P1_MuProkaron) _Real-Time Minikernel_ instead; for high-end 32-bit MCUs or application processors, consider [M7M1_MuEukaron](https://github.com/EDI-Systems/M7M1_MuProkaron) _Real-Time Multi-Core Microkernel_ instead.

## EDI Project Information
Mutate - Archaeo - Simpron (M2A1 R3T1)
