/*
 * File:   main.c
 * Author: jbun
 *
 * Created on March 17, 2025, 2:42 PM
 */

#include "NcApi.h"
#include "NcApiExample.h"
#include "my_ncapi_hal.h"

#include <string.h>

int main(void) 
{
    int seq_no = 1;

    my_ncapi_hal_init();

    ExampleSetupNcApi();

    my_ncapi_hal_terminal_printf("\r\n%s  -  NeoMesh NcApi Example", my_ncapi_hal_get_hw_str());
    my_ncapi_hal_terminal_printf("\r\n> ");

    // Main loop
    while (1) {
        int c;

        // Poll UART for incoming data
        c = my_ncapi_hal_uart_getch();
        if (c != -1)
        {
            RxIrqDetected(c);
        }

        // Poll terminal for user input
        c = my_ncapi_hal_terminal_getch();       
        if (c != -1)
        {
            NcApiErrorCodes res = NCAPI_OK;

            switch (c)
            {
                case 'i':
                {
                    tNcApiNodeInfoParams args = {{0}, NULL};
                    res = NcApiSendNodeInfoRequest(0, &args);
                    break;    
                }
                
                case 'n': 
                {
                    tNcApiNeighborListRequestParams args = {{0}, NULL};
                    res = NcApiSendNeighborListRequest(0, &args); 
                    break;
                }
                
                case 'r':
                {
                    tNcApiRouteInfoRequestParams args = {{0}, NULL};
                    res = NcApiSendRouteInfoRequest(0, &args);
                    break;
                }                              

                case 's':
                {
                    const char *s = my_ncapi_hal_get_hw_str();
                    res = ExampleSendUnacknowledged(0x0011, 1, seq_no++, (uint8_t*)s, strlen(s));
                    break;
                }

                case 'h': // help
                default:
                {
                    my_ncapi_hal_terminal_printf("Commands:\n\r");
                    my_ncapi_hal_terminal_printf("i - Node Info Request\n\r");
                    my_ncapi_hal_terminal_printf("n - Neighbor List Request\n\r");
                    my_ncapi_hal_terminal_printf("r - Route Info Request\n\r");
                    my_ncapi_hal_terminal_printf("s - Send unacknowledged message\n\r");
                    my_ncapi_hal_terminal_printf("h - Help\n\r");
                    break;
                }
            }
            
            if (res != NCAPI_OK)
            {
                my_ncapi_hal_terminal_printf("Error: %d\n\r", res);
            }

            my_ncapi_hal_terminal_printf("\r\n> ");
        }        
    }
}
