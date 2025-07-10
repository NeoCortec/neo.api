/*
 * File:   my_ncapi_hal.c
 * Author: jbu@neocortec.com
 *
 * Created on June 26, 2025, 2:42 PM
 */

#include "NcApi.h"
#include "NcApiExample.h"
#include "WindowsUart.h"
#include "my_ncapi_hal.h"

#include <stdarg.h>
#include <stdio.h>
#include <conio.h>

static WindowsUart *uart = NULL;

void my_ncapi_hal_init()
{
    uart = new WindowsUart("COM8", 115200, 10);
    if (uart->Open() != 0)
    {
        printf("Failed to open UART\n");
    }
}

const char *my_ncapi_hal_get_hw_str()
{
    static char hw_str[] = "Windows";
    return hw_str;
}

int my_ncapi_hal_uart_getch()
{
    uint8_t c = 0;
   
    if (my_ncapi_hal_uart_read(&c, 1) == 1)
    {
        return c;
    }
    else
    {
        return -1; // No input available
    }
}

int my_ncapi_hal_uart_read(uint8_t *buffer, int length)
{
    if (uart == NULL || buffer == NULL || length <= 0)
    {
        return 0; // Invalid parameters
    }
    
    return uart->Read(buffer, length);
}

void my_ncapi_hal_uart_putch(uint8_t c)
{
    my_ncapi_hal_uart_write(&c, 1);
}

int my_ncapi_hal_uart_write(uint8_t *data, int length)
{
    if (uart == NULL || data == NULL || length <= 0)
    {
        return 0; // Invalid parameters
    }

    return uart->Write(data, length);
}

int my_ncapi_hal_terminal_getch()
{
    if (_kbhit())
    {
        return _getch();
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
    printf("%s", buffer);
    
    va_end(args);
    
    return 0; // Success
}
