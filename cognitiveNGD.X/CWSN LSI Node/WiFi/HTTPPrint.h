/**************************************************************
 * HTTPPrint.h
 * Provides callback headers and resolution for user's custom
 * HTTP Application.
 * 
 * This file is automatically generated by the MPFS Utility
 * ALL MODIFICATIONS WILL BE OVERWRITTEN BY THE MPFS GENERATOR
 **************************************************************/

#ifndef __HTTPPRINT_H
#define __HTTPPRINT_H

#include "TCPIP Stack/TCPIP.h"

#if defined(STACK_USE_HTTP2_SERVER)

extern HTTP_CONN curHTTP;
extern HTTP_STUB httpStubs[MAX_HTTP_CONNECTIONS];
extern BYTE curHTTPID;

void HTTPPrint(DWORD callbackID);
void HTTPPrint_led(WORD);
void HTTPPrint_btn(WORD);
void HTTPPrint_pot(void);
void HTTPPrint_scan(void);
void HTTPPrint_fwver(void);
void HTTPPrint_bssCount(void);
void HTTPPrint_valid(void);
void HTTPPrint_name(void);
void HTTPPrint_wlan(void);
void HTTPPrint_privacy(void);
void HTTPPrint_strength(void);
void HTTPPrint_ssid(void);
void HTTPPrint_nextSSID(void);
void HTTPPrint_prevSSID(void);
void HTTPPrint_prevWLAN(void);
void HTTPPrint_nextWLAN(void);

void HTTPPrint(DWORD callbackID)
{
	switch(callbackID)
	{
        case 0x00000002:
			HTTPIncFile((ROM BYTE*)"footer.inc");
			break;
        case 0x00000006:
			HTTPPrint_led(7);
			break;
        case 0x00000007:
			HTTPPrint_led(6);
			break;
        case 0x00000008:
			HTTPPrint_led(5);
			break;
        case 0x00000009:
			HTTPPrint_led(4);
			break;
        case 0x0000000a:
			HTTPPrint_led(3);
			break;
        case 0x0000000b:
			HTTPPrint_led(2);
			break;
        case 0x0000000c:
			HTTPPrint_led(1);
			break;
        case 0x00000017:
			HTTPPrint_led(0);
			break;
        case 0x00000018:
			HTTPPrint_btn(0);
			break;
        case 0x00000019:
			HTTPPrint_btn(1);
			break;
        case 0x0000001a:
			HTTPPrint_btn(2);
			break;
        case 0x0000001b:
			HTTPPrint_btn(3);
			break;
        case 0x0000001c:
			HTTPPrint_pot();
			break;
        case 0x00000033:
			HTTPIncFile((ROM BYTE*)"header.inc");
			break;
        case 0x0000004a:
			HTTPPrint_scan();
			break;
        case 0x0000004b:
			HTTPPrint_fwver();
			break;
        case 0x0000004c:
			HTTPPrint_bssCount();
			break;
        case 0x00000053:
			HTTPPrint_valid();
			break;
        case 0x00000054:
			HTTPPrint_name();
			break;
        case 0x00000056:
			HTTPPrint_wlan();
			break;
        case 0x00000058:
			HTTPPrint_privacy();
			break;
        case 0x00000059:
			HTTPPrint_strength();
			break;
        case 0x0000005b:
			HTTPPrint_ssid();
			break;
        case 0x0000005c:
			HTTPPrint_nextSSID();
			break;
        case 0x0000005d:
			HTTPPrint_prevSSID();
			break;
        case 0x0000005e:
			HTTPPrint_prevWLAN();
			break;
        case 0x0000005f:
			HTTPPrint_nextWLAN();
			break;
		default:
			// Output notification for undefined values
			TCPPutROMArray(sktHTTP, (ROM BYTE*)"!DEF", 4);
	}

	return;
}

void HTTPPrint_(void)
{
	TCPPut(sktHTTP, '~');
	return;
}

#endif

#endif
