/*****************************************************************************
 *
 *                       USBDebug.h -- USBDebug v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        USBDebug.h
 * Dependencies:    
 * Processor:
 * BOARD:
 * Compiler:        C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         B105 -FAILURE IS NOT AN OPTION-
 *
 *****************************************************************************
 * File Description:
 *
 * Este fichero implemente funciones para hacer posible el envío de mensajes de
 * debug a través del USB. He cogido como base el CDC-Serial Emulator, que en
 * realidad sirve como pasarela entre la UART y el USB. Así que dejo algunas
 * cosas que en realidad sobran para lo que pretendo hacer aquí que es solo
 * imprimir y recibir por el USB.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   25/7/2012    Initial revision
 *****************************************************************************/
#ifndef USBDEBUG_H
#define	USBDEBUG_H


#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

//BOOL B105_USB_Initialized = FALSE; //XXX_Willy.

int B105_USB_Printf(unsigned char* texto);
BYTE B105_USB_Tasks(void);
void B105_USB_Init(void);
char B105_USB_GetC(void);
char* B105_USB_Get(void);
static void B105_InitializeSystem(void);


#endif	/* USBDEBUG_H */
