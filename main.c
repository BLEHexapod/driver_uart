/* 
 * File:   main.c
 * Author: bart452
 *
 * Created on 15 May 2016, 12:14
 */

/* -------------------------------------------------------------------------- */
/* pragma's for configuring the Cerebot board.                                */
/* -------------------------------------------------------------------------- */
#pragma config ICESEL   = ICS_PGx2		// ICE/ICD Comm Channel Select
#pragma config BWP      = OFF			// Boot Flash Write Protect
#pragma config CP       = OFF			// Code Protect
#pragma config IESO     = OFF			// Internal/External Switch-over
#pragma config OSCIOFNC = OFF			// CLKO Enable
#pragma config FCKSM    = CSDCMD		// Clock Switching & Fail Safe Clock Monitor
#pragma config WDTPS    = PS1			// Watchdog Timer Postscale
#pragma config FWDTEN   = OFF			// Watchdog Timer 
#pragma config UPLLIDIV = DIV_2			// USB PLL Input Divider
#pragma config UPLLEN   = OFF			// USB PLL Enabled
#pragma config PWP      = OFF			// Program Flash Write Protect
#pragma config DEBUG    = OFF			// Debugger Enable/Disable
    
#pragma config FNOSC    = PRIPLL        // Oscillator selection
#pragma config POSCMOD  = HS            // Primary oscillator mode
#pragma config FPLLIDIV = DIV_2         // PLL input divider
#pragma config FPLLMUL  = MUL_20        // PLL multiplier
#pragma config FPLLODIV = DIV_1         // PLL output divider
#pragma config FPBDIV   = DIV_2         // Peripheral bus clock divider
#pragma config FSOSCEN  = OFF           // Secondary oscillator enable

#include "freertos/FreeRTOS.h"
#include "os_thread.h"
#include "drv_uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static drv_uartHandle_t uartHandle;

static void uartHandler(void* args, uint8_t size)
{
    
}

static void uartThread(void *args)
{
    drv_uartConfig_t uartConf = {
        .baud = BAUD9600,
        .dataBits = NOPAR_8BIT,
        .fifoSize = FIFO_FULL,
        .isBlocking = false,
        .uartDev = UART_DEV1,
        .intPriority = 5,
        .bufferSize = 20,
        .stopBits = ONESTOP,
        .onReceive = uartHandler
    };
    uartHandle = drv_uartNew(&uartConf);
    
    while(1) {
        drv_uartPuts(uartHandle, "test\n\r");
        os_timerDelay(10);
    }
}

int main(void) {
    
    os_threadConfig_t threadConfig = {
        .name=  "UT",
        .priority = THREAD_PRIO_NORM,
        .threadCallback = uartThread,
        .threadArgs = NULL,
        .stackSize = STACK_SIZE_DEFAULT
    };
    
    os_threadNew(&threadConfig);
    os_startScheduler();
    while(1);
    return (EXIT_SUCCESS);
}

void vAssertCalled(const char *file, uint32_t line)
{
    volatile uint32_t ul = 0;

    (void)file;
    (void)line;

    __asm volatile("di");
    {
        /* Set ul to a non-zero value using the debugger to step out of this
        function. */
        while (ul == 0) {
            portNOP();
        }
    }
    __asm volatile("ei");
}