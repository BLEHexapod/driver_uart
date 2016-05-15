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

#include "drv_uart.h"
#include <sys/attribs.h>
#include <xc.h>

struct drv_uartHandle {
    uartDevices_t uartDev;      /**<Desired uart device to initialize, only UARTDEV1 is supported atm*/
    bool blocking : 1;          /**<Use interrupts? must be on(1) for now*/
    callback onReceive;         /**<Function to execute if the receive buffer is full*/
    volatile uint8_t *buffer;   /**<Buffer with the received uart data*/
    volatile uint8_t bufIndex;  /**<Keeps tracks of the index of the buffer*/
};

volatile uint8_t *uartBuf[NUM_UARTS] = {0};

drv_uartEventHandler_t handlers[NUM_UARTS] = {0};

/**
 * Local function to calculate the baudrate according to the datasheet
 * @param baud  Desired baudrate, see BAUDRATE enum
 * @param highSpeed Enable high speed mode
 * @return      int value to configure UxBRG with
 * @see CPUCLK
 */
static uint16_t calculateBaud(uartBaudRates_t baud, bool highSpeed)
{
    if (highSpeed) {
        return (SYS_CLK_FREQ / (4 * baud)) - 1;
    } else {
        return (SYS_CLK_FREQ / (16 * baud)) - 1;
    }
}

/**
 * Function to enable interrupts for a uart device
 * @param handle    Handle to the uart instance.
 * @param priority  Interrupt priority
 */
static void uartEnableInt(drv_uartHandle_t handle, uint8_t priority)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            IFS0CLR = IFS0_U1E_BIT;
            IPC6SET = (priority << 2) | (3);
            IEC0SET = IEC0_U1E_BIT;
            break;
        case UART_DEV2:
            IFS1CLR = IFS1_U2RX_BIT;
            IPC8SET = (priority << 2) | (3);
            IEC1SET = IEC1_U2E_BIT;
            break;
    }
}

/**
 * Set flags in the mode register for a uart device
 * @param handle Handle to the uart instance.
 * @param flags Flags to set in the register.
 */
static void uartModeSetFlags(drv_uartHandle_t handle, uint32_t flags)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            U1MODESET = flags;
            break;
        case UART_DEV2:
            U2MODESET = flags;
            break;
    }
}

/**
 * Clear flags in the mode register for a uart device
 * @param handle Handle to the uart instance.
 * @param flags Flags to clear in the register.
 */
static void uartModeClrFlags(drv_uartHandle_t handle, uint32_t flags)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            U1MODECLR = flags;
            break;
        case UART_DEV2:
            U2MODECLR = flags;
            break;
    }
}

static void registerBuffer(drv_uartHandle_t handle)
{
    uartBuf[handle->uartDev] = handle->buffer;
}

/**
 * Clear uart buffer and set index counter to 0
 * @param handle Handle to the uart instance.
 */
static void uartFlush(drv_uartHandle_t handle)
{
    memset((uint8_t*) handle->buffer, 0, sizeof (handle->buffer));
    handle->bufIndex = 0;
}

static void uartHandlers(void *data, uint8_t source)
{
    switch (source) {
        case _UART1_VECTOR:
            if(handlers[UART_DEV1])
                handlers[UART_DEV1](data);
            break;
        case _UART2_VECTOR:
            if(handlers[UART_DEV2])
                handlers[UART_DEV2](data);
            break;
    }
}

void __ISR(_UART1_VECTOR, ipl6auto) uart1Handler(void)
{
    uint8_t i;
    for (i = 0; i < U1STAbits.URXISEL; i++)
        uartBuf[UART_DEV1][i] = U1RXREG;
    uartHandlers((void*) uartBuf, _UART1_VECTOR);
    IFS0CLR = IFS0_U1E_BIT;
}

void __ISR(_UART2_VECTOR, ipl1auto) uart2Handler(void)
{
    uint8_t i;
    for (i = 0; i < 4; i++)
        uartBuf[UART_DEV2][i] = U1RXREG;
    uartHandlers((void*) uartBuf, _UART2_VECTOR);
    IFS1CLR = IFS1_U2E_BIT;
}

drv_uartHandle_t drv_uartNew(drv_uartConfig_t *config)
{
    drv_uartHandle_t handle = calloc(1, sizeof (struct drv_uartHandle));
    handle->onReceive = config->onReceive;
    drv_uartSetBaud(handle, config->baud);
    drv_uartSetDataBits(handle, config->dataBits);
    drv_uartSetStopBit(handle, config->stopBits);
    drv_uartSetFifoSize(handle, config->fifoSize);
    handle->buffer = calloc(1, config->bufferSize);

    if (config->isBlocking)
        uartEnableInt(handle, config->intPriority);
    uartModeSetFlags(handle, U_ON);
    return handle;
}

void drv_uartEnable(drv_uartHandle_t handle)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            U1STASET = (1 << U_URXEN) | (1 << U_UTXEN);
            break;
        case UART_DEV2:
            U2STASET = (1 << U_URXEN) | (1 << U_UTXEN);
            break;
    }
}

void drv_uartPut(drv_uartHandle_t handle, uint8_t data)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            while (U1STAbits.UTXBF);
            U1TXREG = data;
            break;
        case UART_DEV2:
            while (U2STAbits.UTXBF);
            U2TXREG = data;
            break;
        default:
            break;
    }
}

int8_t drv_uartTryPut(drv_uartHandle_t handle, uint8_t data)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            if (U1STAbits.UTXBF)
                return UART_BUSY;
            else
                U1TXREG = data;
            return UART_SUCCES;
        case UART_DEV2:
            if (U2STAbits.UTXBF)
                return UART_BUSY;
            else
                U2TXREG = data;
            return UART_SUCCES;
        default:
            return UART_ERROR;
    }
}

void drv_uartPuts(drv_uartHandle_t handle, uint8_t *data)
{
    while (*data) {
        drv_uartPut(handle, *(data++));
    }
}

int8_t drv_uarTryPuts(drv_uartHandle_t handle, uint8_t *data)
{
    //TODO: keep track of index
    while (*data) {
        if(drv_uartTryPut(handle, *(data++) == UART_BUSY))
            return UART_BUSY;
    }
    return UART_SUCCES;
}

uint8_t drv_uartGet(drv_uartHandle_t handle)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            while (!U1STAbits.URXDA);
            return U1RXREG;
        case UART_DEV2:
            while (!U2STAbits.URXDA);
            return U2RXREG;
        default:
            return 0;
    }
}

uint8_t drv_uartTryGet(drv_uartHandle_t handle)
{
    switch(handle->uartDev) {
        case UART_DEV1:
            if(U1STAbits.URXDA)
                return U1RXREG;
            else
                return UART_NO_DATA;
            break;
        case UART_DEV2:
            if(U2STAbits.URXDA)
                return U1RXREG;
            else
                return UART_NO_DATA;
            break;
        default:
            return UART_NO_DATA;
    }
}

uint8_t drv_uartGets(drv_uartHandle_t handle, uint8_t *data)
{
    uint8_t i;
    for (i = 0; i < sizeof(data); i++) {
        data[i] = drv_uartGet(handle);
    }
    return i;
}

int8_t drv_uartTryGets(drv_uartHandle_t handle, uint8_t *data)
{
    //TODO implement interrupt based reading
    return 0;
}

void drv_uartSetOnReceive(drv_uartHandle_t handle, drv_uartEventHandler_t task)
{
    handle->onReceive = task;
}

void drv_uartSetBaud(drv_uartHandle_t handle, uartBaudRates_t baud)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            U1BRG = calculateBaud(baud, false);
            break;
        case UART_DEV2:
            U2BRG = calculateBaud(baud, false);
            break;
    }
}

void drv_uartSetDataBits(drv_uartHandle_t handle, uartDataBits_t data)
{
    switch (data) {
        case NOPAR_9BIT:
            uartModeSetFlags(handle, (1 << U_PDSEL0) | (1 << U_PDSEL1));
        case ODDPAR_8BIT:
            uartModeSetFlags(handle, (1 << U_PDSEL1));
            uartModeClrFlags(handle, (1 << U_PDSEL0));
            break;
        case EVENPAR_8BIT:
            uartModeSetFlags(handle, (1 << U_PDSEL0));
            uartModeClrFlags(handle, (1 << U_PDSEL1));
            break;
        case NOPAR_8BIT:
            uartModeClrFlags(handle, (1 << U_PDSEL0) | (1 << U_PDSEL1));
            break;
    }
}

void drv_uartSetStopBit(drv_uartHandle_t handle, uartStopBits_t stop)
{
    switch (stop) {
        case ONESTOP:
            uartModeClrFlags(handle, (1 << U_STSEL));
            break;
        case TWOSTOP:
            uartModeSetFlags(handle, (1 << U_STSEL));
    }
}

void drv_uartSetFifoSize(drv_uartHandle_t handle, uartFifoSizes_t fifoSize)
{
    switch (handle->uartDev) {
        case UART_DEV1:
            U1STAbits.URXISEL = fifoSize;
            break;
        case UART_DEV2:
            U2STAbits.URXISEL = fifoSize;
            break;
    }
}

void drv_uartDestroy(drv_uartHandle_t handle)
{
    uartModeClrFlags(handle, U_ON);
    free((void*) handle->buffer);
    free(handle);
}