/*
Copyright (c) 2025, NeoCortec A/S
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "NcApi.h"
#include "NeoParser.h"
#include "NcApiExample.h"


// Global functions expected by NcApi


NcApiErrorCodes NcApiSupportTxData(uint8_t n, uint8_t * finalMsg, uint8_t finalMsgLength)
{
    UNUSED(n);
    UNUSED(finalMsg);
    UNUSED(finalMsgLength);
    // Write to particular UART indexed by n
    return NCAPI_OK;
}

void NcApiSupportMessageWritten(uint8_t n, void * callbackToken, uint8_t * finalMsg, uint8_t finalMsgLength)
{
    UNUSED(n);
    UNUSED(callbackToken);
    UNUSED(finalMsg);
    UNUSED(finalMsgLength);
}

// TODO (jbu): This will be changed later so the Api always makes a callback if is defined otherwise this function will be called
#ifdef LET_NC_API_HANDLE_CALLBACKS
void NcApiSupportMessageReceived(uint8_t n,void * callbackToken, uint8_t * msg, uint8_t msgLength)
{
    UNUSED(callbackToken);
    NcApiExecuteCallbacks(n,msg,msgLength);
}
#else

typedef union
{
    tNcApiHostAckNack 		NcApiAckNack;
    tNcApiHostData 			NcApiHostData;
    tNcApiHostDataHapa 		NcApiHostDataHapa;
    tNcApiHostUnackData 		NcApiHostDataUnack;
    tNcApiHostUnackDataHapa 	NcApiHostDataUnackHapa;
    tNcApiWesStatus 		NcApiWesStatus;
    tNcApiNodeInfoReply		NcApiNodeInfo;
    tNcApiNeighborListReply	NcApiNeighborList;
    tNcApiNetCmdReply		NcApiNetCmdResponse;
    tNcApiWesSetupRequest	NcApiWesSetupRequest;
} tNcApiAllMessages;

void NcApiSupportMessageReceived(uint8_t n,void * callbackToken, uint8_t * msg, uint8_t msgLength)
{
    tNcApiAllMessages NcApiMsg;	

    switch(msg[0])
    {
    case HostAckEnum:
        NcApiGetMsgAsHostAck(msg, &NcApiMsg.NcApiAckNack);
        break;
    case HostNAckEnum:
        NcApiGetMsgAsHostAck(msg, &NcApiMsg.NcApiAckNack);
        break;
    case HostDataEnum:
        NcApiGetMsgAsHostData(msg, &NcApiMsg.NcApiHostData);
        break;
    case HostDataHapaEnum:
        NcApiGetMsgAsHostDataHapa(msg, &NcApiMsg.NcApiHostDataHapa);
        break;
    case HostUnackDataEnum:
        NcApiGetMsgAsHostUnackData(msg, &NcApiMsg.NcApiHostDataUnack);
        break;
        
    case HostUnackDataHapaEnum:
        NcApiGetMsgAsHostUnackDataHapa(msg, &NcApiMsg.NcApiHostDataUnackHapa);
        break;
    case NodeInfoReplyEnum:
        NcApiGetMsgAsNodeInfoReply(msg, &NcApiMsg.NcApiNodeInfo);
        break;
    case NeighborListReplyEnum:
        NcApiGetMsgAsNeighborListReply(msg, &NcApiMsg.NcApiNeighborList);
        break;
    case NetCmdReplyEnum:
        NcApiGetMsgAsNetCmdResponse(msg, &NcApiMsg.NcApiNetCmdResponse);
        break;
    case WesStatusEnum:
        NcApiGetMsgAsWesStatus(msg, &NcApiMsg.NcApiWesStatus);
        break;
    case WesSetupRequestEnum:
        NcApiGetMsgAsWesSetupRequest(msg, &NcApiMsg.NcApiWesSetupRequest);
        break;
             
    default:
        // unhandled 
        break;
    }
}
#endif

// Call this function when a CTS interrupts is detected  ...
void CtsIrqDetected()
{
    // ... or call the function below directly in your irq function:
    NcApiCtsActive(0);
}

// Call this function when a RX interrupts is detected  ...
void RxIrqDetected(uint8_t byte)
{
    // ... or call the function below directly in your irq function:
    NcApiRxData(0, byte);
}

// Call this function when a Nwu interrupts is detected  ...
void NwuIrqDetected()
{
    // ... or call the function below directly in your irq function:
    NcApiNwuActive(0);
}