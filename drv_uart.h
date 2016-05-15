/*
 * Copyright 2015 - 2016 Bart Monhemius.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UART_H
#define	UART_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "pinDefs.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define NUM_UARTS   2
    
// Error codes
#define UART_BUSY   -1
#define UART_SUCCES 0
#define UART_ERROR  -2
#define UART_NO_DATA UINT8_MAX

//UART bit definitions
#define U_ON        15
#define U_PDSEL0    1
#define U_PDSEL1    2
#define U_URXEN     12
#define U_UTXEN     10
#define U_STSEL     0

typedef struct drv_uartHandle *drv_uartHandle_t;
typedef void(*drv_uartEventHandler_t)(void*);

typedef enum {
    BAUD1200 = 1200,
    BAUD2400 = 2400,
    BAUD9600 = 9600,
    BAUD19200 = 19200,
    BAUD38400 = 38400,
    BAUD57600 = 57600,
    BAUD115200 = 115200
} uartBaudRates_t;

typedef enum {
    ONESTOP = 0,
    TWOSTOP
} uartStopBits_t;

typedef enum {
    NOPAR_9BIT = 0,
    ODDPAR_8BIT,
    EVENPAR_8BIT,
    NOPAR_8BIT
} uartDataBits_t;

typedef enum {
    UART_DEV1 = 0,
    UART_DEV2 = 1
} uartDevices_t;

typedef enum {
    FIFO_FULL = 3,
    FIFO_3 = 2,
    FIFO_CHAR = 0
} uartFifoSizes_t;

typedef struct {
    uartBaudRates_t baud;               /**<Desired baudrate, see the BAUDRATE enum*/
    uartStopBits_t stopBits;            /**<Desired number of stopbits, see the STOPBITS enum*/
    uartDataBits_t dataBits;            /**<Desired number of data and parity bits, see DATABITS enum*/
    uartDevices_t uartDev;              /**<Desired uart device to initialize, only UARTDEV1 is supported atm*/
    bool isBlocking : 1;                /**<Use interrupts? must be on(1) for now*/
    drv_uartEventHandler_t onReceive;   /**<Function to execute if the receive buffer is full*/
    uint8_t intPriority;                /**<Priority of the interrupt*/
    uartFifoSizes_t fifoSize;           /**<Size of the hardware FIFO buffer*/
    size_t bufferSize;                  /**<Size of the software buffer*/
} drv_uartConfig_t;

/**
 * Initialise and configure a new uart instance.
 * @param config    Configuration for the uart device.
 * @param handle    Handle to the uart instance.
 */
drv_uartHandle_t drv_uartNew(drv_uartConfig_t *config);

/**
 * Enable the uart device.
 * @param handle    Handle to the uart instance.
 */
void drv_uartEnable(drv_uartHandle_t handle);

/**
 * Send a char over uart
 * @param data  Char to send
 * @param handle    Handle to the uart instance.
 */
void drv_uartPut(drv_uartHandle_t handle, uint8_t data);

/**
 * Try to send a char using uart, this function is non-blocking and might need
 * to be called multiple times if the uart device is busy.
 * @param handle
 * @return 
 */
int8_t drv_uartTryPut(drv_uartHandle_t handle, uint8_t data);

/**
 * Send byte array / pointer over uart
 * @param data  Char pointer or array to send
 * @param handle    Handle to the uart instance.
 * @see uartPut
 */
void drv_uartPuts(drv_uartHandle_t handle, uint8_t data[]);

/**
 * Try to send a string using uart, this function is non-blocking and might need
 * to be called multiple times if the uart device is busy.
 * @param handle
 * @return 
 */
int8_t drv_uartTryPuts(drv_uartHandle_t handle, uint8_t *data);

/**
 * Get char from serial
 * @param index buffer index to get
 * @param handle    Handle to the uart instance.
 * @return char received from serial
 */
uint8_t drv_uartGet(drv_uartHandle_t handle);

/**
 * Try to receive a char using uart, this function is non-blocking and might need
 * to be called multiple times if the uart device is busy or no data is available.
 * @param handle    Handle to the uart instance.
 * @return The received character
 * @retval 255 If no data was available
 */
uint8_t drv_uartTryGet(drv_uartHandle_t handle);

/**
 * Get the full buffer of chars from serial, might be incomplete or empty
 * @param data  char array or pointer to store the buffer in
 * @param handle    Handle to the uart instance.
 * @return Number of bytes read.
 */
uint8_t drv_uartGets(drv_uartHandle_t handle, uint8_t* data);

/**
 * Try to receive a string using uart, this function is non-blocking and might need
 * to be called multiple times if the uart device is busy or no data is available.
 * @param handle    Handle to the uart instance.
 * @return 
 */
int8_t drv_uartTryGets(drv_uartHandle_t handle, uint8_t *data);

/**
 * Change the callback when a the uart buffer is full
 * @param task      function to excecute
 * @param handle    Handle to the uart instance.
 */
void drv_uartSetOnReceive(drv_uartHandle_t handle, callback task);

/**
 * Change baudrate of a uart device
 * @param baud      Desired baudrate, see the BAUDRATE enum
 * @param handle    Handle to the uart instance.
 */
void drv_uartSetBaud(drv_uartHandle_t handle, uartBaudRates_t baud);

/**
 * Change the number of stopbits of a uart device
 * @param data      Desired number of data and parity bits, see DATABITS enum
 * @param handle    Handle to the uart instance.
 */
void drv_uartSetDataBits(drv_uartHandle_t handle, uartDataBits_t data);

/**
 * Configure the nimber of stopbits for the uart protocol.
 * @param stop      Desired number of stopbits, see the STOPBITS enum
 * @param handle    Handle to the uart instance.
 */
void drv_uartSetStopBit(drv_uartHandle_t device, uartStopBits_t stop);

/**
 * Set the hardware fifo size for uart's received data.
 * @param handle    Handle to the uart instance.
 * @param fifoSize  Fifo size.
 */
void drv_uartSetFifoSize(drv_uartHandle_t handle, uartFifoSizes_t fifoSize);

/**
 * Delete the uart driver instance and free up memory
 * @param handle    Handle to the uart instance.
 */
void drv_uartDestroy(drv_uartHandle_t handle);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */
