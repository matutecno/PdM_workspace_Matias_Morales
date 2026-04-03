STM32 University Projects - Coursework Solutions
This repository contains solutions for the exercises proposed in my Embedded Systems courses.

⚠️ Important: Library Dependency
These projects rely on shared drivers stored in the Personal_libs folder. If a project does not compile, you must link this folder to your environment:

Right-click on the project -> Select "Properties".

Go to C/C++ General -> Paths and Symbols.

Select the "Source Location" tab.

Click on "Link Folder...".

Folder name: Enter Personal_libs.

Check the box "Link to folder in the file system".

Click "Browse..." and select your local Personal_libs directory.

Click OK -> Apply and Close.



📂 How to Add New Libraries to the Project
If you add a new module (like Delay or JSNSR04T) to the Personal_libs folder, follow these steps to ensure the IDE and the Compiler (GCC) recognize them:

1. Refresh the Project Explorer
After moving or creating files in your Linux file system (ThinkPad), the IDE needs to sync:

Right-click on the Personal_libs folder in the Project Explorer.

Select Refresh (or simply press F5). The new Inc and Src folders should now be visible.

2. Configure Include Paths (CRITICAL)
Even if the files are visible in the explorer, the compiler won't find the .h files unless you explicitly tell it where to look:

Right-click on the project name -> Properties.

Navigate to C/C++ Build -> Settings.

Under the Tool Settings tab, go to: MCU GCC Compiler -> Include paths.

Click the Add... icon (the page with the + symbol).

Click on Workspace..., navigate to the specific Inc folder (e.g., Personal_libs/Delay/Inc), and click OK.

Click Apply and Close.

Note: You must repeat step 2 for every new library module you create to avoid "No such file or directory" errors during compilation.





Project Directory

📂 Excercise 1
LED Blink: Basic implementation using External Interrupts (EXTI) to handle push-button inputs, avoiding CPU blocking.



📂 Excercise 2
Non-blocking Delays: Timing implementation using the HAL_GetTick() method to allow multitasking.

Bonus Exercise: Found in the folder ending in _3.

Custom Driver: I developed a modular library (del.c and del.h) to encapsulate delay logic for future projects.

Theory: Check Notes.txt within the project folder for the technical Q&A.


📂 Exercise 3
No comments

📂 Exercise 4
Finite machine states



📂 Exercise 5

Part 1: Polling UART Driver

**Platform:** Nucleo-F446RE (STM32F446RE, 84 MHz)
**IDE:** STM32CubeIDE
**Course:** Microcontroller Programming — FIUBA

## Description

Implementation of a UART driver over USART2 in polling mode, without DMA or interrupts. The driver encapsulates the HAL handle and exposes a high-level API for sending and receiving strings.

## Serial Port Configuration

| Parameter       | Value     |
|-----------------|-----------|
| Instance        | USART2    |
| Baud rate       | 9600      |
| Data bits       | 8         |
| Parity          | Odd       |
| Stop bits       | 1         |
| Flow control    | None      |
| Oversampling    | x16       |
| Pins            | PA2 (TX), PA3 (RX) |

> The serial terminal must be configured as **9600 8-Odd-1**.

## Driver Files

| File | Description |
|------|-------------|
| `STM32CubeIDE/Drivers/API_uart.c/Src/API_uart.c` | Driver implementation |
| `STM32CubeIDE/Drivers/API_uart.c/Inc/API_uart.h` | Public interface |

## API

```c
bool uartInit(void);
void uartSendString(uint8_t *pstring);
void uartSendStringSize(uint8_t *pstring, uint16_t size);
void uartReceiveStringSize(uint8_t *pstring, uint16_t size);
```

- `uartInit`: initializes USART2 and prints the port configuration to the console. Returns `1` on success, `0` on error.
- `uartSendString`: transmits a null-terminated string.
- `uartSendStringSize`: transmits exactly `size` bytes, one byte at a time.
- `uartReceiveStringSize`: receives bytes until `\n` or until `size` bytes are read. Blocks until reception is complete.

## Main Behavior

On startup, `uartInit` prints the port configuration. The main loop then:
1. Sends `buffer` over UART
2. Waits 500 ms
3. Sends `buffer2` over UART
4. Waits 500 ms
5. Sends `buffer3` over UART and waits to receive a string from the terminal
6. The received string is stored in `buffer3` for the next cycle

## Clock

- Source: HSE 8 MHz
- PLL: PLLM=8, PLLN=336, PLLP=DIV4 → SYSCLK = 84 MHz
- APB1 = 42 MHz (USART2 clock source)
- Voltage scaling: SCALE2
