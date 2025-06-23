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



/**
<!-- The following provides documentation of the NeoCortec API in markdown and HTML format for Doxygen -->

\mainpage Introduction and overview

The application layer interfaces with the node through the API when payload data is being
transmitted or received. The API implements handles for encoding/decoding and synchronisation of 
the data transfer to and from the node.

For further information, please also refer to the documents on 
NeoCortec's download page: <br>
<a href="http://neocortec.com/downloads/">neocortec.com/downloads/</a>


Overview of the API context
====================

The Application using the API typically resides in a microcontroller which communicates with the NeoCortec node through a UART.

\image html  "ApiContext.png" "API context" width=500px
\image latex "ApiContext.pdf" "API context" width=12cm

\latexonly \newpage \endlatexonly

Interface between the API and the Tx part of the Application
--------------------

Functions for initiating a message to the NeoCortec module by enqueueing it for transmission:

\msc
arcgradient = 0;
tx[label="Tx part"], api[label="API"];
tx=>api [label="NcApiSendUnacknowledged()",      URL="\ref NcApiSendUnacknowledged()"];
tx=>api [label="NcApiSendAcknowledged()",        URL="\ref NcApiSendAcknowledged()"];
tx=>api [label="NcApiSendNodeInfoRequest()",     URL="\ref NcApiSendNodeInfoRequest()"];
tx=>api [label="NcApiSendNeighborListRequest()", URL="\ref NcApiSendNeighborListRequest()"];
tx=>api [label="NcApiSendNetCmd()",              URL="\ref NcApiSendNetCmd()"];
tx=>api [label="NcApiSendWesCmd()",              URL="\ref NcApiSendWesCmd()"];
tx=>api [label="NcApiSendWesResponse()",         URL="\ref NcApiSendWesResponse()"];
tx=>api [label="NcApiSendAltCmd()",              URL="\ref NcApiSendAltCmd()"];
\endmsc

Unfortunately the links in the sequence diagram does not work in the PDF version 
of the documentation, and hence links to the functions are provided here:
\li NcApiSendUnacknowledged() <br>
NcApiSendAcknowledged() <br>
NcApiSendNodeInfoRequest() <br>
NcApiSendNeighborListRequest() <br>
NcApiSendNetCmd() <br>
NcApiSendWesCmd() <br>
NcApiSendWesResponse() <br>
NcApiSendAltCmd()

Interface between the API and the Tx part of the API Support
--------------------

Functions related to the actual transmission of a message to the NeoCortec module as
response to the CTS signal:

\msc
arcgradient = 0;
tx[label="Tx part"], api[label="API"];
tx<=api [label="NcApiSupportTxData()",           URL="\ref NcApiSupportTxData()"];
tx<=api [label="NcApiSupportMessageWritten()",   URL="\ref NcApiSupportMessageWritten()"];
\endmsc

NcApiSupportTxData() will be called if there is a pending enqueued message when NcApiCtsActive()
is called. <br>
Please also refer to the section "Interface between the API and the CTS part of the API Support".

Unfortunately the links in the sequence diagram does not work in the PDF version 
of the documentation, and hence links to the functions are provided here:
\li NcApiSupportTxData() (provided by the API Support) <br>
NcApiSupportMessageWritten()

Interface between the API and the CTS part of the API Support
--------------------

Function to inform the API that the NeoCortec module is ready to receive messages:

\msc
arcgradient = 0;
cts[label="CTS part"], api[label="API"];
cts=>api [label="NcApiCtsActive()",              URL="\ref NcApiCtsActive()"];
\endmsc

If there is a pending enqueued message the API will call NcApiSupportTxData(), and when the entire 
message has been delivered to the UART, NcApiSupportMessageWritten() is called. <br>
Please also refer to the section "Interface between the API and the Tx part of the API Support".

Unfortunately the links in the sequence diagram does not work in the PDF version 
of the documentation, and hence links to the functions are provided here:
\li NcApiCtsActive()

Interface between the API and the Rx part of the Application
--------------------

When data is received from the mesh network, a series of call-back functions will 
call the Application with the received data. Depending on what type of data is received, 
a type specific call-back is issued. It is optional for the application layer to register 
for the call-backs. 

Call back functions to pass on received messages to the application:

\msc
arcgradient = 0;
api[label="API"], rx[label="Rx part"];
api=>rx [label="pfnReadCallback()"];
api=>rx [label="pfnHostAckCallback()"];
api=>rx [label="pfnHostNAckCallback()"];
api=>rx [label="pfnHostDataCallback()"];
api=>rx [label="pfnHostDataHapaCallback()"];
api=>rx [label="pfnHostUnackDataCallback()"];
api=>rx [label="pfnHostUnackDataHapaCallback()"];
api=>rx [label="pfnNodeInfoReplyCallback()"];
api=>rx [label="pfnNeighborListReplyCallback()"];
api=>rx [label="pfnWesSetupRequestCallback()"];
api=>rx [label="pfnWesStatusCallback()"];
api=>rx [label="pfnNetCmdResponseCallback()"];
api<=rx [label="NcApiExecuteCallbacks()",        URL="\ref NcApiExecuteCallbacks()"];
\endmsc

Unfortunately the links in the sequence diagram does not work in the PDF version 
of the documentation, and hence links to the functions are provided here:
\li pfn...Callback() (provided by the Application) <br>
NcApiExecuteCallbacks()

Interface between the API and the Rx part of the API Support
--------------------

Functions related to reception of messages from the NeoCortec module:

\msc
arcgradient = 0;
api[label="API"], rx[label="Rx part"];
api=>rx [label="NcApiSupportMessageReceived()",  URL="\ref NcApiSupportMessageReceived()"];
api<=rx [label="NcApiRxData()",                  URL="\ref NcApiRxData()"];
\endmsc

When data is received from the UART, it shall be forwarded to NcApiRxData().
When a complete message has been received, the application will be notified via 
NcApiSupportMessageReceived().

Unfortunately the links in the sequence diagram does not work in the PDF version 
of the documentation, and hence links to the functions are provided here:
\li NcApiSupportMessageReceived() (provided by the API Support) <br>
NcApiRxData()

*/
