/*******************************************************************************
 FileName:      HardwareProfile.h for LSI TARGET BOARD - PIC32MX795F512L (*or H)
 Dependencies:  See INCLUDES section
 Processor:     PIC32 USB Microcontrollers
 Hardware:      PIC32MX795F512L PIM
 Compiler:      Microchip C32 (for PIC32)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the ?Company?) for its PIC? Microcontroller is intended and supplied to you,
 the Company?s customer, for use solely and exclusively on Microchip PIC
 Microcontroller products. The software is owned by the Company and/or its
 supplier, and is protected under applicable copyright laws. All rights are
 reserved. Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to civil liability
 for the breach of the terms and conditions of this license.

 THIS SOFTWARE IS PROVIDED IN AN ?AS IS? CONDITION. NO WARRANTIES, WHETHER
 EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO
 THIS SOFTWARE. THE COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR
 SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
********************************************************************************
 File Description: LSI CWSN NODE TARGET BOARD's HARDWARE PROFILE
 Author:  Juan Domingo Rebollo - LSI - Laboratorio de Sistemas Intergados - UPM

 Designed for PIC32MX 664F064L / 664F128L / 675F256L / 675F512L / 695F512L or
 PIC32MX 775F256L / 775F512L / 795F512L (100 pins layout, 4 SPI modules)

 Most of features also adapted for PIC32MX795F512H (Previous MCU target, 64 pins
 layout and 3 SPI modules) for test and development purposes.
*******************************************************************************/

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "WirelessProtocols/ConfigApp.h"

////////////////////////////////////////////////////////////////////////////////
/* RADIO INTERFACES - See individual stacks' config files *********************/
////////////////////////////////////////////////////////////////////////////////

//#define MIWI_0434_RI     //Comment if it is not available in the target board.
//#define MIWI_0868_RI     //Comment if it is not available in the target board.
#define MIWI_2400_RI     //Comment if it is not available in the target board.


////////////////////////////////////////////////////////////////////////////////
/* TRANSCEIVERS Board configuration *******************************************/
////////////////////////////////////////////////////////////////////////////////
//MiWi transceivers available in the node are selected in "ConfigApp.h" file.

#if defined MIWI_0434_RI
    #define MRF49XA_1       //respetamos pero esta nomenclatura va a volar
    #define MRF49XA_1_IN_434
    #define MRF49XA_1_IN_SPI1
    #define MRF49XA_1_USES_INT1

    //FUTURA NOMENCLATURA
    //#define MRF49XA_0434
    // #define MIWI_0434_IN_SPI1
    // #define MIWI_0434_IN_INT1

#endif
#if defined MIWI_0868_RI
    #define MRF49XA_2       //respetamos pero esta nomenclactura va a volar
    #define MRF49XA_2_IN_868
    #define MRF49XA_2_IN_SPI3
    #define MRF49XA_2_USES_INT3

   // #define MRF49XA_0868
   // #define MIWI_0868_IN_SPI3
   // #define MIWI_0868_IN_INT3

#endif
#if defined MIWI_2400_RI
    #define MRF24J40
    #define MRF24J40_IN_SPI4
    #define MRF24J40_USES_INT4

    //FUTURA NOMENCLATURA
    //#define MIWI_2400_IN_SPI4
    //#define MIWI_2400_IN_INT4
#endif

////////////////////////////////////////////////////////////////////////////////
/* DEBUGGING OPTIONS **********************************************************/
////////////////////////////////////////////////////////////////////////////////
//ENABLE CONSOLE is defined in ConfigApp.h. Traces will be sent to PC via RS232
//using UART if this option is enabled. Choose the UART module to be used
//according to your hardware profile.
#if defined ENABLE_CONSOLE //|| defined STACK_USE_UART
   #define DEBUG_UART3       //IMPORTANT: Do not define if SPI2 is in use.
    
#elif defined ENABLE_USB_DEBUG
    //IMPLEMENTAR VOLCAR TRAZAS VÍA USB.
#endif

////////////////////////////////////////////////////////////////////////////////
/* MICROCONTROLLER SLEEP AND WAKE OPTIONS *************************************/
////////////////////////////////////////////////////////////////////////////////
#define WAKE_FROM_SLEEP_WDT
//#define WAKE_FROM_SLEEP_SOSC_T1
#if defined WAKE_FROM_SLEEP_SOSC_T1
    #define SOSC_FREQ_HZ 32768      //Secondary oscillator frequency in Hertz
#endif

    //ASSERTIONS
    #if defined WAKE_FROM_SLEEP_WDT && defined WAKE_FROM_SLEEP_SOSC_T1
        #error "Select only one option for sleeping the device."
    #endif

////////////////////////////////////////////////////////////////////////////////
/* STACKS MAINTENANCE TASKS ***************************************************/
////////////////////////////////////////////////////////////////////////////////
/* Choose if the node is in charge of maintening all the stacks. Enabling this
 * option implies using a timer for doing the task peridically. Conversely, if
 * disabled, the application should call the mainteinance function as often as
 * possible. Second option is only recommended for those applications which
 * demand doing their tasks without being interrupted by the maintenance.
 ******************************************************************************/
#define NODE_DOES_MAINTENANCE_TASKS
    #ifndef NODE_DOES_MAINTENANCE_TASKS
        #define APP_DOES_MANTEINANCE_TASKS //App. code should call AllStacksTasks()
    #endif

/* NODE MAINTENANCE TASKS PERIOD ***********************************************
 * Dflt setting: 16-bit synchronous timer using PBCLK (SYSCLK/PBDIV), PBDIV = 4,
 *               SYSCLK = 80 MHz, Timer PreScaler = 256 (PS), INT priority = 1.
 * T(s) = (PBDIV * PS / SYSCLK (Hz)) * PR => it can vary from ~13 us to ~840 ms.
 *
 *  PR value    0x004E  0x0258  0x030D  0x04E2  0x07A1  0x0C35  0x0F42  0x16E3
 *  T (ms)      1       5       10      16      25      40      50      75
 *
 *  PR value    0x1E84  0x30D4  0x4C4B  0x65B9  0x7A12  0x9896  0xCB73  0xE4E1
 *  T (ms)      100     160     250     333,3   400     500     666,7   750
 ******************************************************************************/
#if defined NODE_DOES_MAINTENANCE_TASKS
    #define MAINTENANCE_PERIOD  0x07A1  //Node Maintenance Tasks Period (PR1)
#endif




// Suitable transceivers for the radio interfaces.
// Select the appropriate frequency band for each transceiver according to
// your radio interfaces definition in "ConfigTransceivers.h" file.
#if defined MIWI_0434_RI && !defined MRF49XA_1_IN_434
        #error "One MRF49XA transceivers must define MiWi 434 MHz band."
               "See ConfigTransceivers.h file"
#endif

#if defined MIWI_0868_RI && !defined MRF49XA_2_IN_868
    #error "One MRF49XA transceivers must define MiWi 868 MHz band."
               "See ConfigTransceivers.h file"
#endif

#if defined MIWI_2400_RI && !defined MRF24J40
    #error "MiWi at 2,4 GHz band requires MRF24J40 transceiver."
#endif

// Number of interfaces available
#if !defined(MIWI_2400_RI) && !defined(MIWI_0868_RI) && !defined(MIWI_0434_RI)
    #error "At least one Radio Interface must be defined."
#endif
    #if defined(MIWI_2400_RI) && defined(MIWI_0868_RI) && defined(MIWI_0434_RI)
        #define NumRadioInterfaces 3
    #elif defined(MIWI_2400_RI) ^ defined(MIWI_0868_RI) ^ defined(MIWI_0434_RI)
        #define NumRadioInterfaces 1
    #else
        #define NumRadioInterfaces 2
    #endif
    #define NumMiWiInterfaces (NumRadioInterfaces)

/* END OF VALIDATIONS *********************************************************/

////////////////////////////////////////////////////////////////////////////////
/* PIN DEFINITIONS ************************************************************/
////////////////////////////////////////////////////////////////////////////////
#if defined (__32MX675F256L__) || defined (__32MX675F512L__)

    //SPI1 & INTERRUPT 0
    #define SPI_SDI1    PORTCbits.RC4
    #define SDI1_TRIS   TRISCbits.TRISC4
    #define SPI_SDO1    PORTDbits.RD0
    #define SDO1_TRIS   TRISDbits.TRISD0
    #define SPI_SCK1    PORTDbits.RD10
    #define SCK1_TRIS   TRISDbits.TRISD10
    #define SPI_nSS1    PORTDbits.RD9 // ESTO SE USA PA ALGO?
    #define nSS1_TRIS   TRISDbits.TRISD9

    #define EXTINT_INT0 PORTDbits.RD0
    #define INT0_TRIS   TRISDbits.TRISD0

    //SPI2 & UART3, UART6
    #define SPI_SDI2    PORTGbits.RG7
    #define SDI2_TRIS   TRISGbits.TRISG7
    #define SPI_SDO2    PORTGbits.RG8
    #define SDO2_TRIS   TRISGbits.TRISG8
    #define SPI_SCK2    PORTGbits.RG6
    #define SCK2_TRIS   TRISGbits.TRISG6
    #define SPI_nSS2    PORTGbits.RG9 // ESTO SE USA PA ALGO?
    #define nSS2_TRIS   TRISGbits.TRISG9
//////////////////////////////////////////////////////REVISAAAAAAAAAR PINOUT
    #define UART_U3TX   PORTGbits.RG8
    #define U3TX_TRIS   TRISGbits.TRISG8
    #define UART_U3RX   PORTGbits.RG7
    #define U3RX_TRIS   TRISGbits.TRISG7

    #define UART_U6TX   PORTGbits.RG6
    #define U6TX_TRIS   TRISGbits.TRISG6
    #define UART_U6RX   PORTGbits.RG9
    #define U6RX_TRIS   TRISGbits.TRISG9
    //SPI3 & UART1, UART4
    #define SPI_SDI3    PORTFbits.RF2
    #define SDI3_TRIS   TRISFbits.TRISF2
    #define SPI_SDO3    PORTFbits.RF8
    #define SDO3_TRIS   TRISFbits.TRISF8
    #define SPI_SCK3    PORTDbits.RD15
    #define SCK3_TRIS   TRISDbits.TRISD15
    #define SPI_nSS3    PORTDbits.RD14
    #define nSS3_TRIS   TRISDbits.TRISD14

    #define UART_U1TX   PORTFbits.RF8
    #define U1TX_TRIS   TRISFbits.TRISF8
    #define UART_U1RX   PORTFbits.RF2
    #define U1RX_TRIS   TRISFbits.TRISF2

    #define UART_U4TX   PORTDbits.RD15
    #define U4TX_TRIS   TRISDbits.TRISD15
    #define UART_U4RX   PORTDbits.RD14
    #define U4RX_TRIS   TRISDbits.TRISD14
    //SPI4 & UART2, UART5
    #define SPI_SDI4    PORTFbits.RF4
    #define SDI4_TRIS   TRISFbits.TRISF4
    #define SPI_SDO4    PORTFbits.RF5
    #define SDO4_TRIS   TRISFbits.TRISF5
    #define SPI_SCK4    PORTFbits.RF13
    #define SCK4_TRIS   TRISFbits.TRISF13
    #define SPI_nSS4    PORTFbits.RF12
    #define nSS4_TRIS   TRISFbits.TRISF12

    #define UART_U2TX   PORTFbits.RF5
    #define U2TX_TRIS   TRISFbits.TRISF5
    #define UART_U2RX   PORTFbits.RF4
    #define U2RX_TRIS   TRISFbits.TRISF4

    #define UART_U5TX   ORTFbits.RF13
    #define U5TX_TRIS   TRISFbits.TRISF13
    #define UART_U5RX   PORTFbits.RF12
    #define U5RX_TRIS   TRISFbits.TRISF12
    //I2C
    #define I2C_SDA2    PORTAbits.RA3
    #define SDA2_TRIS   TRISAbits.TRISA3
    #define I2C_SCL2    PORTAbits.RA2
    #define SCL2_TRIS   TRISAbits.TRISA2

    //INTERRUPTS 1,2,3 & 4
    #define EXTINT_INT1 PORTEbits.RE8
    #define INT1_TRIS   TRISEbits.TRISE8
    #define EXTINT_INT2 PORTEbits.RE9
    #define INT2_TRIS   TRISEbits.TRISE9
    #define EXTINT_INT3 PORTAbits.RA14
    #define INT3_TRIS   TRISAbits.TRISA14
    #define EXTINT_INT4 PORTAbits.RA15
    #define INT4_TRIS   TRISAbits.TRISA15

    //IO PORTS Change Notification
    #define BUTTON_1            PORTDbits.RD5
    //#define BUTTON2_TRIS       TRISDbits.TRISD5
    #define BUTTON_2            PORTDbits.RD7
    //#define BUTTON3_TRIS       TRISDbits.TRISD7
    #define ReadBUTTONS()      PORTD
    #define BUTTON_1_PORT_MASK 0x0020
    #define BUTTON_2_PORT_MASK 0x0080

#endif
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
/* I/O pin definitions ********************************************************/
////////////////////////////////////////////////////////////////////////////////
#define INPUT_PIN           1
#define OUTPUT_PIN          0
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
/* Application & Board specific definitions and configuration *****************/
// These defintions will tell the main() function which board is currently
// selected. This will allow the application to add the correct configuration
// bits as well as use the correct initialization functions for the board.
////////////////////////////////////////////////////////////////////////////////

/* SysClock, PBClock... *******************************************************/
#define RUN_AT_80MHZ

#if defined(RUN_AT_80MHZ)
    #define GetSystemClock()        80000000UL
    #define GetPeripheralClock()    80000000UL      //Will be divided down
    #define GetInstructionClock()   (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#else
    #error "Choose a speed"
#endif

#ifdef __PIC32MX__
    #define RCON_SLEEP_MASK (0x8)
    #define RCON_IDLE_MASK  (0x4)
    #define Sleep()         PowerSaveSleep()
    #define CLOCK_FREQ      80000000 //Must be coherent with config in NodeHAL.c
#else
    #define CLOCK_FREQ      8000000
#endif

//TIMERS
#define TMRL TMR2

/* RF TRANSCEIVERS -> SPI interfaces ******************************************/
#if defined(MIWI_0434_RI)
    //SPI selection and pin definitions were above.
   #if defined MRF49XA_1_IN_SPI1
        #define MRF49XA_1_SPI_SDI     SPI_SDI1
        #define MRF49XA_1_SDI_TRIS    SDI1_TRIS
        #define MRF49XA_1_SPI_SDO     SPI_SDO1
        #define MRF49XA_1_SDO_TRIS    SDO1_TRIS
        #define MRF49XA_1_SPI_SCK     SPI_SCK1
        #define MRF49XA_1_SCK_TRIS    SCK1_TRIS

        #define MRF49XA_1_SPICON      SPI1CON
        #define MRF49XA_1_SPICONCLR   SPI1CONCLR
    #else
        #error "Radio Interface at 0434 has no SPI"
    #endif

        //Pins selection for debugging with Guilja's extension board -------------//
        #define MRF49XA_1_PHY_CS            LATEbits.LATE1
        #define MRF49XA_1_PHY_CS_TRIS       TRISEbits.TRISE1
        #define MRF49XA_1_PHY_RESETn        LATEbits.LATE4
        #define MRF49XA_1_PHY_RESETn_TRIS   TRISEbits.TRISE4
        #define MRF49XA_1_nFSEL             LATEbits.LATE3
        #define MRF49XA_1_nFSEL_TRIS        TRISEbits.TRISE3
        #define MRF49XA_1_FINT              PORTBbits.RB2
        #define MRF49XA_1_FINT_TRIS         TRISBbits.TRISB2
   
    #if defined MRF49XA_1_USES_INT1
        #define MRF49XA_1_INT_PIN   EXTINT_INT1
        #define MRF49XA_1_INT_TRIS  INT1_TRIS
        //SFRs CONFIGURATION FOR MRF49XA_1 using INT1
        #define MRF49XA_1_IF        IFS0bits.INT1IF
        #define MRF49XA_1_IE        IEC0bits.INT1IE
#else
        #error "Radio Interface at 0434 has no interruption line"
    #endif
#endif

#if defined (MIWI_0868_RI)
    //SPI selection and pin definitions were above.

    #if defined MRF49XA_2_IN_SPI3
        #define MRF49XA_2_SPI_SDI     SPI_SDI3
        #define MRF49XA_2_SDI_TRIS    SDI3_TRIS
        #define MRF49XA_2_SPI_SDO     SPI_SDO3
        #define MRF49XA_2_SDO_TRIS    SDO3_TRIS
        #define MRF49XA_2_SPI_SCK     SPI_SCK3
        #define MRF49XA_2_SCK_TRIS    SCK3_TRIS

        #define MRF49XA_2_SPICON      SPI3CON
        #define MRF49XA_2_SPICONCLR   SPI3CONCLR
    #else
        #error "Radio Interface at 0868 has no SPI"
    #endif
   

    //Pins selection ---------------------------------------------------------//
    #define MRF49XA_2_PHY_CS            LATEbits.LATE5
    #define MRF49XA_2_PHY_CS_TRIS       TRISEbits.TRISE5
    #define MRF49XA_2_PHY_RESETn        LATEbits.LATE7
    #define MRF49XA_2_PHY_RESETn_TRIS   TRISEbits.TRISE7
    #define MRF49XA_2_nFSEL             LATBbits.LATB1
    #define MRF49XA_2_nFSEL_TRIS        TRISBbits.TRISB1
    #define MRF49XA_2_FINT              PORTEbits.RE9
    #define MRF49XA_2_FINT_TRIS         TRISEbits.TRISE9

    #if defined MRF49XA_2_USES_INT3
        #define MRF49XA_2_INT_PIN   EXTINT_INT3
        #define MRF49XA_2_INT_TRIS  INT3_TRIS
        //SFRs CONFIGURATION FOR MRF49XA_2 using INT3
        #define MRF49XA_2_IF        IFS0bits.INT3IF
        #define MRF49XA_2_IE        IEC0bits.INT3IE
    #else
        #error "Radio Interface at 0868 has no interruption line"
    #endif
#endif

#if defined(MIWI_2400_RI)
    //SPI selection and pin definitions were above.
 
    #if defined MRF24J40_IN_SPI4
        #define SPI_SDI     SPI_SDI4
        #define SDI_TRIS    SDI4_TRIS
        #define SPI_SDO     SPI_SDO4
        #define SDO_TRIS    SDO4_TRIS
        #define SPI_SCK     SPI_SCK4
        #define SCK_TRIS    SCK4_TRIS

        #define SPICON      SPI4CON
        #define SPICONCLR   SPI4CONCLR
    #else
        #error "Radio Interface at 2400 has no interruption SPI"
    #endif

        #define PHY_CS              LATEbits.LATE5          
        #define PHY_CS_TRIS         TRISEbits.TRISE5
        #define PHY_RESETn          LATEbits.LATE7
        #define PHY_RESETn_TRIS     TRISEbits.TRISE7
        #define PHY_WAKE            LATEbits.LATE6
        #define PHY_WAKE_TRIS       TRISEbits.TRISE6

    #if defined MRF24J40_USES_INT4
        #define MRF24J40_INT_PIN    EXTINT_INT4
        #define MRF24J40_INT_TRIS   INT4_TRIS
        //SFRs CONFIGURATION FOR MRF24J40 using INT4
        #define MRF24J40_IF         IFS0bits.INT4IF
        #define MRF24J40_IE         IEC0bits.INT4IE
    #else
        #error "Radio Interface at 2400 has no interruption line"
    #endif
#endif


#if defined DEBUG_UART3
    #define UBRG                U3BRG
    #define UMODE               U3MODE
    #define USTA                U3STA
    #define BusyUART()          !U3STAbits.TRMT
    #define TxNotRdyUART()      !U3STAbits.UTXBF
    #define DataRdyUART()       U3STAbits.URXDA
    #define ReadUART()          (unsigned int) UARTGetDataByte(UART3)
    #define WriteUART(a)        UARTSendDataByte(UART3, a)
    #define getcUART()          UARTGetDataByte(UART3);
#endif

#if defined ENABLE_CONSOLE
    #define putcUART(a)  do{while(BusyUART()); WriteUART(a); while(BusyUART());}while(0)
    #define putrsUART(a) WriteStringPC(a)
    #define putsUART(a)  WriteStringPC(a)
    void WriteStringPC(const char *string);
#endif
/******************************************************************************/