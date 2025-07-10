#include "BaseUart.h"
#include <windows.h>

#ifndef WINDOWS_UART_H_
#define WINDOWS_UART_H_

#define READ_BUFFER_SIZE 256

class WindowsUart : public NcUart
{
public:
    WindowsUart(const char * port, int baudrate,  int rxTimeOutMs);
    int Open();
    int Write(uint8_t * data, int length);
    int Read(uint8_t * buffer, int length);
    int Close();
    int CtsTask();

private:
    DWORD dwEvtMask;
    HANDLE serialHandle;
    int baudrate;
    int rx_len;
    int rx_pos;
    uint8_t rx_buffer[READ_BUFFER_SIZE];
};

#endif  // WINDOWS_UART_H_