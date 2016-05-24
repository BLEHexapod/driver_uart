/* 
 * File:   main.c
 * Author: bart452
 *
 * Created on 15 May 2016, 12:14
 */

#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pragmas.h"

/*
 * 
 */
int main(void) {

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


