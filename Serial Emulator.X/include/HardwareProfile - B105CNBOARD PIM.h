/* 
 * File:   HardwareProfile - B105CNBOARD PIM.h
 * Author: guilja
 *
 * Created on 18 de julio de 2012, 13:10
 */

#ifndef HARDWAREPROFILE___B105CNBOARD_PIM_H
#define	HARDWAREPROFILE___B105CNBOARD_PIM_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

    /*******************************************************************/
    /******** USB stack hardware selection options *********************/
    /*******************************************************************/
    //This section is the set of definitions required by the MCHPFSUSB
    //  framework.  These definitions tell the firmware what mode it is
    //  running in, and where it can find the results to some information
    //  that the stack needs.
    //These definitions are required by every application developed with
    //  this revision of the MCHPFSUSB framework.  Please review each
    //  option carefully and determine which options are desired/required
    //  for your application.

    //#define USE_SELF_POWER_SENSE_IO
//    #define tris_self_power     TRISAbits.TRISA2    // Input //Porque en nuestro
// ni siquiera existe.
    #define self_power          1

    //#define USE_USB_BUS_SENSE_IO
//    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input //XXX-Willy. Porque
// lo utilizamos para otra cosa.
    #define USB_BUS_SENSE       1

    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/
    /******** Application specific definitions *************************/
    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/

    /** Board definition ***********************************************/
    //These defintions will tell the main() function which board is
    //  currently selected.  This will allow the application to add
    //  the correct configuration bits as wells use the correct
    //  initialization functions for the board.  These defitions are only
    //  required in the stack provided demos.  They are not required in
    //  final application design.
    #define DEMO_BOARD PIC32MX795F512H_PIM //XXX-Willy.
    #define B105CNBOARD //XXX-Willy.

    //#define RUN_AT_48MHZ
    //#define RUN_AT_24MHZ
    //#define RUN_AT_60MHZ //XXX-Willy.
    #define RUN_AT_80MHZ //XXX-Willy.

    // Various clock values
    #if defined(RUN_AT_48MHZ)
        #define GetSystemClock()            48000000UL
        #define GetPeripheralClock()        48000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_24MHZ)
        #define GetSystemClock()            24000000UL
        #define GetPeripheralClock()        24000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_60MHZ)
        #define GetSystemClock()            60000000UL
        #define GetPeripheralClock()        60000000UL  // Will be divided down
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_80MHZ) //XXX-Willy.
        #define GetSystemClock()            80000000UL
        #define CLOCK_FREQ   80000000UL
//        #define GetPeripheralClock()        80000000UL  // Will be divided down
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #else
        #error Choose a speed
    #endif

    /** LED ************************************************************/
    #define mInitAllLEDs()      LATB &= 0xFC3F; TRISB &= 0xFC3F; //LATA &= 0xFFC3; TRISA &= 0xFFC3; //XXX-Willy.

    #define mLED_1              LATBbits.LATB6 // LATAbits.LATA2 //XXX-Willy
    #define mLED_2              LATBbits.LATB7 // LATAbits.LATA3 //XXX-Willy
    #define mLED_3              LATBbits.LATB8 // LATAbits.LATA4 //XXX-Willy
    #define mLED_4              LATBbits.LATB9 // LATAbits.LATA5 //XXX-Willy

    #define mGetLED_1()         mLED_1
    #define mGetLED_2()         mLED_2
    #define mGetLED_3()         mLED_3
    #define mGetLED_4()         mLED_4

    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         mLED_3 = 1;
    #define mLED_4_On()         mLED_4 = 1;

    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3 = 0;
    #define mLED_4_Off()        mLED_4 = 0;

    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3 = !mLED_3;
    #define mLED_4_Toggle()     mLED_4 = !mLED_4;

    /** SWITCH *********************************************************/
    #define mInitSwitch2()      TRISDbits.TRISD5=1; //TRISDbits.TRISD6=1; //XXX-Willy.
    #define mInitSwitch3()      TRISDbits.TRISD4=1; //TRISDbits.TRISD7=1; //XXX-Willy.
    #define mInitAllSwitches()  mInitSwitch2();mInitSwitch3();
    #define sw2                 PORTDbits.RD5 //el SW1 de la placa de Fern.
    #define sw3                 PORTDbits.RD4 //el SW2 de la placa de Fern.

    /** UART ***********************************************************/
    #define BAUDRATE2       19200UL
    #define BRG_DIV2        4
    #define BRGH2           1

    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0


#endif	/* HARDWAREPROFILE___B105CNBOARD_PIM_H */

