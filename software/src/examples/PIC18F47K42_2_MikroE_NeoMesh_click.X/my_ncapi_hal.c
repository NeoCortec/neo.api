/*
 * File:   my_ncapi_hal.c
 * Author: jbu@neocortec.com
 *
 * Created on June 26, 2025, 2:42 PM
 */

#include <xc.h>           // Include the PIC18F47K42 header file
#define _XTAL_FREQ 64000000 // Set the system clock frequency (e.g., 8 MHz)
#include "mcc_generated_files/system/system.h"

#include "NcApi.h"
#include "NcApiExample.h"

#include <stdarg.h>
#include <stdio.h>

static int cts_cnt = 0;

void CTS_irq(void)
{
    cts_cnt++;
    PORTEbits.RE0 = ~PORTEbits.RE0; // Flash LED
    CtsIrqDetected();
}
  
void my_ncapi_hal_init()
{
    SYSTEM_Initialize();
    IO_RB4_SetInterruptHandler(CTS_irq);

    PORTDbits.RD7 = 0;
    PORTEbits.RE0 = 0;

    INTERRUPT_GlobalInterruptEnable();
}

const char *my_ncapi_hal_get_hw_str()
{
    static char *hw_str = "PIC18F47K42 CNANO";
    return hw_str;
}

int my_ncapi_hal_uart_getch()
{
    if (UART1_IsRxReady())
    {
        return UART1_Read();
    }
    else
    {
        return -1; // No input available
    }
}

void my_ncapi_hal_uart_putch(uint8_t c)
{
    while(!UART1_IsTxReady());
    UART1_Write(c);
}

int my_ncapi_hal_uart_write(uint8_t *data, int length)
{
    if (data == NULL || length <= 0)
    {
        return 0; // Invalid parameters
    }

    int bytes_written = 0;
    for (int i = 0; i < length; i++)
    {
        while(!UART1_IsTxReady());
        UART1_Write(data[i]);
        bytes_written++;
    }
    
    return bytes_written;
}

int my_ncapi_hal_terminal_getch()
{
    if (UART2_IsRxReady())
    {
        return UART2_Read();
    }
    else
    {
        return -1; // No input available
    }
}

int my_ncapi_hal_terminal_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        while(!UART2_IsTxReady());
        UART2_Write(buffer[i]);
    }
    
    va_end(args);
    
    return 0; // Success
}
