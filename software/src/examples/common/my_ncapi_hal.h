/*
 * File:   my_ncapi_hal.h
 * Author: jbu@neocortec.com
 *
 * Created on June 26, 2025, 2:42 PM
 */

 #include <stdint.h>
 
#ifndef MY_HCAPI_HAL_H_
#define MY_HCAPI_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

void my_ncapi_hal_init();
const char *my_ncapi_hal_get_hw_str();
int my_ncapi_hal_uart_getch();
void my_ncapi_hal_uart_putch(uint8_t c);
int my_ncapi_hal_uart_write(uint8_t *data, int length);
int my_ncapi_hal_uart_read(uint8_t *buffer, int length);
int my_ncapi_hal_terminal_getch();
int my_ncapi_hal_terminal_printf(const char *format, ...);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* MY_HCAPI_HAL_H_ */

