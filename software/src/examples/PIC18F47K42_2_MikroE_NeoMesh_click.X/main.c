/*
 * File:   main.c
 * Author: jbun
 *
 * Created on March 17, 2025, 2:42 PM
 */


#include <xc.h>           // Include the PIC18F47K42 header file
#define _XTAL_FREQ 64000000 // Set the system clock frequency (e.g., 8 MHz)


#include "mcc_generated_files/system/system.h"
#include "NcApi.h"
#include "NcApiExample.h"

#include <string.h>

void ExampleSetupNcApi();
NcApiErrorCodes ExampleSendUnacknowledged(uint16_t destNodeId, uint8_t port, uint16_t appSeqNo, uint8_t * payload, uint8_t payloadLen);

int cts_cnt = 0;

void CTS_irq(void)
{
    cts_cnt++;
    PORTEbits.RE0 = ~PORTEbits.RE0; // Flash LED
    CtsIrqDetected();
}


void main(void) 
{
    int seq_no = 1;
    int last_cnt = 0;
    SYSTEM_Initialize();
    IO_RB4_SetInterruptHandler(CTS_irq);

    PORTDbits.RD7 = 0;
    PORTEbits.RE0 = 0;

    INTERRUPT_GlobalInterruptEnable();
    
    ExampleSetupNcApi();

    printf("\r\nPIC18F47K42 CNANO  -  NeoMesh NcApi Example");
    printf("\r\n> ");

    // Main loop
    while (1) {
        if (UART1_IsRxReady())
        {
            uint8_t c = UART1_Read();
            RxIrqDetected(c);
        }
        
        if (UART2_IsRxReady())
        {
            NcApiErrorCodes res = NCAPI_OK;
            uint8_t c = UART2_Read();

            switch (c)
            {
                case 'i':
                {
                    tNcApiNodeInfoParams args = {0, NULL};
                    res = NcApiSendNodeInfoRequest(0, &args);
                    break;    
                }
                
                case 'n': 
                {
                    tNcApiNeighborListRequestParams args = {0, NULL};
                    res = NcApiSendNeighborListRequest(0, &args); 
                    break;
                }
                
                case 'r':
                {
                    tNcApiRouteInfoRequestParams args = {0, NULL};
                    res = NcApiSendRouteInfoRequest(0, &args);
                    break;
                }                              

                case 's':
                {
                    char *s = "PIC18F47K42";
                    res = ExampleSendUnacknowledged(0x0011, 1, seq_no++, s, strlen(s));
                    break;
                }

                case 'h': // help
                default:
                {
                    printf("Commands:\n\r");
                    printf("i - Node Info Request\n\r");
                    printf("n - Neighbor List Request\n\r");
                    printf("r - Route Info Request\n\r");
                    printf("s - Send unacknowledged message\n\r");
                    printf("h - Help\n\r");
                    break;
                }
            }
            
            if (res != NCAPI_OK)
            {
                printf("Error: %d\n\r", res);
            }

            printf("\r\n> ");
        }        
    }
}
