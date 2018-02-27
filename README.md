# MuSimpron

**RMS** is a super-simple state machine-based coroutine library targeting 4- and 8-bit microcontrollers.
- Macro-packaged C constructs as the body of the control flow
- Practically useful OS for 4- and 8-bit MCUs
- Contains only 50 lines of code and have almost zero overhead
- Completely free from assembly coding or porting
- Boost system flexibility and functionality
- Do not require separate stacks for different coroutines

This software is licensed under the MIT license.

## Quick Demo
### Normal coroutines
```C
 
```

### Interrupt coroutines 
```C

```

## Getting Started

Copy the single file into your project and use it as the main file. No porting needed as long as you have a C compiler. The "kernel"  file supplied contains the example program itself, and it will use two LEDs to show the program execution if unmodified. Deploying this into mass-produced products should be very easy. This product does not have documents associated with it; it should be self-illustrating.
Keep in mind that this is designed for 4- and 8-bit MCUs, not for high-performance MCUs or application processors, though it is possible to run such "RTOS" on high-end processors as well. However, doing so will be a huge wastage of their resources. For 16-or 32-bit MCUs, consider [M5P1_MuProkaron](https://github.com/EDI-Systems/M5P1_MuProkaron) _Real-Time Minikernel_ instead; for high-end 32-bit MCUs or application processors, consider [M7M1_MuEukaron](https://github.com/EDI-Systems/M7M1_MuProkaron) _Real-Time Multi-Core Microkernel_ instead.

## EDI Project Information
Mutate - Archaeo - Simpron (M2A1 R3T1)
