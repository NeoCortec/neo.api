#include "WindowsUart.h"
#include "NcApi.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

static int CtsTaskRun(void *p)
{
    WindowsUart *uart = (WindowsUart *)p;
    uart->CtsTask();
    return 0;   
}

WindowsUart::WindowsUart(const char * port, int baudrate, int /*rxTimeOutMs*/)
    : serialHandle(NULL), baudrate(baudrate)
{
    char full_port[32];
    sprintf(full_port, "\\\\.\\%s", port);

    this->serialHandle = CreateFile(full_port, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if(this->serialHandle == INVALID_HANDLE_VALUE)
    {
        printf ("Could not open COM port: %s\nCreateFile failed with error %ld.\n", port, GetLastError());
    }
    this->baudrate = baudrate;
}

int WindowsUart::Open()
{
    DCB serialParams = { };
    serialParams.DCBlength = sizeof(serialParams);

    if (this->serialHandle == NULL)
    {
        return -1;
    }
    int success = GetCommState(this->serialHandle, &serialParams);
    if(!success)
    {
        printf("Could not get COMM state\n");
        return -1;
    }

    serialParams.BaudRate = this->baudrate;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;
    serialParams.fOutxCtsFlow = true;
    serialParams.fRtsControl = RTS_CONTROL_DISABLE;

    success = SetCommState(this->serialHandle, &serialParams);
    if(!success)
    {
        printf("Could not set COMM state %ld\n", GetLastError());
        return -1;
    }

    COMMTIMEOUTS timeout = { };
    timeout.ReadIntervalTimeout = 1;
    timeout.ReadTotalTimeoutConstant = 1;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    success = SetCommTimeouts(this->serialHandle, &timeout);
    if(!success)
    {
        printf("Could not set COMM timeouts\n");
        return -1;
    }

    // Start the CTS task
    HANDLE ctsThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CtsTaskRun, this, 0, NULL);
    if (ctsThread == NULL)
    {
        printf("Could not create CTS task thread, error %ld\n", GetLastError());
        return -1;
    }

    return 0;
}

int WindowsUart::CtsTask()
{
    DWORD dwModemStatus;
    BOOL fCTS = 16;
    DWORD dwCommEvent;

    // printf("Cts ioctl task started\r\n");

	if (!SetCommMask(this->serialHandle, EV_CTS))
	{
		printf("SetCommMask failed with error %ld\n", GetLastError());
		return -1;
	}

	while (true)
	{
		if (!WaitCommEvent(this->serialHandle, &dwCommEvent, NULL))
		{
			printf("WaitCommEvent failed with error %ld\n", GetLastError());
			continue;
		}
		if (!GetCommModemStatus(this->serialHandle, &dwModemStatus))
		{
			printf("GetCommModemStatus failed with error %ld\n", GetLastError());
			continue;
		}
		fCTS = MS_CTS_ON & dwModemStatus;
		if (fCTS == MS_CTS_ON)
		{
            NcApiNwuActive(0); // Clear rx buffer in NcApi
			NcApiCtsActive(0); // Notify NcApi that CTS is active
			// EscapeCommFunction(this->serialHandle, SETRTS);  // For testing purposes only
		}
		else
		{
			// EscapeCommFunction(this->serialHandle, CLRRTS);	// For testing purposes only
            long unsigned int bytes_read = 0;

            this->rx_len = bytes_read;
            this->rx_pos = 0;

            BOOL success = ReadFile(this->serialHandle, this->rx_buffer, 
                sizeof(this->rx_buffer), &bytes_read, NULL);
            
            if (success && bytes_read > 0)
            {
                this->rx_len = bytes_read;
            }
        }
	}

	return -1; 
}

int WindowsUart::Write(uint8_t * data, int length)
{
    long unsigned int bytes_written = 0;
	EscapeCommFunction(this->serialHandle, CLRRTS);	// For testing purposes only
    WriteFile(this->serialHandle, data, length, &bytes_written, NULL);
	EscapeCommFunction(this->serialHandle, SETRTS);	// For testing purposes only
    return bytes_written;
}

int WindowsUart::Read(uint8_t * buffer, int length)
{
    int rest = this->rx_len - this->rx_pos;
    if (rest > length)
    {
        rest = length;
    }

    if (rest > 0)
    {
        // Copy the available data from the rx_buffer to the provided buffer
        memcpy(buffer, this->rx_buffer + this->rx_pos, rest);
        this->rx_pos += rest;
        if (this->rx_pos >= this->rx_len)
        {
            this->rx_len = 0;
            this->rx_pos = 0;
        }
        return rest;
    }

    return 0; // No data available
}

int WindowsUart::Close()
{
    CloseHandle(this->serialHandle);
    return 1;
}
