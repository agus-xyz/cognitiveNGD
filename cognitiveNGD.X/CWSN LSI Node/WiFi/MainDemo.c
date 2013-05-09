/*********************************************************************
 *
 *  Main Application Entry Point and TCP/IP Stack Demo
 *  Module for Microchip TCP/IP Stack
 *   -Demonstrates how to call and use the Microchip TCP/IP stack
 *	 -Reference: Microchip TCP/IP Stack Help (TCPIP Stack Help.chm)
 *
 *********************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    TCPIP.h
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.11b or higher
 *					Microchip C30 v3.24 or higher
 *					Microchip C18 v3.36 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2010 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author              Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Nilesh Rajbharti		4/19/01		Original (Rev. 1.0)
 * Nilesh Rajbharti		2/09/02		Cleanup
 * Nilesh Rajbharti		5/22/02		Rev 2.0 (See version.log for detail)
 * Nilesh Rajbharti		7/9/02		Rev 2.1 (See version.log for detail)
 * Nilesh Rajbharti		4/7/03		Rev 2.11.01 (See version log for detail)
 * Howard Schlunder		10/1/04		Beta Rev 0.9 (See version log for detail)
 * Howard Schlunder		10/8/04		Beta Rev 0.9.1 Announce support added
 * Howard Schlunder		11/29/04	Beta Rev 0.9.2 (See version log for detail)
 * Howard Schlunder		2/10/05		Rev 2.5.0
 * Howard Schlunder		1/5/06		Rev 3.00
 * Howard Schlunder		1/18/06		Rev 3.01 ENC28J60 fixes to TCP, 
 *									UDP and ENC28J60 files
 * Howard Schlunder		3/01/06		Rev. 3.16 including 16-bit micro support
 * Howard Schlunder		4/12/06		Rev. 3.50 added LCD for Explorer 16
 * Howard Schlunder		6/19/06		Rev. 3.60 finished dsPIC30F support, added PICDEM.net 2 support
 * Howard Schlunder		8/02/06		Rev. 3.75 added beta DNS, NBNS, and HTTP client (GenericTCPClient.c) services
 * Howard Schlunder		12/28/06	Rev. 4.00RC added SMTP, Telnet, substantially modified TCP layer
 * Howard Schlunder		04/09/07	Rev. 4.02 added TCPPerformanceTest, UDPPerformanceTest, Reboot and fixed some bugs
 * Howard Schlunder		xx/xx/07	Rev. 4.03
 * HSchlunder & EWood	08/27/07	Rev. 4.11
 * HSchlunder & EWood	10/08/07	Rev. 4.13
 * HSchlunder & EWood	11/06/07	Rev. 4.16
 * HSchlunder & EWood	11/08/07	Rev. 4.17
 * HSchlunder & EWood	11/12/07	Rev. 4.18
 * HSchlunder & EWood	02/11/08	Rev. 4.19
 * HSchlunder & EWood   04/26/08    Rev. 4.50 Moved most code to other files for clarity
 * KHesky               07/07/08    Added MRF24W-specific support
 * SGustafson           01/30/09    Added LinkMgrII, LinkLib, Console, and iperf.
 ********************************************************************/
/*
 * This macro uniquely defines this file as the main entry point.
 * There should only be one such definition in the entire project,
 * and this file must define the AppConfig variable as described below.
 */
#define THIS_IS_STACK_APPLICATION

// Include all headers for any enabled TCPIP Stack functions
#include "TCPIP Stack/TCPIP.h"

#if defined(STACK_USE_ZEROCONF_LINK_LOCAL)
#include "TCPIP Stack/ZeroconfLinkLocal.h"
#endif
#if defined(STACK_USE_ZEROCONF_MDNS_SD)
#include "TCPIP Stack/ZeroconfMulticastDNS.h"
#endif

/************************ HEADERS **********************************/
#include "MiWi/ConfigApp.h"
#include "WirelessProtocols/Console.h"
#include "Transceivers/Transceivers.h"
#include "WirelessProtocols/SymbolTime.h"
#include "Transceivers/Security.h"
#include "WirelessProtocols/MCHP_API.h"

/************************ VARIABLES ********************************/
#define LIGHT   0x01
#define SWITCH  0x02

/*******************************************************************/
// AdditionalNodeID variable array defines the additional 
// information to identify a device on a P2P connection. This array
// will be transmitted with the P2P_CONNECTION_REQUEST command to 
// initiate the connection between the two devices. Along with the 
// long address of this device, this variable array will be stored 
// in the P2P Connection Entry structure of the partner device. The 
// size of this array is ADDITIONAL_CONNECTION_PAYLOAD, defined in 
// P2PDefs.h.
// In this demo, this variable array is set to be empty.
/******************************************************************/
#if ADDITIONAL_NODE_ID_SIZE > 0
    BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {SWITCH};
#endif

/*******************************************************************/
// The variable myChannel defines the channel that the P2P connection
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available 
// with least amount of energy (or noise).
/*******************************************************************/
BYTE myChannel = 10; 

/*******************************************************************/
// the following two variable arrays are the data to be transmitted
// in this demo. They are bitmap of English word "MiWi" and "DE"
// respectively.
/*******************************************************************/
ROM const BYTE MiWi[5][21] = 
{
    {0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x20,0x20,0xB2,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0xB2,0x20,0x20,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0xB2,0x0D,0x0A}
};   

ROM const BYTE DE[5][11] = 
{
    {0xB2,0xB2,0xB2,0x20,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A},
    {0xB2,0x20,0x20,0xB2,0x20,0xB2,0x20,0x20,0x20,0x0D,0x0A},
    {0xB2,0xB2,0xB2,0x20,0x20,0xB2,0xB2,0xB2,0xB2,0x0D,0x0A}
};  



extern BYTE myLongAddress[];

// Used for Wi-Fi assertions
#define WF_MODULE_NUMBER   WF_MODULE_MAIN_DEMO


#if defined( WF_CONSOLE )
#include "TCPIP Stack/WFConsole.h"
#include "IperfApp.h"
#endif 

// Declare AppConfig structure and some other supporting stack variables
APP_CONFIG AppConfig;
static unsigned short wOriginalAppConfigChecksum;	// Checksum of the ROM defaults for AppConfig
BYTE AN0String[8];

// Use UART2 instead of UART1 for stdout (printf functions).  Explorer 16 
// serial port hardware is on PIC UART2 module.
#if defined(EXPLORER_16) || defined(PIC24FJ256DA210_DEV_BOARD)
	int __C30_UART = 2;
#endif


// Private helper functions.
// These may or may not be present in all applications.
static void InitAppConfig(void);
static void InitializeBoard(void);

#if defined(WF_CS_TRIS)
    static void WF_Connect(void);
#endif

//
// PIC18 Interrupt Service Routines
// 
// NOTE: Several PICs, including the PIC18F4620 revision A3 have a RETFIE FAST/MOVFF bug
// The interruptlow keyword is used to work around the bug when using C18
#if defined(__18CXX)
	#if defined(HI_TECH_C)
	void interrupt low_priority LowISR(void)
	#else
	#pragma interruptlow LowISR
	void LowISR(void)
	#endif
	{
	    TickUpdate();
	}
	
	#if defined(HI_TECH_C)
	void interrupt HighISR(void)
	#else
	#pragma interruptlow HighISR
	void HighISR(void)
	#endif
	{
	    #if defined(STACK_USE_UART2TCP_BRIDGE)
			UART2TCPBridgeISR();
		#endif

		#if defined(WF_CS_TRIS)
			WFEintISR();
		#endif // WF_CS_TRIS
	}

	#if !defined(HI_TECH_C)
	#pragma code lowVector=0x18
	void LowVector(void){_asm goto LowISR _endasm}
	#pragma code highVector=0x8
	void HighVector(void){_asm goto HighISR _endasm}
	#pragma code // Return to default code section
	#endif

// C30 and C32 Exception Handlers
// If your code gets here, you either tried to read or write
// a NULL pointer, or your application overflowed the stack
// by having too many local variables or parameters declared.
#elif defined(__C30__)

    #if defined(STACK_USE_UART)
      #define UART2PrintString    putrsUART
    #else
      #define UART2PrintString(x)
    #endif

    void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void)
    {
      UART2PrintString( "!!! Default interrupt handler !!!\r\n" );
      while (1)
      {
          Nop();
          Nop();
          Nop();
      }
    }

    void __attribute__((interrupt, auto_psv)) _OscillatorFail(void)
    {
      UART2PrintString( "!!! Oscillator Fail interrupt handler !!!\r\n" );
      while (1)
      {
          Nop();
          Nop();
          Nop();
      }
    }
    void __attribute__((interrupt, auto_psv)) _AddressError(void)
    {
      UART2PrintString( "!!! Address Error interrupt handler !!!\r\n" );
      while (1)
      {
          Nop();
          Nop();
          Nop();
      }
    }
    void __attribute__((interrupt, auto_psv)) _StackError(void)
    {
      UART2PrintString( "!!! Stack Error interrupt handler !!!\r\n" );
      while (1)
      {
          Nop();
          Nop();
          Nop();
      }
    }
    void __attribute__((interrupt, auto_psv)) _MathError(void)
    {
      UART2PrintString( "!!! Math Error interrupt handler !!!\r\n" );
      while (1)
      {
          Nop();
          Nop();
          Nop();
      }
    }

#elif defined(__C32__)
	void _general_exception_handler(unsigned cause, unsigned status)
	{
		Nop();
		Nop();
	}
#endif


//
// Main application entry point.
//
#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{

    BYTE i, j;
    BYTE OperatingChannel = 0xFF;
    BYTE TxSynCount = 0;
    BYTE TxSynCount2 = 0;
    MIWI_TICK t1, t2;
    BYTE TxPersistFailures = 0;
    BOOL ReadyToSleep = FALSE;
    BYTE TxNum = 0;
    BYTE RxNum = 0;
    BYTE PressedButton = 0;

	static DWORD t = 0;
	static DWORD dwLastIP = 0;

    #if defined (EZ_CONFIG_STORE)
    static DWORD ButtonPushStart = 0;
    #endif

	// Initialize application specific hardware
	//InitializeBoard();

    BoardInit();   //REVISAR      
    ConsoleInit();  //REVISAR
	WF_HIBERNATE_TRIS = 0;
	WF_HIBERNATE_IO = 1;

    #if defined(PROTOCOL_P2P)
        Printf("\r\nStarting Node 2 of Feature Demo for MiWi(TM) P2P Stack ...");
    #endif
    #if defined(PROTOCOL_MIWI)
        Printf("\r\nStarting Node 2 of Feature Demo for MiWi(TM) Stack ...");
    #endif
    #if defined(PICDEMZ) 
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RB5");
        Printf("\r\n           Button 2: RB4");
        Printf("\r\nOutput Configuration:");
        Printf("\r\n                     RS232 port");
        Printf("\r\n              LED 1: RA0");
        Printf("\r\n              LED 2: RA1");
    #endif
    #if defined(PIC18_EXPLORER) 
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RB0");
        Printf("\r\n           Button 2: RA5");
        Printf("\r\nOutput Configuration:");
        Printf("\r\n                     RS232 port");
        Printf("\r\n                     USB port");
        Printf("\r\n              LED 1: D8");
        Printf("\r\n              LED 2: D7");
    #endif
    #if defined(EIGHT_BIT_WIRELESS_BOARD) 
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RB0");
        Printf("\r\n           Button 2: RB2");
        Printf("\r\nOutput Configuration:");
        Printf("\r\n              LED 1: RA2");
        Printf("\r\n              LED 2: RA3");
    #endif
    #if defined(EXPLORER16) 
        Printf("\r\nInput Configuration:");
        Printf("\r\n           Button 1: RD6");
        Printf("\r\n           Button 2: RD7");
        Printf("\r\nOutput Configuration:");
        Printf("\r\n                     RS232 port");
        Printf("\r\n              LED 1: D10");
        Printf("\r\n              LED 2: D9");
    #endif
    #if defined(MRF24J40)
    Printf("\r\n     RF Transceiver: MRF24J40");
    #elif defined(MRF49XA)
    Printf("\r\n     RF Transceiver: MRF49XA");
    #elif defined(MRF89XA)
    Printf("\r\n     RF Transceiver: MRF89XA");
    #endif
    Printf("\r\n   Demo Instruction:");
    Printf("\r\n                     Power on the board until LED 1 lights up");
    Printf("\r\n                     to indicate it is connected to the peer.");
    Printf("\r\n                     Push Button 1 to broadcast a message. Push");
    Printf("\r\n                     Button 2 to unicast encrypted message on");
    Printf("\r\n                     PICDEM Z or Explorer 16 demo boards. LED 2");
    Printf("\r\n                     will be toggled upon receiving messages.");
    Printf("\r\n\r\n");
    
    LED_1 = 0;
    LED_2 = 0;
    
    
    /*******************************************************************/
    // Following block display demo information on LCD of demo board.
    /*******************************************************************/    
    #if defined(MRF49XA)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Feature P2P Demo MRF49XA Node 2", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Feature MiWi Dem MRF49XA Node 2", 0, TRUE); 
        #endif
    #elif defined(MRF24J40)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Feature P2P DemoMRF24J40 Node 2", 0, TRUE);
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Feature MiWi DemMRF24J40 Node 2", 0, TRUE);
        #endif
    #elif defined(MRF89XA)
        #if defined(PROTOCOL_P2P)
            LCDDisplay((char *)"Feature P2P Demo MRF89XA Node 2", 0, TRUE); 
        #endif
        #if defined(PROTOCOL_MIWI)
            LCDDisplay((char *)"Feature MiWi Dem MRF89XA Node 2", 0, TRUE); 
        #endif
    #endif

    /*******************************************************************/
    // If button 1 on the demo board is pressed and hold during the
    // reset or power on process, Network Freezer feature is invoked
    // to restore all previous network configurations. If Network 
    // Freezer feature is invoked, there is no requirement to re-
    // establish new connection
    /*******************************************************************/
    if( PUSH_BUTTON_1 == 0 )
    {  
        while(PUSH_BUTTON_1 == 0 );
        
        /*********************************************************************/
        // Function MiApp_ProtocolInit intialize the protocol stack.
        // The return value is a boolean to indicate the status of the 
        //      operation.
        // The only parameter indicates if Network Freezer should be invoked.
        //      When Network Freezer feature is invoked, all previous network
        //      configurations will be restored to the states before the 
        //      reset or power cycle
        //
        // In this example, we assume that the user wants to apply Network
        //      Freezer feature and restore the network configuration if
        //      button 1 is pressed and hold when powering up.
        /*********************************************************************/
        MiApp_ProtocolInit(TRUE);

        LCDDisplay((char *)" Network Freezer    ENABLED", 0, TRUE); 
        Printf("\r\nNetwork Freezer Feature is enabled. There will be no hand-shake process.\r\n");

        DumpConnection(0xFF);
    }
    else
    {
        /*********************************************************************/
        // Function MiApp_ProtocolInit intialize the protocol stack.
        // The return value is a boolean to indicate the status of the 
        //      operation.
        // The only parameter indicates if Network Freezer should be invoked.
        //      When Network Freezer feature is invoked, all previous network
        //      configurations will be restored to the states before the 
        //      reset or power cycle
        //
        // In this example, we assume that the user wants to start from
        // scratch and ignore previous network configuration if button 1
        // is released when powering up.
        /*********************************************************************/
        MiApp_ProtocolInit(FALSE);   
        
        #ifdef ENABLE_ACTIVE_SCAN
        
            ConsolePutROMString((ROM char*)"\r\nStarting Active Scan...");    
            
            LCDDisplay((char *)"Active Scanning", 0, FALSE);
            
            while(1)
            {
                /*********************************************************************/
                // Function MiApp_SearchConnection will return the number of existing 
                // connections in all channels. It will help to decide which channel 
                // to operate on and which connection to add
                // The return value is the number of connections. The connection data
                //     are stored in global variable ActiveScanResults. Maximum active
                //     scan result is defined as ACTIVE_SCAN_RESULT_SIZE
                // The first parameter is the scan duration, which has the same 
                //     definition in Energy Scan. 10 is roughly 1 second. 9 is a half 
                //     second and 11 is 2 seconds. Maximum scan duration is 14, or 
                //     roughly 16 seconds.
                // The second parameter is the channel map. Bit 0 of the double
                //     word parameter represents channel 0. For the 2.4GHz frequency
                //     band, all possible channels are channel 11 to channel 26.
                //     As the result, the bit map is 0x07FFF800. Stack will filter
                //     out all invalid channels, so the application only needs to pay
                //     attention to the channels that are not preferred.
                /*********************************************************************/
                j = MiApp_SearchConnection(10, 0xFFFFFFFF);
                
                if( j > 0 )
                {
                    // now print out the scan result.
                    Printf("\r\nActive Scan Results: \r\n");
                    for(i = 0; i < j; i++)
                    {
                        Printf("Channel: ");
                        PrintDec(ActiveScanResults[i].Channel);
                        Printf("   RSSI: ");
                        PrintChar(ActiveScanResults[i].RSSIValue);
                        #if defined(IEEE_802_15_4)
                            #if ADDITIONAL_NODE_ID_SIZE > 0
                                Printf("   PeerInfo: ");
                                for(j = 0; j < ADDITIONAL_NODE_ID_SIZE; j++)
                                {
                                    PrintChar(ActiveScanResults[i].PeerInfo[j]);
                                }
                            #endif
                            Printf("    PANID: ");
                            PrintChar(ActiveScanResults[i].PANID.v[1]);
                            PrintChar(ActiveScanResults[i].PANID.v[0]);
                            Printf("\r\n");
                        #endif
                        OperatingChannel = ActiveScanResults[i].Channel;
                    }
                }
            
                if( OperatingChannel != 0xFF )
                {
                    /*******************************************************************/
                    // Function MiApp_SetChannel assign the operation channel(frequency)
                    // for the transceiver. Channels 0 - 31 has been defined for the 
                    // wireless protocols, but not all channels are valid for all 
                    // transceivers, depending on their hardware design, operating
                    // frequency band, data rate and other RF parameters
                    /*******************************************************************/
                    MiApp_SetChannel(OperatingChannel);
                    break;
                }
                
                Printf("\r\nNo Suitable PAN, Rescanning...");
            }
        #endif
        
        
        
        /*********************************************************************/
        // Function MiApp_ConnectionMode sets the connection mode for the 
        // protocol stack. Possible connection modes are:
        //  - ENABLE_ALL_CONN       accept all connection request
        //  - ENABLE_PREV_CONN      accept only known device to connect
        //  - ENABL_ACTIVE_SCAN_RSP do not accept connection request, but allow
        //                          response to active scan
        //  - DISABLE_ALL_CONN      disable all connection request, including
        //                          active scan request
        /*********************************************************************/
        MiApp_ConnectionMode(ENABLE_ALL_CONN);
        
        /*******************************************************************/
        // Function MiApp_EstablishConnection establish connections between
        // two devices. It has two input parameters:
        // The first parameter is the index of the target device in the 
        //     active scan table. It requires a MiApp_SearchConnection call
        //     before hand. If seraching connection is not performed in 
        //     advance, user can apply 0xFF to the first parameter to 
        //     indicate that it is OK to establish connection with any 
        //     device.
        // The second parameter is the connection mode, either directly or
        //     indirectly. Direct connection is a connection in the radio
        //     range. All protocol stack support this connection mode. 
        //     Indirect connection is the connection out of radio range. 
        //     An indirect connection has to rely on other device to route 
        //     the messages between two connected devices. Indirect 
        //     connection is also called "Socket" connection in MiWi 
        //     Protocol. Since MiWi P2P protocol only handles connection 
        //     of one hop, indirect connection is not supported in MiWi 
        //     P2P protocol, but supported in other networking protocols.
        // Function MiApp_EstablishConnection returns the index of the 
        //     connected device in the connection table. If no connection 
        //     is established after predefined retry times 
        //     CONNECTION_RETRY_TIMES, it will return 0xFF. If multiple 
        //     connections have been established, it will return the one 
        //     of the indexes of the connected device.
        /*******************************************************************/
        i = MiApp_EstablishConnection(0, CONN_MODE_DIRECT);
        Printf("\r\nConnection Created");
        
        /*******************************************************************/
        // Display current opertion on LCD of demo board, if applicable
        /*******************************************************************/
        LCDDisplay((char *)" Connected Peer  on Channel %d", currentChannel, TRUE);

        
        /*******************************************************************/
        // Function DumpConnection is used to print out the content of the
        // P2P Connection Entry on the hyperterminal. It may be useful in 
        // the debugging phase.
        // The only parameter of this function is the index of the P2P 
        // Connection Entry. The value of 0xFF means to print out all
        // valid P2P Connection Entry; otherwise, the P2P Connection Entry
        // of the input index will be printed out.
        /*******************************************************************/
        #ifdef ENABLE_DUMP
            DumpConnection(0xFF);
        #endif
    }
    
    // Turn on LED 1 to indicate P2P connection established
    LED_1 = 1;

    /*******************************************************************/
    // Following block display demo instructions on LCD based on the
    // demo board used. 
    /*******************************************************************/
    #if defined(EXPLORER16)
        LCDDisplay((char *)"RD6: Broadcast  RD7: Unicast", 0, FALSE); 
    #elif defined(PIC18_EXPLORER)
        LCDDisplay((char *)"RB0: Broadcast", 0, FALSE); 
    #elif defined(EIGHT_BIT_WIRELESS_BOARD)
        LCDDisplay((char *)"RB0: Broadcast  RB2: Unicast", 0, FALSE);
    #endif

    while(1)
    {   
        /*******************************************************************/
        // Function MiApp_MessageAvailable will return a boolean to indicate 
        // if a message for application layer has been received by the 
        // transceiver. If a message has been received, all information will 
        // be stored in rxMessage, structure of RECEIVED_MESSAGE.
        /*******************************************************************/
        if( MiApp_MessageAvailable() )
        {
            /*******************************************************************/
            // If a packet has been received, following code prints out some of
            // the information available in rxMessage.
            /*******************************************************************/
            #ifdef ENABLE_SECURITY
                if( rxMessage.flags.bits.secEn )
                {
                    ConsolePutROMString((ROM char *)"Secured ");
                }
            #endif
            #ifndef TARGET_SMALL
                if( rxMessage.flags.bits.broadcast )
                {
                    ConsolePutROMString((ROM char *)"Broadcast Packet with RSSI ");
                }
                else
                {
                    ConsolePutROMString((ROM char *)"Unicast Packet with RSSI ");
                }
                PrintChar(rxMessage.PacketRSSI);
                if( rxMessage.flags.bits.srcPrsnt )
                {
                    ConsolePutROMString((ROM char *)" from ");
                    if( rxMessage.flags.bits.altSrcAddr )
                    {
                        PrintChar( rxMessage.SourceAddress[1] );
                        PrintChar( rxMessage.SourceAddress[0] );
                    }
                    else
                    {    
                        for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                        {
                            PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH-1-i]);
                        }
                    }    
                }
            #endif
            ConsolePutROMString((ROM char *)": ");

            for(i = 0; i < rxMessage.PayloadSize; i++)
            {
                ConsolePut(rxMessage.Payload[i]);
            }
            
            // Toggle LED2 to indicate receiving a packet.
            LED_2 ^= 1;

            /*******************************************************************/
            // Function MiApp_DiscardMessage is used to release the current 
            // received message. After calling this function, the stack can 
            // start to process the next received message.
            /*******************************************************************/ 
            MiApp_DiscardMessage();
            
            /*******************************************************************/
            // Following block update the total received and transmitted 
            // messages on the LCD of the demo board. 
            /*******************************************************************/
            LCDTRXCount(TxNum, ++RxNum);
        }
        else 
        {
            #ifdef ENABLE_FREQUENCY_AGILITY
                /***********************************************************************/
                // TxPersistFailures is the local variable to track the transmission 
                // failure because no acknowledgement frame is received. Typically,
                // this is the indication of either very strong noise, or the PAN
                // has hopped to another channel. 
                /***********************************************************************/
                if( TxPersistFailures > 3 )
                {
                    /***********************************************************************/
                    // Display the resynchronization message on LCD of demo board, if 
                    // applicable 
                    /***********************************************************************/
                    LCDDisplay((char *)"Resynchronizing the Connection", 0, FALSE); 
                    ConsolePutROMString((ROM char*)"\r\nResynchronizing the Connection...");
                    
                    /*******************************************************************/
                    // Function MiApp_TransceiverPowerState is used to set the power state
                    // of RF transceiver. There are three possible states:
                    //   - POWER_STATE_SLEEP        Put transceiver into sleep
                    //   - POWER_STATE_WAKEUP       Wake up the transceiver only
                    //   - POWER_STATE_WAKEUP_DR    Wake up and send Data Request command
                    /*******************************************************************/
                    MiApp_TransceiverPowerState(POWER_STATE_WAKEUP);
                    
                    /***********************************************************************/
                    // Function MiApp_ResyncConnection is used to synchronized connection
                    // if one side of communication jumped to another channel, when 
                    // frequency agility is performed. Usually, this is done by the 
                    // sleeping device, since the sleeping device cannot hear the broadcast 
                    // of channel hopping command.
                    //
                    // The first parameter is the index of connection table for the peer 
                    // node, which we would like to resynchronize to.
                    // The second parameter is the bit map of channels to be scanned
                    /***********************************************************************/
                    MiApp_ResyncConnection(0, 0xFFFFFFFF);    
                    TxPersistFailures = 0;
                    ReadyToSleep = FALSE;
                    /***********************************************************************/
                    // Display the resynchronization result on the LCD of demo board, if 
                    // applicable
                    /***********************************************************************/
                    LCDDisplay((char *)" Resynchronized  to Channel %d", currentChannel, FALSE);
                }
                else
                {
                    #ifdef ENABLE_SLEEP
                        ReadyToSleep = TRUE;
                    #endif
                }    
            
            #endif
            
            #ifdef ENABLE_SLEEP
                /*******************************************************************/
                // If Data Request command and data transmision has been handled,
                // as the RFD device, it is time to consider put both the radio and 
                // MCU into sleep mode to conserve power.
                /*******************************************************************/
                if( ReadyToSleep )
                {   
                    #if defined(ENABLE_TIME_SYNC)
                        DWORD_VAL    delayTick;
                    #endif
                    ReadyToSleep = FALSE;
                    
                    /*******************************************************************/
                    // Function MiApp_TransceiverPowerState is used to set the power 
                    // state of RF transceiver. There are three possible states:
                    //   - POWER_STATE_SLEEP        Put transceiver into sleep
                    //   - POWER_STATE_WAKEUP       Wake up the transceiver only
                    //   - POWER_STATE_WAKEUP_DR    Wake up and send Data Request 
                    //                              command
                    /*******************************************************************/
                    MiApp_TransceiverPowerState(POWER_STATE_SLEEP);
                                        
                    /*******************************************************************/
                    // Prepare the condition to wake up the MCU. The MCU can either be
                    // waken up by the timeout of watch dog timer (Time Synchronization 
                    // off), timeout of counter with external 32KHz crystal (Time 
                    // Synchronization on), or by the pin change notification interrupt 
                    // by pushing the button.
                    /*******************************************************************/
                    #if defined(ENABLE_TIME_SYNC)
                        /******************************************************************/
                        // When Time Synchronization feature is enabled, a counter with 
                        // external 32KHz crystal will be used to keep the precise timing
                        // when the MCU is in sleep. we need to update the counter with 
                        // proper value. The timeout of the counter will wake up the MCU.
                        /******************************************************************/
                        
                        /******************************************************************/
                        // Following block is used to update the CounterValue with more
                        // precise timing control. This block is useful if very precise 
                        // timing is required, such as time slot is very short (<< 1 second)
                        // or application spends considerable, unpredictable time performing
                        // other operations
                        /******************************************************************/
                        #if 1
                            delayTick.Val = 0;
                            
                            #if defined(__18CXX)
                                delayTick.v[0] = TMR3L;
                                delayTick.v[1] = TMR3H;
                            #elif defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) ||defined(__PIC24H__)
                                delayTick.w[0] = TMR1;
                            #elif defined(__PIC32MX__)
                                delayTick.w[0] = TMR1;
                            #endif
                            delayTick.Val += CounterValue.Val;
                            if( delayTick.v[2] > 0 )
                            {
                                if( WakeupTimes.Val > 0 )
                                {
                                    WakeupTimes.Val--;
                                }
                                else
                                {
                                    delayTick.Val = CounterValue.Val;
                                }    
                            }
                            CounterValue.v[0] = delayTick.v[0];
                            CounterValue.v[1] = delayTick.v[1];   
                        #endif 
                        
                        /******************************************************************/
                        // The total sleeping time of the device is controlled by two 16bit
                        // parameters: WakeupTimes and CouterValue. Both values are referred
                        // from the protocol stack.
                        // The counter with external 32KHz crystal can provide wakeup time 
                        // up to around 16 seconds. 
                        // If the intended sleep period is less than 16 second, only 
                        // CounterValue will be loaded to the counter, the device
                        // will wake up by the counter timeout. 
                        // If the intended sleep period is longer than 16 seconds, 
                        // WakeupTimes will store the total number of 16-second wakeup and 
                        // the rest remaining time will be loaded into CounterValue. When
                        // the counter with external 32KHz crystal timeout and wake up the
                        // MCU, the MCU will reduce WakeupTimes by one and keep the radio
                        // in sleep. When the WakeupTimes are reduced to be zero, 
                        // CounterValue will be loaded to the counter. The whole device
                        // will wake up once the counter timeout this time.
                        /******************************************************************/
                        while(1)
                        {
                            #if defined(__18CXX)
                                if( WakeupTimes.Val > 0 )
                                {
                                    LED_2 = 0;
                                    TMR3H = 0;
                                    TMR3L = 0;
                                }    
                                else
                                {
                                    LED_2 = 1;
                                    
                                    TMR3H = CounterValue.v[1];
                                    TMR3L = CounterValue.v[0];
                                }    
                                PIR2bits.TMR3IF = 0;
                                while( !ConsoleIsPutReady() );
                                #if defined(PIC18_EXPLORER)
                                    INTCONbits.INT0IF = 0;
                                    INTCON2bits.INTEDG0 = 0;
                                    INTCONbits.INT0IE = 1;
                                #elif defined(EIGHT_BIT_WIRELESS_BOARD)
                                    INTCONbits.INT0IF = 0;
                                    INTCON2bits.INTEDG0 = 0;
                                    INTCONbits.INT0IE = 1;
                                    INTCON3bits.INT2IF = 0;
                                    INTCON2bits.INTEDG2 = 0;
                                    INTCON3bits.INT2IE = 1;
                                #elif defined(PICDEMZ)
                                    INTCONbits.RBIF = 0;        // clear pin change notification interrupt
                                    INTCONbits.RBIE = 1;        // enable pin change notification interrupt
                                #endif
                            #elif defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__)
                                
                                if( WakeupTimes.Val > 0 )
                                {
                                    LED_2 = 0;
                                    TMR1 = 0;
                                }    
                                else
                                {
                                    LED_2 = 1;
                                    
                                    TMR1 = CounterValue.Val;
                                }  
                                IFS0bits.T1IF = 0;

                                IFS1bits.CNIF = 0;
                                IEC1bits.CNIE = 1;          // enable pin change notification interrupt
                            #elif defined(__PIC32MX__)
                                if( WakeupTimes.Val > 0 )
                                {
                                    LED_2 = 0;
                                    TMR1 = 0;
                                }    
                                else
                                {
                                    LED_2 = 1;
                                    while(T1CONbits.TWIP) ;
                                    TMR1 = CounterValue.Val;
                                }  
                                IFS0bits.T1IF = 0;
                              
                                Enable_PB_1_2_Interrupts(); // enable pin change notification interrupt
                            #endif
                            
                            // make sure UART has nothing to output
                            while(ConsoleIsPutReady() == 0);
                    
                            // Put MCU into sleep mode
                            Sleep();
                            
                            #if defined(__18CXX)
                                #if defined(PIC18_EXPLORER)
                                    INTCONbits.INT0IE = 0;
                                #elif defined(EIGHT_BIT_WIRELESS_BOARD)
                                    INTCONbits.INT0IE = 0;
                                    INTCON3bits.INT2IE = 0;    
                                #elif defined(PICDEMZ)
                                    INTCONbits.RBIE = 0;        // disable pin change notification interrupt
                                #endif
                            #elif defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__)
                                //IEC1bits.CNIE = 0;          // disable pin change notification interrupt
                            #elif defined(__PIC32MX__)
                                RCONCLR = RCON_SLEEP_MASK | RCON_IDLE_MASK;   // clear the Sleep and Idle bits
                            #endif
                            
                            if( WakeupTimes.Val == 0 )
                            {
                                break;
                            }    
                            WakeupTimes.Val--;
                        } 
                    
                    #else
                    
                        /******************************************************************/
                        // When Time Synchronization is not enabled, watch dog timer is 
                        // used to wake up the device periodically.
                        /******************************************************************/
                        #if defined(__18CXX)
                            ClrWdt();
                            WDTCONbits.SWDTEN = 1;      // enable watch dog timer
                            #if defined(PIC18_EXPLORER)
                                INTCONbits.INT0IF = 0;
                                INTCON2bits.INTEDG0 = 0;
                                INTCONbits.INT0IE = 1;
                            #elif defined(EIGHT_BIT_WIRELESS_BOARD)
                                INTCONbits.INT0IF = 0;
                                INTCON2bits.INTEDG0 = 0;
                                INTCONbits.INT0IE = 1;
                                INTCON3bits.INT2IF = 0;
                                INTCON2bits.INTEDG2 = 0;
                                INTCON3bits.INT2IE = 1;
                            #elif defined(PICDEMZ)
                                INTCONbits.RBIF = 0;        // clear pin change notification interrupt
                                INTCONbits.RBIE = 1;        // enable pin change notification interrupt
                            #endif
                        #elif defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__)
                            ClrWdt();
                            RCONbits.SWDTEN = 1;        // enable watch dog timer
                            IFS1bits.CNIF = 0;
                            IEC1bits.CNIE = 1;          // enable pin change notification interrupt
                        #elif defined(__PIC32MX__)
                            Enable_PB_1_2_Interrupts(); // enable pin change notification interrupt
                            EnableWDT();                // enable watch dog timer
                            ClearWDT();                 // clear watch dog timer
                        #endif
                        
                        // make sure UART has nothing to output
                        while(ConsoleIsPutReady() == 0);
                        
                        // Put MCU into sleep mode
                        Sleep();
                        
                        #if defined(__18CXX)
                            #if defined(PIC18_EXPLORER)
                                INTCONbits.INT0IE = 0;
                            #elif defined(EIGHT_BIT_WIRELESS_BOARD)
                                INTCONbits.INT0IE = 0;
                                INTCON3bits.INT2IE = 0;    
                            #elif defined(PICDEMZ)
                                INTCONbits.RBIE = 0;        // disable pin change notification interrupt
                            #endif
                            WDTCONbits.SWDTEN = 0;      // disable watch dog timer
                        #elif defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__)
                            RCONbits.SWDTEN = 0;        // disable watch dog timer                           
                        #elif defined(__PIC32MX__)
                            RCONCLR = RCON_SLEEP_MASK | RCON_IDLE_MASK;   // clear the Sleep and Idle bits
                            DisableWDT();
                        #endif
                    #endif

                    
                    /*******************************************************************/
                    // Function ButtonPressed will return if any of the two buttons
                    // (RB5 and RB4 on PICDEM Z or RD6 and RD7 on Explorer 16) has been
                    // pushed.
                    /*******************************************************************/
                    PressedButton = ButtonPressed();   

                    #if defined(ENABLE_ENHANCED_DATA_REQUEST) 
                        /*******************************************************************/
                        // The handling of Enhanced Data Request feature depends on when
                        // to send out message. If Enhanced Data Request feature is enabled,
                        // The message should be sent out before transceiver wakes up and
                        // sends out the Data Reqeust. Otherwise, the message should be 
                        // sent after transceiver wakes up and sends out the Data Reqeust 
                        // command.
                        // When Enhanced Data Request feature is enabled, only unicast 
                        // message can be sent with Data Request. Broadcast, however, has 
                        // a different destination address, thus must be sent after the 
                        // normal Data Request procedure in the normal way. 
                        /*******************************************************************/         
                        switch( PressedButton )
                        {
                            case 1: 
                                /*******************************************************************/                
                                // Button 1 pressed
                                // First use MiApp_FlushTx to reset the Transmit buffer. Then fill 
                                // the TX buffer by calling function MiApp_WriteData
                                /*******************************************************************/
                                MiApp_FlushTx();
                                if( (TxSynCount % 6 ) == 0 )
                                {
                                    MiApp_WriteData(0x0D);
                                    MiApp_WriteData(0x0A);
                                }
                                else
                                {                     
                                    for(i = 0; i < 21; i++)
                                    {
                                        MiApp_WriteData(MiWi[(TxSynCount%6)-1][i]);
                                    }
                                }
                                TxSynCount++;
                                
                                /*******************************************************************/
                                // Function MiApp_UnicastConnection is one of the functions to 
                                // unicast a message.
                                //    The first parameter is the index of connection table for 
                                //       the peer device. In this demo, since there are only two
                                //       devices involved, the peer device must be stored in the 
                                //       first P2P Connection Entry in the connection table.
                                //    The second parameter is the boolean to indicate if we need 
                                //       to secure the frame. If encryption is applied, the 
                                //       security level and security key are defined in the 
                                //       configuration file for the transceiver
                                //
                                // Another way to unicast a message is by calling function
                                // MiApp_UnicastAddress. Instead of supplying the index of the 
                                // connection table of the peer device, this function requires 
                                // the input parameter of destination address directly.
                                /*******************************************************************/
                                if( MiApp_UnicastConnection(0, FALSE) == FALSE )
                                {
                                    Printf("\r\nUnicast Failed\r\n");
                                    /*******************************************************************/
                                    // Following block display error message on LCD of demo board.
                                    /*******************************************************************/
                                    LCDDisplay((char *)" Unicast Failed", 0, TRUE);
                                }
                                else
                                {
                                    TxNum++;
                                }
                                
                                /*******************************************************************/
                                // Following block update the total received and transmitted messages
                                // on the LCD of the demo board. 
                                /*******************************************************************/
                                LCDTRXCount(TxNum, RxNum);   

                                break;
              
                            case 2:
                                /*******************************************************************/                
                                // Button 2 pressed.
                                // First use MiApp_FlushTx to reset the Transmit buffer. Then fill 
                                // the TX buffer by calling function MiApp_WriteData
                                /*******************************************************************/
                                MiApp_FlushTx();
                                if( (TxSynCount2 % 6 ) == 0 )
                                {
                                    MiApp_WriteData(0x0D);
                                    MiApp_WriteData(0x0A);
                                }
                                else
                                {                     
                                    for(i = 0; i < 11; i++)
                                    {
                                        MiApp_WriteData(DE[(TxSynCount2%6)-1][i]);
                                    }
                                }
                                TxSynCount2++;
                                
                                /*******************************************************************/
                                // Function MiApp_UnicastConnection is one of the functions to 
                                // unicast a message.
                                //    The first parameter is the index of connection table for 
                                //       the peer device. In this demo, since there are only two
                                //       devices involved, the peer device must be stored in the 
                                //       first P2P Connection Entry in the connection table.
                                //    The second parameter is the boolean to indicate if we need 
                                //       to secure the frame. If encryption is applied, the 
                                //       security level and security key are defined in the 
                                //       configuration file for the transceiver
                                //
                                // Another way to unicast a message is by calling function
                                // MiApp_UnicastAddress. Instead of supplying the index of the 
                                // connection table of the peer device, this function requires 
                                // the input parameter of destination address directly.
                                /*******************************************************************/
                                if( MiApp_UnicastConnection(0, TRUE) == FALSE )
                                {
                                    Printf("\r\nUnicast Failed\r\n");
                                    /*******************************************************************/
                                    // Following block display error message on LCD of demo board.
                                    /*******************************************************************/
                                    LCDDisplay((char *)" Unicast Failed", 0, TRUE);
                                }
                                else
                                {
                                    TxNum++;
                                }
                                
                                /*******************************************************************/
                                // Following block update the total received and transmitted messages
                                // on the LCD of the demo board. 
                                /*******************************************************************/
                                LCDTRXCount(TxNum, RxNum);
                                break;
                                
                            default:
                                MiApp_FlushTx();
                                break;
                        }
   
                    #endif
                    
                    /*******************************************************************/
                    // Function MiApp_TransceiverPowerState is used to set the power 
                    // state of RF transceiver. There are three possible states:
                    //   - POWER_STATE_SLEEP        Put transceiver into sleep
                    //   - POWER_STATE_WAKEUP       Wake up the transceiver only
                    //   - POWER_STATE_WAKEUP_DR    Wake up and send Data Request 
                    //                              command
                    /*******************************************************************/
                    if( MiApp_TransceiverPowerState(POWER_STATE_WAKEUP_DR) > SUCCESS )
                    {
                        Printf("\r\nData Request Failed");
                        TxPersistFailures++;
                        LCDDisplay((char *)"Connection Lost     %d Times", TxPersistFailures, FALSE);
                    }
                    else
                    {
                        if( TxPersistFailures > 0 )
                        {
                            LCDTRXCount(TxNum, RxNum);
                        }
                        TxPersistFailures = 0;
                        
                    }
                    
                    #if !defined(ENABLE_ENHANCED_DATA_REQUEST)
                        /*******************************************************************/
                        // The handling of Enhanced Data Request feature depends on when
                        // to send out message. If Enhanced Data Request feature is enabled,
                        // The message should be sent out before transceiver wakes up and
                        // sends out the Data Reqeust. Otherwise, the message should be 
                        // sent after transceiver wakes up and sends out the Data Reqeust 
                        // command.
                        /*******************************************************************/
                        switch( PressedButton )
                        {
                            case 1: 
                                /*******************************************************************/                
                                // Button 1 pressed.
                                // First use MiApp_FlushTx to reset the Transmit buffer. Then fill 
                                // the TX buffer by calling function MiApp_WriteData
                                /*******************************************************************/
                                MiApp_FlushTx();
                                if( (TxSynCount % 6 ) == 0 )
                                {
                                    // insert an empty line for every complete bitmap
                                    MiApp_WriteData(0x0D);
                                    MiApp_WriteData(0x0A);
                                }
                                else
                                {                     
                                    for(i = 0; i < 21; i++)
                                    {
                                        MiApp_WriteData(MiWi[(TxSynCount%6)-1][i]);
                                    }
                                }
                                TxSynCount++;
                                
                                /*******************************************************************/
                                // Function MiApp_BroadcastPacket is used to broadcast a message
                                //    The only parameter is the boolean to indicate if we need to
                                //    secure the message
                                /*******************************************************************/
                                MiApp_BroadcastPacket(FALSE);
                                
                                /*******************************************************************/
                                // Following block update the total received and transmitted 
                                // messages on the LCD of the demo board. 
                                /*******************************************************************/
                                LCDTRXCount(++TxNum, RxNum);   
                                break;
              
                            case 2:
                                /*******************************************************************/                
                                // Button 2 pressed.
                                // First use MiApp_FlushTx to reset the Transmit buffer. Then fill 
                                // the TX buffer by calling function MiApp_WriteData
                                /*******************************************************************/
                                MiApp_FlushTx();
                                if( (TxSynCount2 % 6 ) == 0 )
                                {
                                    // insert an empty line for every complete bitmap
                                    MiApp_WriteData(0x0D);
                                    MiApp_WriteData(0x0A);
                                }
                                else
                                {                     
                                    for(i = 0; i < 11; i++)
                                    {
                                        MiApp_WriteData(DE[(TxSynCount2%6)-1][i]);
                                    }
                                }
                                TxSynCount2++;
                                
                                /*******************************************************************/
                                // Function MiApp_UnicastConnection is one of the functions to 
                                // unicast a message.
                                //    The first parameter is the index of connection table for 
                                //       the peer device. In this demo, since there are only two
                                //       devices involved, the peer device must be stored in the 
                                //       first P2P Connection Entry in the connection table.
                                //    The second parameter is the boolean to indicate if we need 
                                //       to secure the frame. If encryption is applied, the 
                                //       security level and security key are defined in the 
                                //       configuration file for the transceiver
                                //
                                // Another way to unicast a message is by calling function
                                // MiApp_UnicastAddress. Instead of supplying the index of the 
                                // connection table of the peer device, this function requires the 
                                // input parameter of destination address directly.
                                /*******************************************************************/
                                if( MiApp_UnicastConnection(0, TRUE) == FALSE )
                                {
                                    Printf("\r\nUnicast Failed\r\n");
                                    /*******************************************************************/
                                    // Following block display error message on LCD of demo board.
                                    /*******************************************************************/
                                    LCDDisplay((char *)" Unicast Failed", 0, TRUE);
                                }
                                else
                                {
                                    TxNum++;
                                }
                                
                                /*******************************************************************/
                                // Following block update the total received and transmitted 
                                // messages on the LCD of the demo board. 
                                /*******************************************************************/
                                LCDTRXCount(TxNum, RxNum);
                                break;
                                
                            default:
                                break;
                        }

                    #endif

                }
            #endif   
             
        }
    }
}

#if defined(WF_CS_TRIS)
/*****************************************************************************
 * FUNCTION: WF_Connect
 *
 * RETURNS:  None
 *
 * PARAMS:   None
 *
 *  NOTES:   Connects to an 802.11 network.  Customize this function as needed 
 *           for your application.
 *****************************************************************************/
static void WF_Connect(void)
{
    UINT8 ConnectionProfileID;
    UINT8 channelList[] = MY_DEFAULT_CHANNEL_LIST;
    #if defined(WF_USE_POWER_SAVE_FUNCTIONS)
    BOOL  PsPollEnabled;
    #endif
    
    /* create a Connection Profile */
    WF_CPCreate(&ConnectionProfileID);

    #if defined(STACK_USE_UART)
    putrsUART("Set SSID (");
    putsUART(AppConfig.MySSID);
    putrsUART(")\r\n");
    #endif
    WF_CPSetSsid(ConnectionProfileID, 
                 AppConfig.MySSID, 
                 AppConfig.SsidLength);

    #if defined(STACK_USE_UART)
    putrsUART("Set Network Type\r\n");
	#endif
    WF_CPSetNetworkType(ConnectionProfileID, AppConfig.networkType);

    if (AppConfig.networkType == WF_ADHOC)
    {
        WF_CPSetAdHocBehavior(ConnectionProfileID, WF_ADHOC_CONNECT_THEN_START);
    }
    
    putrsUART("Set Security\r\n");
    switch(AppConfig.SecurityMode) {
        case WF_SECURITY_OPEN:
            WF_CPSetSecurity(ConnectionProfileID, WF_SECURITY_OPEN, 0, NULL, 0);
            break;
        case WF_SECURITY_WEP_40:
            // assume key 0
            WF_CPSetSecurity(ConnectionProfileID, AppConfig.SecurityMode, 0, AppConfig.SecurityKey, 5);
            break;
        case WF_SECURITY_WEP_104:
            // assume key 0
            WF_CPSetSecurity(ConnectionProfileID, AppConfig.SecurityMode, 0, AppConfig.SecurityKey, 13);
            break;
        case WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:
            WF_CPSetSecurity(ConnectionProfileID, WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE, 
                             0, AppConfig.SecurityKey, strlen((char*)AppConfig.SecurityKey));
            break;
        case WF_SECURITY_WPA_AUTO_WITH_KEY:
            WF_CPSetSecurity(ConnectionProfileID, WF_SECURITY_WPA_AUTO_WITH_KEY,
                             0, AppConfig.SecurityKey, 32);
            break;
        default:
			putrsUART("\r\n\r\nCaptain this should NOT happen.\r\n\r\n");

    }
        
	#if defined(STACK_USE_UART)
	putrsUART("Set Scan Type\r\n");
	#endif
    WF_CASetScanType(MY_DEFAULT_SCAN_TYPE);
    
    #if defined(STACK_USE_UART)
    putrsUART("Set Channel List\r\n");
    #endif    
    WF_CASetChannelList(channelList, sizeof(channelList));
    
    #if defined(STACK_USE_UART)
    putrsUART("Set list retry count\r\n");
    #endif
    WF_CASetListRetryCount(MY_DEFAULT_LIST_RETRY_COUNT);

    #if defined(STACK_USE_UART)        
    putrsUART("Set Event Notify\r\n");    
    #endif
    WF_CASetEventNotificationAction(MY_DEFAULT_EVENT_NOTIFICATION_LIST);
    
#if defined(WF_USE_POWER_SAVE_FUNCTIONS)
    PsPollEnabled = (MY_DEFAULT_PS_POLL == WF_ENABLED);
    if (!PsPollEnabled)
    {    
        /* disable low power (PS-Poll) mode */
        #if defined(STACK_USE_UART)
        putrsUART("Disable PS-Poll\r\n");        
        #endif
        WF_PsPollDisable();
    }    
    else
    {
        /* Enable low power (PS-Poll) mode */
        #if defined(STACK_USE_UART)
        putrsUART("Enable PS-Poll\r\n");        
        #endif
        WF_PsPollEnable(TRUE);
    }    
#endif

    #if defined(STACK_USE_UART)
    putrsUART("Set Beacon Timeout\r\n");
    #endif
    WF_CASetBeaconTimeout(40);


    #if defined(STACK_USE_UART)                     
    putrsUART("Start WiFi Connect\r\n");        
    #endif
    WF_CMConnect(ConnectionProfileID);
}   
#endif /* WF_CS_TRIS */

// Writes an IP address to the LCD display and the UART as available
void DisplayIPValue(IP_ADDR IPVal)
{
//	printf("%u.%u.%u.%u", IPVal.v[0], IPVal.v[1], IPVal.v[2], IPVal.v[3]);
    BYTE IPDigit[4];
	BYTE i;
#ifdef USE_LCD
	BYTE j;
	BYTE LCDPos=16;
#endif

	for(i = 0; i < sizeof(IP_ADDR); i++)
	{
	    uitoa((WORD)IPVal.v[i], IPDigit);

		#if defined(STACK_USE_UART)
			putsUART((char *) IPDigit);
		#endif

		#ifdef USE_LCD
			for(j = 0; j < strlen((char*)IPDigit); j++)
			{
				LCDText[LCDPos++] = IPDigit[j];
			}
			if(i == sizeof(IP_ADDR)-1)
				break;
			LCDText[LCDPos++] = '.';
		#else
			if(i == sizeof(IP_ADDR)-1)
				break;
		#endif

		#if defined(STACK_USE_UART)
			while(BusyUART());
			WriteUART('.');
		#endif
	}

	#ifdef USE_LCD
		if(LCDPos < 32u)
			LCDText[LCDPos] = 0;
		LCDUpdate();
	#endif
}

/****************************************************************************
  Function:
    static void InitializeBoard(void)

  Description:
    This routine initializes the hardware.  It is a generic initialization
    routine for many of the Microchip development boards, using definitions
    in HardwareProfile.h to determine specific initialization.

  Precondition:
    None

  Parameters:
    None - None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
static void InitializeBoard(void)
{	
	// LEDs
	LED0_TRIS = 0;
	LED1_TRIS = 0;
	LED2_TRIS = 0;
	LED3_TRIS = 0;
	LED4_TRIS = 0;
	LED5_TRIS = 0;
	LED6_TRIS = 0;
	LED7_TRIS = 0;
	LED_PUT(0x00);

#if defined(__18CXX)
	// Enable 4x/5x/96MHz PLL on PIC18F87J10, PIC18F97J60, PIC18F87J50, etc.
    OSCTUNE = 0x40;

	// Set up analog features of PORTA

	// PICDEM.net 2 board has POT on AN2, Temp Sensor on AN3
	#if defined(PICDEMNET2)
		ADCON0 = 0x09;		// ADON, Channel 2
		ADCON1 = 0x0B;		// Vdd/Vss is +/-REF, AN0, AN1, AN2, AN3 are analog
	#elif defined(PICDEMZ)
		ADCON0 = 0x81;		// ADON, Channel 0, Fosc/32
		ADCON1 = 0x0F;		// Vdd/Vss is +/-REF, AN0, AN1, AN2, AN3 are all digital
	#elif defined(__18F87J11) || defined(_18F87J11) || defined(__18F87J50) || defined(_18F87J50)
		ADCON0 = 0x01;		// ADON, Channel 0, Vdd/Vss is +/-REF
		WDTCONbits.ADSHR = 1;
		ANCON0 = 0xFC;		// AN0 (POT) and AN1 (temp sensor) are anlog
		ANCON1 = 0xFF;
		WDTCONbits.ADSHR = 0;		
	#else
		ADCON0 = 0x01;		// ADON, Channel 0
		ADCON1 = 0x0E;		// Vdd/Vss is +/-REF, AN0 is analog
	#endif
	ADCON2 = 0xBE;		// Right justify, 20TAD ACQ time, Fosc/64 (~21.0kHz)


    // Enable internal PORTB pull-ups
    INTCON2bits.RBPU = 0;

	// Configure USART
    TXSTA = 0x20;
    RCSTA = 0x90;

	// See if we can use the high baud rate setting
	#if ((GetPeripheralClock()+2*BAUD_RATE)/BAUD_RATE/4 - 1) <= 255
		SPBRG = (GetPeripheralClock()+2*BAUD_RATE)/BAUD_RATE/4 - 1;
		TXSTAbits.BRGH = 1;
	#else	// Use the low baud rate setting
		SPBRG = (GetPeripheralClock()+8*BAUD_RATE)/BAUD_RATE/16 - 1;
	#endif


	// Enable Interrupts
	RCONbits.IPEN = 1;		// Enable interrupt priorities
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    // Do a calibration A/D conversion
	#if defined(__18F87J10) || defined(__18F86J15) || defined(__18F86J10) || defined(__18F85J15) || defined(__18F85J10) || defined(__18F67J10) || defined(__18F66J15) || defined(__18F66J10) || defined(__18F65J15) || defined(__18F65J10) || defined(__18F97J60) || defined(__18F96J65) || defined(__18F96J60) || defined(__18F87J60) || defined(__18F86J65) || defined(__18F86J60) || defined(__18F67J60) || defined(__18F66J65) || defined(__18F66J60) || \
	     defined(_18F87J10) ||  defined(_18F86J15) || defined(_18F86J10)  ||  defined(_18F85J15) ||  defined(_18F85J10) ||  defined(_18F67J10) ||  defined(_18F66J15) ||  defined(_18F66J10) ||  defined(_18F65J15) ||  defined(_18F65J10) ||  defined(_18F97J60) ||  defined(_18F96J65) ||  defined(_18F96J60) ||  defined(_18F87J60) ||  defined(_18F86J65) ||  defined(_18F86J60) ||  defined(_18F67J60) ||  defined(_18F66J65) ||  defined(_18F66J60)
		ADCON0bits.ADCAL = 1;
	    ADCON0bits.GO = 1;
		while(ADCON0bits.GO);
		ADCON0bits.ADCAL = 0;
	#elif defined(__18F87J11) || defined(__18F86J16) || defined(__18F86J11) || defined(__18F67J11) || defined(__18F66J16) || defined(__18F66J11) || \
		   defined(_18F87J11) ||  defined(_18F86J16) ||  defined(_18F86J11) ||  defined(_18F67J11) ||  defined(_18F66J16) ||  defined(_18F66J11) || \
		  defined(__18F87J50) || defined(__18F86J55) || defined(__18F86J50) || defined(__18F67J50) || defined(__18F66J55) || defined(__18F66J50) || \
		   defined(_18F87J50) ||  defined(_18F86J55) ||  defined(_18F86J50) ||  defined(_18F67J50) ||  defined(_18F66J55) ||  defined(_18F66J50)
		ADCON1bits.ADCAL = 1;
	    ADCON0bits.GO = 1;
		while(ADCON0bits.GO);
		ADCON1bits.ADCAL = 0;
	#endif

#else	// 16-bit C30 and and 32-bit C32
	#if defined(__PIC32MX__)
	{
		// Enable multi-vectored interrupts
		INTEnableSystemMultiVectoredInt();
		
		// Enable optimal performance
		SYSTEMConfigPerformance(GetSystemClock());
		mOSCSetPBDIV(OSC_PB_DIV_1);				// Use 1:1 CPU Core:Peripheral clocks
		
		// Disable JTAG port so we get our I/O pins back, but first
		// wait 50ms so if you want to reprogram the part with 
		// JTAG, you'll still have a tiny window before JTAG goes away.
		// The PIC32 Starter Kit debuggers use JTAG and therefore must not 
		// disable JTAG.
		DelayMs(50);
		#if !defined(__MPLAB_DEBUGGER_PIC32MXSK) && !defined(__MPLAB_DEBUGGER_FS2)
			DDPCONbits.JTAGEN = 0;
		#endif
		LED_PUT(0x00);				// Turn the LEDs off
		//FERNANDO, MODIFICADO PARA LOS BOTONES DE MI PLACA
		//CNPUESET = 0x00098000;		// Turn on weak pull ups on CN15, CN16, CN19 (RD5, RD7, RD13), which is connected to buttons on PIC32 Starter Kit boards
		CNPUESET = 0x00006000; // CN13 Y CN14 (RD4 Y RD5)
	}
	#endif

	#if defined(__dsPIC33F__) || defined(__PIC24H__)
		// Crank up the core frequency
		PLLFBD = 38;				// Multiply by 40 for 160MHz VCO output (8MHz XT oscillator)
		CLKDIV = 0x0000;			// FRC: divide by 2, PLLPOST: divide by 2, PLLPRE: divide by 2
	
		// Port I/O
		AD1PCFGHbits.PCFG23 = 1;	// Make RA7 (BUTTON1) a digital input
		AD1PCFGHbits.PCFG20 = 1;	// Make RA12 (INT1) a digital input for MRF24WB0M PICtail Plus interrupt

		// ADC
	    AD1CHS0 = 0;				// Input to AN0 (potentiometer)
		AD1PCFGLbits.PCFG5 = 0;		// Disable digital input on AN5 (potentiometer)
		AD1PCFGLbits.PCFG4 = 0;		// Disable digital input on AN4 (TC1047A temp sensor)
	#else	//defined(__PIC24F__) || defined(__PIC32MX__)
		#if defined(__PIC24F__)
			CLKDIVbits.RCDIV = 0;		// Set 1:1 8MHz FRC postscalar
		#endif
		
		// ADC
	    #if defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
	    	// Disable analog on all pins
	    	ANSA = 0x0000;
	    	ANSB = 0x0000;
	    	ANSC = 0x0000;
	    	ANSD = 0x0000;
	    	ANSE = 0x0000;
	    	ANSF = 0x0000;
	    	ANSG = 0x0000;
		#else
		    AD1CHS = 0;					// Input to AN0 (potentiometer)
			//AD1PCFGbits.PCFG4 = 0;		// Disable digital input on AN4 (TC1047A temp sensor) //FERNANDO, RB4 EN MI PLACA ES UN LED
			#if defined(__32MX460F512L__) || defined(__32MX795F512L__)	// PIC32MX460F512L and PIC32MX795F512L PIMs has different pinout to accomodate USB module
				AD1PCFGbits.PCFG2 = 0;		// Disable digital input on AN2 (potentiometer)
			#else
				AD1PCFGbits.PCFG5 = 0;		// Disable digital input on AN5 (potentiometer) //FERNANDO, ESTO ME VALE ESTE PIN ESTA PUESTO AL AIRE
			#endif
		#endif
	#endif

	// ADC
	AD1CON1 = 0x84E4;			// Turn on, auto sample start, auto-convert, 12 bit mode (on parts with a 12bit A/D)
	AD1CON2 = 0x0404;			// AVdd, AVss, int every 2 conversions, MUXA only, scan
	AD1CON3 = 0x1003;			// 16 Tad auto-sample, Tad = 3*Tcy
	#if defined(__32MX460F512L__) || defined(__32MX795F512L__)	// PIC32MX460F512L and PIC32MX795F512L PIMs has different pinout to accomodate USB module
		AD1CSSL = 1<<2;				// Scan pot
	#else
		AD1CSSL = 1<<5;				// Scan pot
	#endif

	// UART
	#if defined(STACK_USE_UART)
		UARTTX_TRIS = 0;
		UARTRX_TRIS = 1;
		UMODE = 0x8000;			// Set UARTEN.  Note: this must be done before setting UTXEN

		#if defined(__C30__)
			USTA = 0x0400;		// UTXEN set
			#define CLOSEST_UBRG_VALUE ((GetPeripheralClock()+8ul*BAUD_RATE)/16/BAUD_RATE-1)
			#define BAUD_ACTUAL (GetPeripheralClock()/16/(CLOSEST_UBRG_VALUE+1))
		#else	//defined(__C32__)
			USTA = 0x00001400;		// RXEN set, TXEN set
			#define CLOSEST_UBRG_VALUE ((GetPeripheralClock()+8ul*BAUD_RATE)/16/BAUD_RATE-1)
			#define BAUD_ACTUAL (GetPeripheralClock()/16/(CLOSEST_UBRG_VALUE+1))
		#endif
	
		#define BAUD_ERROR ((BAUD_ACTUAL > BAUD_RATE) ? BAUD_ACTUAL-BAUD_RATE : BAUD_RATE-BAUD_ACTUAL)
		#define BAUD_ERROR_PRECENT	((BAUD_ERROR*100+BAUD_RATE/2)/BAUD_RATE)
		#if (BAUD_ERROR_PRECENT > 3)
			#warning UART frequency error is worse than 3%
		#elif (BAUD_ERROR_PRECENT > 2)
			#warning UART frequency error is worse than 2%
		#endif
	
		UBRG = CLOSEST_UBRG_VALUE;
	#endif

#endif

// Deassert all chip select lines so there isn't any problem with 
// initialization order.  Ex: When ENC28J60 is on SPI2 with Explorer 16, 
// MAX3232 ROUT2 pin will drive RF12/U2CTS ENC28J60 CS line asserted, 
// preventing proper 25LC256 EEPROM operation.
#if defined(ENC_CS_TRIS)
	ENC_CS_IO = 1;
	ENC_CS_TRIS = 0;
#endif
#if defined(ENC100_CS_TRIS)
	ENC100_CS_IO = (ENC100_INTERFACE_MODE == 0);
	ENC100_CS_TRIS = 0;
#endif
#if defined(EEPROM_CS_TRIS)
	EEPROM_CS_IO = 1;
	EEPROM_CS_TRIS = 0;
#endif
#if defined(SPIRAM_CS_TRIS)
	SPIRAM_CS_IO = 1;
	SPIRAM_CS_TRIS = 0;
#endif
#if defined(SPIFLASH_CS_TRIS)
	SPIFLASH_CS_IO = 1;
	SPIFLASH_CS_TRIS = 0;
#endif
#if defined(WF_CS_TRIS)
	WF_CS_IO = 1;
	WF_CS_TRIS = 0;
#endif

#if defined(PIC24FJ64GA004_PIM)
	__builtin_write_OSCCONL(OSCCON & 0xBF);  // Unlock PPS

	// Remove some LED outputs to regain other functions
	LED1_TRIS = 1;		// Multiplexed with BUTTON0
	LED5_TRIS = 1;		// Multiplexed with EEPROM CS
	LED7_TRIS = 1;		// Multiplexed with BUTTON1
	
	// Inputs
	RPINR19bits.U2RXR = 19;			//U2RX = RP19
	RPINR22bits.SDI2R = 20;			//SDI2 = RP20
	RPINR20bits.SDI1R = 17;			//SDI1 = RP17
	
	// Outputs
	RPOR12bits.RP25R = U2TX_IO;		//RP25 = U2TX  
	RPOR12bits.RP24R = SCK2OUT_IO; 	//RP24 = SCK2
	RPOR10bits.RP21R = SDO2_IO;		//RP21 = SDO2
	RPOR7bits.RP15R = SCK1OUT_IO; 	//RP15 = SCK1
	RPOR8bits.RP16R = SDO1_IO;		//RP16 = SDO1
	
	AD1PCFG = 0xFFFF;				//All digital inputs - POT and Temp are on same pin as SDO1/SDI1, which is needed for ENC28J60 commnications

	__builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS
#endif

#if defined(__PIC24FJ256DA210__)
	__builtin_write_OSCCONL(OSCCON & 0xBF);  // Unlock PPS

	// Inputs
	RPINR19bits.U2RXR = 11;	// U2RX = RP11
	RPINR20bits.SDI1R = 0;	// SDI1 = RP0
	RPINR0bits.INT1R = 34;	// Assign RE9/RPI34 to INT1 (input) for MRF24WB0M Wi-Fi PICtail Plus interrupt
	
	// Outputs
	RPOR8bits.RP16R = 5;	// RP16 = U2TX
	RPOR1bits.RP2R = 8; 	// RP2 = SCK1
	RPOR0bits.RP1R = 7;		// RP1 = SDO1

	__builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS
#endif

#if defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256GB210__)
	__builtin_write_OSCCONL(OSCCON & 0xBF);  // Unlock PPS
	
	// Configure SPI1 PPS pins (ENC28J60/ENCX24J600/MRF24WB0M or other PICtail Plus cards)
	RPOR0bits.RP0R = 8;		// Assign RP0 to SCK1 (output)
	RPOR7bits.RP15R = 7;	// Assign RP15 to SDO1 (output)
	RPINR20bits.SDI1R = 23;	// Assign RP23 to SDI1 (input)

	// Configure SPI2 PPS pins (25LC256 EEPROM on Explorer 16)
	RPOR10bits.RP21R = 11;	// Assign RG6/RP21 to SCK2 (output)
	RPOR9bits.RP19R = 10;	// Assign RG8/RP19 to SDO2 (output)
	RPINR22bits.SDI2R = 26;	// Assign RG7/RP26 to SDI2 (input)
	
	// Configure UART2 PPS pins (MAX3232 on Explorer 16)
	#if !defined(ENC100_INTERFACE_MODE) || (ENC100_INTERFACE_MODE == 0) || defined(ENC100_PSP_USE_INDIRECT_RAM_ADDRESSING)
	RPINR19bits.U2RXR = 10;	// Assign RF4/RP10 to U2RX (input)
	RPOR8bits.RP17R = 5;	// Assign RF5/RP17 to U2TX (output)
	#endif
	
	// Configure INT1 PPS pin (MRF24WB0M Wi-Fi PICtail Plus interrupt signal when in SPI slot 1)
	RPINR0bits.INT1R = 33;	// Assign RE8/RPI33 to INT1 (input)

	// Configure INT3 PPS pin (MRF24WB0M Wi-Fi PICtail Plus interrupt signal when in SPI slot 2)
	RPINR1bits.INT3R = 40;	// Assign RC3/RPI40 to INT3 (input)

	__builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS
#endif

#if defined(__PIC24FJ256GA110__)
	__builtin_write_OSCCONL(OSCCON & 0xBF);  // Unlock PPS
	
	// Configure SPI2 PPS pins (25LC256 EEPROM on Explorer 16 and ENC28J60/ENCX24J600/MRF24WB0M or other PICtail Plus cards)
	// Note that the ENC28J60/ENCX24J600/MRF24WB0M PICtails SPI PICtails must be inserted into the middle SPI2 socket, not the topmost SPI1 slot as normal.  This is because PIC24FJ256GA110 A3 silicon has an input-only RPI PPS pin in the ordinary SCK1 location.  Silicon rev A5 has this fixed, but for simplicity all demos will assume we are using SPI2.
	RPOR10bits.RP21R = 11;	// Assign RG6/RP21 to SCK2 (output)
	RPOR9bits.RP19R = 10;	// Assign RG8/RP19 to SDO2 (output)
	RPINR22bits.SDI2R = 26;	// Assign RG7/RP26 to SDI2 (input)
	
	// Configure UART2 PPS pins (MAX3232 on Explorer 16)
	RPINR19bits.U2RXR = 10;	// Assign RF4/RP10 to U2RX (input)
	RPOR8bits.RP17R = 5;	// Assign RF5/RP17 to U2TX (output)
	
	// Configure INT3 PPS pin (MRF24WB0M PICtail Plus interrupt signal)
	RPINR1bits.INT3R = 36;	// Assign RA14/RPI36 to INT3 (input)

	__builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS
#endif


#if defined(DSPICDEM11)
	// Deselect the LCD controller (PIC18F252 onboard) to ensure there is no SPI2 contention
	LCDCTRL_CS_TRIS = 0;
	LCDCTRL_CS_IO = 1;

	// Hold the codec in reset to ensure there is no SPI2 contention
	CODEC_RST_TRIS = 0;
	CODEC_RST_IO = 0;
#endif

#if defined(SPIRAM_CS_TRIS)
	SPIRAMInit();
#endif
#if defined(EEPROM_CS_TRIS)
	XEEInit();
#endif
#if defined(SPIFLASH_CS_TRIS)
	SPIFlashInit();
#endif
}

/*********************************************************************
 * Function:        void InitAppConfig(void)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           None
 *
 * Output:          Write/Read non-volatile config variables.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
// MAC Address Serialization using a MPLAB PM3 Programmer and 
// Serialized Quick Turn Programming (SQTP). 
// The advantage of using SQTP for programming the MAC Address is it
// allows you to auto-increment the MAC address without recompiling 
// the code for each unit.  To use SQTP, the MAC address must be fixed
// at a specific location in program memory.  Uncomment these two pragmas
// that locate the MAC address at 0x1FFF0.  Syntax below is for MPLAB C 
// Compiler for PIC18 MCUs. Syntax will vary for other compilers.
//#pragma romdata MACROM=0x1FFF0
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1, MY_DEFAULT_MAC_BYTE2, MY_DEFAULT_MAC_BYTE3, MY_DEFAULT_MAC_BYTE4, MY_DEFAULT_MAC_BYTE5, MY_DEFAULT_MAC_BYTE6};
//#pragma romdata

static void InitAppConfig(void)
{
#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
	unsigned char vNeedToSaveDefaults = 0;
#endif
	
	while(1)
	{
		// Start out zeroing all AppConfig bytes to ensure all fields are 
		// deterministic for checksum generation
		memset((void*)&AppConfig, 0x00, sizeof(AppConfig));
		
		AppConfig.Flags.bIsDHCPEnabled = TRUE;
		AppConfig.Flags.bInConfigMode = TRUE;
		memcpypgm2ram((void*)&AppConfig.MyMACAddr, (ROM void*)SerializedMACAddress, sizeof(AppConfig.MyMACAddr));
//		{
//			_prog_addressT MACAddressAddress;
//			MACAddressAddress.next = 0x157F8;
//			_memcpy_p2d24((char*)&AppConfig.MyMACAddr, MACAddressAddress, sizeof(AppConfig.MyMACAddr));
//		}
		AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2<<8ul | MY_DEFAULT_IP_ADDR_BYTE3<<16ul | MY_DEFAULT_IP_ADDR_BYTE4<<24ul;
		AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
		AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2<<8ul | MY_DEFAULT_MASK_BYTE3<<16ul | MY_DEFAULT_MASK_BYTE4<<24ul;
		AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
		AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2<<8ul | MY_DEFAULT_GATE_BYTE3<<16ul | MY_DEFAULT_GATE_BYTE4<<24ul;
		AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 | MY_DEFAULT_PRIMARY_DNS_BYTE2<<8ul  | MY_DEFAULT_PRIMARY_DNS_BYTE3<<16ul  | MY_DEFAULT_PRIMARY_DNS_BYTE4<<24ul;
		AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 | MY_DEFAULT_SECONDARY_DNS_BYTE2<<8ul  | MY_DEFAULT_SECONDARY_DNS_BYTE3<<16ul  | MY_DEFAULT_SECONDARY_DNS_BYTE4<<24ul;
	
	
		// SNMP Community String configuration
		#if defined(STACK_USE_SNMP_SERVER)
		{
			BYTE i;
			static ROM char * ROM cReadCommunities[] = SNMP_READ_COMMUNITIES;
			static ROM char * ROM cWriteCommunities[] = SNMP_WRITE_COMMUNITIES;
			ROM char * strCommunity;
			
			for(i = 0; i < SNMP_MAX_COMMUNITY_SUPPORT; i++)
			{
				// Get a pointer to the next community string
				strCommunity = cReadCommunities[i];
				if(i >= sizeof(cReadCommunities)/sizeof(cReadCommunities[0]))
					strCommunity = "";
	
				// Ensure we don't buffer overflow.  If your code gets stuck here, 
				// it means your SNMP_COMMUNITY_MAX_LEN definition in TCPIPConfig.h 
				// is either too small or one of your community string lengths 
				// (SNMP_READ_COMMUNITIES) are too large.  Fix either.
				if(strlenpgm(strCommunity) >= sizeof(AppConfig.readCommunity[0]))
					while(1);
				
				// Copy string into AppConfig
				strcpypgm2ram((char*)AppConfig.readCommunity[i], strCommunity);
	
				// Get a pointer to the next community string
				strCommunity = cWriteCommunities[i];
				if(i >= sizeof(cWriteCommunities)/sizeof(cWriteCommunities[0]))
					strCommunity = "";
	
				// Ensure we don't buffer overflow.  If your code gets stuck here, 
				// it means your SNMP_COMMUNITY_MAX_LEN definition in TCPIPConfig.h 
				// is either too small or one of your community string lengths 
				// (SNMP_WRITE_COMMUNITIES) are too large.  Fix either.
				if(strlenpgm(strCommunity) >= sizeof(AppConfig.writeCommunity[0]))
					while(1);
	
				// Copy string into AppConfig
				strcpypgm2ram((char*)AppConfig.writeCommunity[i], strCommunity);
			}
		}
		#endif
	
		// Load the default NetBIOS Host Name
		memcpypgm2ram(AppConfig.NetBIOSName, (ROM void*)MY_DEFAULT_HOST_NAME, 16);
		FormatNetBIOSName(AppConfig.NetBIOSName);
	
		#if defined(WF_CS_TRIS)
			// Load the default SSID Name
			WF_ASSERT(sizeof(MY_DEFAULT_SSID_NAME) <= sizeof(AppConfig.MySSID));
			memcpypgm2ram(AppConfig.MySSID, (ROM void*)MY_DEFAULT_SSID_NAME, sizeof(MY_DEFAULT_SSID_NAME));
			AppConfig.SsidLength = sizeof(MY_DEFAULT_SSID_NAME) - 1;
	        #if defined (EZ_CONFIG_STORE)
	        AppConfig.SecurityMode = MY_DEFAULT_WIFI_SECURITY_MODE;
	        AppConfig.networkType = MY_DEFAULT_NETWORK_TYPE;
	        AppConfig.dataValid = 0;
	        #endif // EZ_CONFIG_STORE
		#endif

		// Compute the checksum of the AppConfig defaults as loaded from ROM
		wOriginalAppConfigChecksum = CalcIPChecksum((BYTE*)&AppConfig, sizeof(AppConfig));

		#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
		{
			NVM_VALIDATION_STRUCT NVMValidationStruct;

			// Check to see if we have a flag set indicating that we need to 
			// save the ROM default AppConfig values.
			if(vNeedToSaveDefaults)
				SaveAppConfig(&AppConfig);
		
			// Read the NVMValidation record and AppConfig struct out of EEPROM/Flash
			#if defined(EEPROM_CS_TRIS)
			{
				XEEReadArray(0x0000, (BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
				XEEReadArray(sizeof(NVMValidationStruct), (BYTE*)&AppConfig, sizeof(AppConfig));
			}
			#elif defined(SPIFLASH_CS_TRIS)
			{
				SPIFlashReadArray(0x0000, (BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
				SPIFlashReadArray(sizeof(NVMValidationStruct), (BYTE*)&AppConfig, sizeof(AppConfig));
			}
			#endif
	
			// Check EEPROM/Flash validitity.  If it isn't valid, set a flag so 
			// that we will save the ROM default values on the next loop 
			// iteration.
			if((NVMValidationStruct.wConfigurationLength != sizeof(AppConfig)) ||
			   (NVMValidationStruct.wOriginalChecksum != wOriginalAppConfigChecksum) ||
			   (NVMValidationStruct.wCurrentChecksum != CalcIPChecksum((BYTE*)&AppConfig, sizeof(AppConfig))))
			{
				// Check to ensure that the vNeedToSaveDefaults flag is zero, 
				// indicating that this is the first iteration through the do 
				// loop.  If we have already saved the defaults once and the 
				// EEPROM/Flash still doesn't pass the validity check, then it 
				// means we aren't successfully reading or writing to the 
				// EEPROM/Flash.  This means you have a hardware error and/or 
				// SPI configuration error.
				if(vNeedToSaveDefaults)
				{
					while(1);
				}
				
				// Set flag and restart loop to load ROM defaults and save them
				vNeedToSaveDefaults = 1;
				continue;
			}
			
			// If we get down here, it means the EEPROM/Flash has valid contents 
			// and either matches the ROM defaults or previously matched and 
			// was run-time reconfigured by the user.  In this case, we shall 
			// use the contents loaded from EEPROM/Flash.
			break;
		}
		#endif
		break;
	}


    #if defined (EZ_CONFIG_STORE)
    // Set configuration for ZG from NVM
    /* Set security type and key if necessary, convert from app storage to ZG driver */

    if (AppConfig.dataValid)
        CFGCXT.isWifiDoneConfigure = 1;

    AppConfig.saveSecurityInfo = FALSE;
    #endif // EZ_CONFIG_STORE
}

#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
void SaveAppConfig(const APP_CONFIG *ptrAppConfig)
{
	NVM_VALIDATION_STRUCT NVMValidationStruct;

	// Ensure adequate space has been reserved in non-volatile storage to 
	// store the entire AppConfig structure.  If you get stuck in this while(1) 
	// trap, it means you have a design time misconfiguration in TCPIPConfig.h.
	// You must increase MPFS_RESERVE_BLOCK to allocate more space.
	#if defined(STACK_USE_MPFS) || defined(STACK_USE_MPFS2)
		if(sizeof(NVMValidationStruct) + sizeof(AppConfig) > MPFS_RESERVE_BLOCK)
			while(1);
	#endif

	// Get proper values for the validation structure indicating that we can use 
	// these EEPROM/Flash contents on future boot ups
	NVMValidationStruct.wOriginalChecksum = wOriginalAppConfigChecksum;
	NVMValidationStruct.wCurrentChecksum = CalcIPChecksum((BYTE*)ptrAppConfig, sizeof(APP_CONFIG));
	NVMValidationStruct.wConfigurationLength = sizeof(APP_CONFIG);

	// Write the validation struct and current AppConfig contents to EEPROM/Flash
	#if defined(EEPROM_CS_TRIS)
	    XEEBeginWrite(0x0000);
	    XEEWriteArray((BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
		XEEWriteArray((BYTE*)ptrAppConfig, sizeof(APP_CONFIG));
    #else
		SPIFlashBeginWrite(0x0000);
		SPIFlashWriteArray((BYTE*)&NVMValidationStruct, sizeof(NVMValidationStruct));
		SPIFlashWriteArray((BYTE*)ptrAppConfig, sizeof(APP_CONFIG));
    #endif
}
#endif

#if defined (EZ_CONFIG_STORE)
void RestoreWifiConfig(void)
{
	putrsUART((ROM char*)"\r\nButton push, restore wifi configuration!!!\r\n");

#if defined(EEPROM_CS_TRIS)
    XEEBeginWrite(0x0000);
    XEEWrite(0xFF);
    XEEWrite(0xFF);
    XEEEndWrite();
#elif defined(SPIFLASH_CS_TRIS)
    SPIFlashBeginWrite(0x0000);
    SPIFlashWrite(0xFF);
    SPIFlashWrite(0xFF);
#endif

    // reboot here...
	LED_PUT(0x00);
	while(BUTTON3_IO == 0u);
    Reset();
}

#endif // EZ_CONFIG_STORE
