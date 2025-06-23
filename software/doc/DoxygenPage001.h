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

\page Page001 Initialisation and Application provided functions


Initialisation
====================

The application must call NcApiInit() once for initialisation.

\msc
arcgradient = 0;
App[label="Application"], api[label="API"];
App=>api [label="NcApiInit()",      URL="\ref NcApiInit()"];
\endmsc


UART service functions
====================

The Application must provide the following actions to UART service requests, 
cf. the "Introduction and overview" chapter:

<ul>
<li>CTS active low:               <br> NcApiCtsActive()</li>
<li>RX data (optionally also RT): <br> NcApiRxData()</li>
</ul>

The application must provied means of servicing UART CTS signal changes. When the 
CTS edges low, NcApiCtsActive() must be called.
If there is a pending enqueued message the API will call NcApiSupportTxData(), and when the entire 
message has been delivered to the UART, NcApiSupportMessageWritten() is called. 

The application must provied means of servicing UART Rx data. When a byte is received
NcApiRxData() must be called.
When a complete message has been received, the application will be notified via 
NcApiSupportMessageReceived().


Basic Rx/Tx functions
====================

The Application must provide the following basic functions for receiving and transmitting messages, 
cf. the "Introduction and overview" chapter:

<ul>
<li>NcApiSupportTxData()</li>
<li>NcApiSupportMessageWritten()</li>
<li>NcApiSupportMessageReceived()</li>
</ul>


Received message callback functions
====================

The applicaton can chose to be notified for certain received message types through 
the instance of NcApiRxHandlers, which is aet of application callback function pointers. <br>
Cf. the "Introduction and overview" chapter.

*/
