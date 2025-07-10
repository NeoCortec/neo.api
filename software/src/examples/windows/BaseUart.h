#include <stdint.h>

#ifndef NC_UART_H
#define NC_UART_H

class NcUart
{   
public:
    virtual int Open() { return 0; };
    virtual void *GetHandle() { return 0; };
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    virtual int Write(uint8_t * data, int length) { return 0; };
    virtual int Read(uint8_t * buffer, int length) { return 0; };
#pragma GCC diagnostic pop
    virtual int Close() { return 0; };
};

#endif // NC_UART_H