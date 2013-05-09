/*****************************************************************************
 *
 *              HAL.h -- HAL v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        HAL.h
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
 * Este archivo desarrolla las funciones y datos del modulo Communication Mod.
 * (CR Module).
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   23/10/2012    Initial revision
 *****************************************************************************/

#ifndef  _HAL_H_
#define  _HAL_H_

#include "GenericTypeDefs.h"
#include "ConfigHAL.h"//Lo ponemos aqui para que quien incluya a ConfigApp pero 
                  //que en realidad busca la configuracion que habia antes en 
                  //ConfigApp respectiva a los transceptores, como hace la pila
                  //de MiWi, pues llegue a ella.

#include "HardwareProfile.h" //Para coger el reloj y la def de B105CNBOARD.

/*****************************************************************************/
/**************************CONFIGURACION HARDWARE*****************************/
/***********************************MIWI**************************************/
/*****************************************************************************/
    #if defined(B105CNBOARD)
        //XXX-Willy. El flag y el enable de la interrupción que genera el
        //transceptor.
        #if defined(__PIC32MX__)
            #if defined(MRF24J40)
//                #define RFIF            IFS0bits.INT1IF //Esto era lo que venia y lo
//                #define RFIE            IEC0bits.INT1IE //de abajo lo que tenia Fern.
                #define RFIF            IFS0bits.INT3IF //Interrupt Flag Status Register.
                #define RFIE            IEC0bits.INT3IE //XXX-Willy//IFS0bits.INT3IF//IEC0bits.INT3IE//IFS0bits.INT3IF.GUILLERMO.
            #elif defined(MRF49XA)
                #define RFIF            IFS0bits.INT1IF //Interrupt Flag Status Register.
                #define RFIE            IEC0bits.INT1IE //XXX-Willy
            #elif defined(MRF49XA_alt)  //XXX-Willy. Para el modulo alternativo.
                #define RFIF            IFS0bits.INT4IF //Change Notification Interrupt Flag Status Register.
                #define RFIE            IEC0bits.INT4IE //XXX-Willy
            #endif

        #else
            #if !defined(MRF89XA)
                #define RFIF            IFS1bits.INT1IF
                #define RFIE            IEC1bits.INT1IE
            #endif
            #define CLOCK_FREQ      8000000
        #endif

        //XXX-Willy. El registro del valor y de dirección del pin donde se
        //concecta la linea de interrupción del transceptor.
        #if defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC32MX__)
            #if defined(MRF89XA)
                #define IRQ1_INT_PIN      PORTEbits.RE9
                #define IRQ1_INT_TRIS     TRISEbits.TRISE9
                #define IRQ0_INT_PIN      PORTEbits.RE8
                #define IRQ0_INT_TRIS     TRISEbits.TRISE8

                //On Explorer 16 with PICtail plus connections we can use both IRQ0 and IRQ1 as interrupt lines
                //instead of using IRQ0 as port pin and IRQ1 as interrupt pin to the PIC
                //Enable USE_IRQ0_AS_INTERRUPT in ConfigMRF89XA.h
            #elif defined(MRF24J40)
                #define RF_INT_PIN      PORTDbits.RD10 //Guillermo. Es a donde esta conectado.
                #define RF_INT_TRIS     TRISDbits.TRISD10//Guillermo.
            #elif defined(MRF49XA)
                #define RF_INT_PIN      PORTDbits.RD8     //Guillermo. Es a donde esta conectado.
                #define RF_INT_TRIS     TRISDbits.TRISD8  //Guillermo.
            #elif defined(MRF49XA_alt)    //XXX-Willy. Para el modulo alternativo.
                #define RF_INT_PIN      PORTDbits.RD11 //Guillermo. Es a donde esta conectado.
                #define RF_INT_TRIS     TRISDbits.TRISD11//Guillermo.
            #endif

        #elif defined(__dsPIC33F__) || defined(__PIC24H__)
            #define RF_INT_PIN      PORTAbits.RA12
            #define RF_INT_TRIS     TRISAbits.TRISA12
        #endif

        //XXX-Willy. El registro del valor y de dirección del pin donde se
        //concecta el Chip Select, Reset, y los tres pines de SPI.
        #if defined(MRF24J40)
            #define PHY_CS              LATEbits.LATE5//Guillermo.LATBbits.LATB2
            #define PHY_CS_TRIS         TRISEbits.TRISE5//Guillermo.TRISBbits.TRISB2
            #define PHY_RESETn          LATEbits.LATE7//Guillermo.LATGbits.LATG2
            #define PHY_RESETn_TRIS     TRISEbits.TRISE7//Guillermo.TRISGbits.TRISG2

            #define SPI_SDI             PORTFbits.RF4//Guillermo.PORTFbits.RF7
            #define SDI_TRIS            TRISFbits.TRISF4//Guillermo.TRISFbits.TRISF7
            #define SPI_SDO             PORTFbits.RF5//Guillermo.LATFbits.LATF8
            #define SDO_TRIS            TRISFbits.TRISF5//Guillermo.TRISFbits.TRISF8
            #define SPI_SCK             PORTBbits.RB14//Guillermo.LATFbits.LATF6
            #define SCK_TRIS            TRISBbits.TRISB14//Guillermo.TRISFbits.TRISF6

        #elif defined(MRF49XA)
            #define PHY_CS              LATEbits.LATE1//Guillermo.MRF49XA
            #define PHY_CS_TRIS         TRISEbits.TRISE1//Guillermo.MRF49XA
            #define PHY_RESETn          LATEbits.LATE4//Guillermo.MRF49XA
            #define PHY_RESETn_TRIS     TRISEbits.TRISE4//Guillermo.MRF49XA

            #define SPI_SDI             PORTGbits.RG7//Guillermo.MRF49XA
            #define SDI_TRIS            TRISGbits.TRISG7//Guillermo.MRF49XA
            #define SPI_SDO             PORTGbits.RG8//Guillermo.MRF49XA
            #define SDO_TRIS            TRISGbits.TRISG8//Guillermo.MRF49XA
            #define SPI_SCK             PORTGbits.RG6//Guillermo.MRF49XA
            #define SCK_TRIS            TRISGbits.TRISG6//Guillermo.MRF49XA

        #elif defined(MRF49XA_alt)
            #define PHY_CS              LATBbits.LATB10   //Guillermo.MRF49XA_alt
            #define PHY_CS_TRIS         TRISBbits.TRISB10 //Guillermo.MRF49XA_alt
            #define PHY_RESETn          LATBbits.LATB15   //Guillermo.MRF49XA_alt
            #define PHY_RESETn_TRIS     TRISBbits.TRISB15 //Guillermo.MRF49XA_alt

            #define SPI_SDI             PORTDbits.RD2     //Guillermo.MRF49XA_alt
            #define SDI_TRIS            TRISDbits.TRISD2  //Guillermo.MRF49XA_alt
            #define SPI_SDO             PORTDbits.RD3     //Guillermo.MRF49XA_alt
            #define SDO_TRIS            TRISDbits.TRISD3  //Guillermo.MRF49XA_alt
            #define SPI_SCK             PORTDbits.RD1     //Guillermo.MRF49XA_alt
            #define SCK_TRIS            TRISDbits.TRISD1  //Guillermo.MRF49XA_alt
        #endif

        //XXX-Willy. El registro del valor y de dirección del pin donde se
        //concecta el FIFO Selector y la Interrupción FIFO.
        #if defined(MRF49XA)
            #define nFSEL           LATEbits.LATE3//Guillermo.MRF49XA
            #define nFSEL_TRIS      TRISEbits.TRISE3//Guillermo.MRF49XA
            #define FINT            PORTBbits.RB2  //Guillermo.MRF49XA
            #define FINT_TRIS       TRISBbits.TRISB2//Guillermo.MRF49XA
        #elif defined(MRF49XA_alt)
            #define nFSEL           LATBbits.LATB11    //Guillermo.MRF49XA_alt
            #define nFSEL_TRIS      TRISBbits.TRISB11  //Guillermo.MRF49XA_alt
            #define FINT            PORTBbits.RB3      //Guillermo.MRF49XA_alt
            #define FINT_TRIS       TRISBbits.TRISB3   //Guillermo.MRF49XA_alt
        #elif defined(MRF24J40)
            #define PHY_WAKE        LATEbits.LATE6//Guillermo.LATGbits.LATG3
            #define PHY_WAKE_TRIS   TRISEbits.TRISE6//TRISGbits.TRISG3
        #elif defined(MRF89XA)
            #if defined(__PIC32MX__)
                #define PHY_IRQ1        IFS0bits.INT2IF
                #define PHY_IRQ1_En     IEC0bits.INT2IE
                #define PHY_IRQ0        IFS0bits.INT1IF
                #define PHY_IRQ0_En     IEC0bits.INT1IE
            #else
                #define PHY_IRQ1        IFS1bits.INT2IF
                #define PHY_IRQ1_En     IEC1bits.INT2IE
                #define PHY_IRQ0        IFS1bits.INT1IF
                #define PHY_IRQ0_En     IEC1bits.INT1IE
            #endif

            #define PHY_IRQ1_TRIS   TRISEbits.TRISE9
            #define Config_nCS      LATBbits.LATB1
            #define Config_nCS_TRIS TRISBbits.TRISB1
            #define Data_nCS        LATBbits.LATB2
            #define Data_nCS_TRIS   TRISBbits.TRISB2
            #define PHY_RESETn      LATGbits.LATG2
            #define PHY_RESETn_TRIS TRISGbits.TRISG2
        #endif

        //XXX-Willy. Hemos redireccinado los leds a pines de los que estan en el submodulo de la placa de Fern
        //llamado "pines general" porque vamos a utilizar donde originalmente estaban los leds para el módulo
        //MRF49XA en su configuración alternativa. En caso de no estar definido dejamos todo como estaba.
        #if defined(MRF49XA_alt) || defined(MRF49XA)
                #define LED_1               LATBbits.LATB6  //Guillermo.LATAbits.LATA7
                #define LED_2               LATBbits.LATB7  //Guillermo.LATAbits.LATA6
                #define LED_1_TRIS          TRISBbits.TRISB6//Guillermo.TRISAbits.TRISA7
                #define LED_2_TRIS          TRISBbits.TRISB7//Guillermo.TRISAbits.TRISA6
        #else
                #define LED_1               LATBbits.LATB6    //Guillermo.LATAbits.LATA7
                #define LED_2               LATBbits.LATB7    //Guillermo.LATAbits.LATA6
                #define LED_1_TRIS          TRISBbits.TRISB6  //Guillermo.TRISAbits.TRISA7
                #define LED_2_TRIS          TRISBbits.TRISB7  //Guillermo.TRISAbits.TRISA6
        #endif

        #define PUSH_BUTTON_1       PORTDbits.RD5   //Guillermo.PORTDbits.RD6
        #define PUSH_BUTTON_2       PORTDbits.RD4   //Guillermo.PORTDbits.RD7
        #define BUTTON_1_TRIS       TRISDbits.TRISD5//Guillermo.TRISDbits.TRISD6
        #define BUTTON_2_TRIS       TRISDbits.TRISD4//Guillermo.TRISDbits.TRISD7

        // Define SUPPORT_TWO_SPI if external EEPROM use the second SPI
        // port alone, not sharing SPI port with the transceiver
        //Guillermo.#define SUPPORT_TWO_SPI

        // External EEPROM SPI chip select pin definition
//        #define EE_nCS_TRIS         TRISDbits.TRISD12 //Guillermo. Tanto esto como lo del Two_SPI está comentado
//        #define EE_nCS              LATDbits.LATD12 //porque aquí no tenemos external EEPROM.

        #define TMRL TMR2


    #endif
/*****************************************************************************/
/************************FIN CONFIGURACION HARDWARE***************************/
/***********************************MIWI**************************************/
/*****************************************************************************/


/*****************************************************************************/
/*******************************FUNCIONES HAL*********************************/
/*****************************************************************************/
///*SPI*/
//#define putcSPI1(data)         SpiChnPutC(SPI_CHANNEL4, data) //Redefino la función con SPI_CHANNEL4 que es donde
//#define getcSPI1()             SpiChnGetC(SPI_CHANNEL4);      //está puesto el módulo MiWi.

/*MIWI*/
#define Misma_EUI(direc1, direc2) isSameAddress(direc1, direc2) //XXX_MIWI

void CRM_HAL_ChanHop(BYTE DestChan); /*La nuesrta propia de salto de canal para
                                      evitar que se notifique al resto de nodos
                                      y obligarles por tanto a cambiar, puesto
                                      que MiWi salta en cuanto recibe la
                                      peticion.*/


extern BYTE tmpSourceLongAddress[MY_ADDRESS_LENGTH]; /*Para saber la direccion larga
                                                      del nodo que nos ha enviado el 
                                                      ultimo paqute.*/


#define ObtenerEUI(a) tmpSourceLongAddress[a]

volatile BOOL DatosDeApp;


/*****************************************************************************/
/*****************************FIN DE FUNCIONES HAL****************************/
/*****************************************************************************/


/******************************************************************************/
/********************************GUILLERMO*************************************/
/******************************************************************************/

#endif
