/******************************************************************************
Filename    : rms.h
Author      : pry
Date        : 02/05/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The "kernel" of the RMS. This is a set of macros for efficient
              implementation of simple yet self-explaining stackless state 
              machines tasks that could be used on almost all microcontrollers
              no matter how esoteric (hardware stacks, local variable overlays,
              function reentrancy limitations) they might be. This is most 
              useful for cheap 8- or even 4-bit devices that cannot even run
              RMP, and is proven in a dozen of commercialized projects to be 
              the best practise particularly towards memory efficiency. This
              could also be used to write a light-weight state machine thread
              in a larger, threaded system (i.e. a thread in RMP).
              -----------------------------------------------------------------
              How to use this RMS "kernel"?
              In RMS, all tasks are "void task(void)" or "int task(void)", and
              they share a single thread stack. Multiple states exist for each
              task. Upon entry of each task, RMS jumps to the point correspoding
              to the current state and starts execution.
              When writing the task function, define the states of it, and make
              sure the state machine body is enclosed by "RMS_BEGIN(...);" and 
              "RMS_END();", while using "RMS_EXECUTE():{...}" for execution
              of each state.
              Call "RMS_YIELD();" when a yield to other tasks is desired, and
              call "RMS_CONTINUE();" to execute the current task for another
              round. "RMS_YIELD();" or "RMS_CONTINUE();" must be called before
              the end of any task execution or an error would occur. RMS also
              provides "RMS_CATCH():{...}" to catch wild or invalid states, and 
              "RMS_WAIT(...);" to wait for a certain condition.
              When the task is "int task(void)" instead of "void task(void)",
              "RMS_YIELD_RV(...);" must be used instead of "RMS_YIELD();", and
              "RMS_WAIT_RV(...,...);" must be used instead of "RMS_WAIT(...);".
              -----------------------------------------------------------------
              Possible programming styles with RMS?
              With this set of macros, the programmer is free to write the
              application in any style, including event-triggered scheduling,
              priority-driven scheduling, or just table-driven scheduling. 
              All tasks executions are run-to-completion hence preemptions
              within a single thread are impossible; however, it is possible
              to write the interrupt handlers as a task (or even a thread
              hosting multiple tasks) to circumvent this limitation, and
              multiple preemption priorities may be reached through hardware
              interrupt nesting. 
              For interrupt activation detections, it is recommended to reserve
              a flag for each of them, and detect if this flag is set in the
              appropriate task execution. If a flag being waited on is not set,
              the task can yield immediately to allow execution of other tasks;
              if set, then the task can clear it and proceed to interrupt
              processing. Delays that do not spin (but still poll the flags
              less frequently) can be achieved in this way.
              Another limitation is that the task yielding is only possible
              within the task function but not their subfunctions. This 
              limitation is due to the fact that all tasks share the same
              stack, and is somewhat lesser of a problem because all tasks in
              low-end systems are fairly simple.
              -----------------------------------------------------------------
              Why not a more complete, "bloated" framework?
              In low-end microcontroller programming, every single bit count.
              You typically run out of RAM first, then Flash, and finally CPU,
              as opposed to higher-end platforms where CPU is the bottleneck.
              RAM typically depends on how many variables you declare thus can
              be easily controlled. Flash however, is different; the endless
              bug fixes and new features will chew through reserves slowly but
              stadily. If you adopt a bloated framework from the beginning and
              find yourself out of Flash for bug fixing towards the end, you're
              probably beyond salvation. The same goes for timers, etc, as 8051
              only have two timers; if the system monopolizes one then you only
              have a single one left. Taking these into account, the RMS is 
              designed without mandating many features, while being flexible
              enough to allow the programmer to implement them in a simple, 
              project-specific, and memory-efficient way.
              -----------------------------------------------------------------
              How's this not a copycat of Protothreads?
              Despite the technical similarities, RMS choose to expose the
              internal states of the tasks, forcing the programmer to think
              formally about the system and separate different states into 
              individual executions. This is very useful when designing 
              industrial products that require strict certification, where you
              need to translate a predefined state transition diagram into 
              code. In addition, this design (1) doesn't suffer from surprise 
              conflicts with the possible switch-cases inside each execution,
              (2) doesn't need compiler-specific expensions, and (3) only needs
              one byte of RAM as opposed to two for each task.
******************************************************************************/

/* Define ********************************************************************/
/* Basic structure  - self-explanatory */
#define RMS_BEGIN(T,X)      static T __RMS_STATE__=(X); \
                            __RMS_LABEL__: \
                            switch(__RMS_STATE__){
#define RMS_EXECUTE(X)      case(X)
#define RMS_CATCH()         default
#define RMS_STATE(S)        (__RMS_STATE__=(S))
#define RMS_YIELD()         return
#define RMS_YIELD_RV(V)     return(V)
#define RMS_END()           }

/* Helper macros - self-explanatory */
#define RMS_CONTINUE()      goto __RMS_LABEL__
#define RMS_WAIT(X) \
do \
{ \
    if(!(X)) \
        RMS_YIELD(); \
} \
while(0)
#define RMS_WAIT_RV(X,V) \
do \
{ \
    if(!(X)) \
        RMS_YIELD_RV(V); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
