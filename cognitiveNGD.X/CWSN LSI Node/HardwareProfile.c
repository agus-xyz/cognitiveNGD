/*******************************************************************************
 *              HardwareProfile.c -- Hardware Profile
 *******************************************************************************
 * FileName:        HardwareProfile.c
 * Dependencies:
 * Processor:       PIC18, PIC24, PIC32, dsPIC30, dsPIC33
 * Compiler:        C18 02.20.00 or higher
 *                  C30 02.03.00 or higher
 *                  C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright ? 2007-2010 Microchip Technology Inc.  All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute 
 * Software only when embedded on a Microchip microcontroller or digital 
 * signal controller and used with a Microchip radio frequency transceiver, 
 * which are integrated into your product or third party product (pursuant 
 * to the terms in the accompanying license agreement).   
 *
 * You should refer to the license agreement accompanying this Software for 
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
 * PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
 * LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
 * CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
 * DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
 * ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
 * LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
 * TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
 * NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************
 * File Description:
 *
 * This file provides configuration and basic hardware functionality based on 
 * chosen hardware. It has been adapted for LSI CWSN node from demo boards and
 * examples. Configuration chosen is applied by invoking the initialization
 * functions.
 *
 * Change History:
 *  Rev   Date         Description
 *  0.1   2/17/2009    Initial revision
 *  3.1   5/28/2010    MiWi DE 3.1
 * Author: Juan Domingo Rebollo - Laboratiorio de Sistemas Integrados (LSI-UPM)
 ******************************************************************************/
#include "Compiler.h"
#include "WirelessProtocols/Console.h"
#include "Common/TimeDelay.h"
#include "HardwareProfile.h"

#if defined(__PIC32MX__)
    /* MAX SPI CLOCK FREQ SUPPORTED FOR MIWI TRANSCIEVER */
    #if defined(MRF89XA)
        #define MAX_SPI_CLK_FREQ_FOR_P2P             (1000000)
    #else
        #define MAX_SPI_CLK_FREQ_FOR_P2P             (1000000)
    #endif
    #define        Sleep()      PowerSaveSleep()
#endif

/*******************************************************************************
 * Function:    BoardInit(void)
 * PreCondition:None
 * Input:       None
 * Output:      None
 * Overview:    SPI pins and SFR, Maintenance Tasks Timer, External Interrupts,
 *              and other board issues initialization.
 * Note:        This routine needs to be called before initialising MiWi stack
 *              or invoking other function that operates on MiWi stack.
 ******************************************************************************/
void BoardInit(void){
    #if defined(__PIC32MX__)

    // RADIO INTERFACES & SPI INIT -------------------------------------------//
        #if defined HARDWARE_SPI
            /* Peripheral Bus Frequency = System Clock / PB Divider */
            unsigned int pbFreq;
            pbFreq = (DWORD) CLOCK_FREQ/(1 << mOSCGetPBDIV());

            unsigned int SPI_Clk_Freq;
            unsigned char SPI_Brg;
        #endif

        #if defined MRF24J40
            PHY_CS_TRIS = 0;
            PHY_CS = 1;
            PHY_RESETn_TRIS = 0;
            PHY_RESETn = 1;

            MRF24J40_INT_TRIS = 1;

            SDI_TRIS = 1;
            SDO_TRIS = 0;
            SCK_TRIS = 0;
            SPI_SDO = 0;
            SPI_SCK = 0;

            PHY_WAKE_TRIS = 0;
            PHY_WAKE = 1;

            SPICONCLR = 0xFFFFFFFF;             // Clear SPIxCON register

            #ifdef HARDWARE_SPI
                /* Enable SPI, Set to Master Mode & Set CKE bit : Serial output
                 * data changes on transition from active clock state to Idle
                 * clock state */
                SPICON = 0x00008120;

                /* PB Frequency can be maximum 40 MHz */
                if(pbFreq > (2 * MAX_SPI_CLK_FREQ_FOR_P2P)){
                    SPI_Brg = 1;
                    /* Continue the loop till you find SPI Baud Rate Reg Value */
                    while(1){
                        /* SPI Clock Calculation as per PIC32 Manual */
                        SPI_Clk_Freq = pbFreq / (2 * (SPI_Brg + 1));

                        if(SPI_Clk_Freq <= MAX_SPI_CLK_FREQ_FOR_P2P){
                            break;
                        }
                        SPI_Brg++;
                    }
                    #if defined MRF24J40_IN_SPI1
                    mSpiChnSetBrg (1, SPI_Brg);
                    #elif defined MRF24J40_IN_SPI2
                    mSpiChnSetBrg (2, SPI_Brg);
                    #elif defined MRF24J40_IN_SPI3
                        mSpiChnSetBrg (1A, SPI_Brg);
                    #elif defined MRF24J40_IN_SPI4
                        mSpiChnSetBrg (3A, SPI_Brg);
                    #endif
               }
               else{
                    #if defined MRF24J40_IN_SPI1
                    mSpiChnSetBrg (1, 0);
                    #elif defined MRF24J40_IN_SPI2
                    mSpiChnSetBrg (2, 0);
                    #elif defined MRF24J40_IN_SPI3
                        mSpiChnSetBrg (1A, 0);
                    #elif defined MRF24J40_IN_SPI4
                        mSpiChnSetBrg (3A, SPI_Brg);
                    #endif
               }
            #endif
        #endif
        #if defined(MRF49XA_1)
            //Configuration for Guilja's Expansion Board, Connection SLot 1 --//
            mPORTESetPinsDigitalOut(BIT_1); //nCS
            mPORTBSetPinsDigitalIn(BIT_2);  //FINT      //Juan: Added.
            //----------------------------------------------------------------//

            MRF49XA_1_PHY_CS_TRIS = 0;
            MRF49XA_1_PHY_CS = 1;
            MRF49XA_1_PHY_RESETn_TRIS = 0;
            MRF49XA_1_PHY_RESETn = 1;

            MRF49XA_1_INT_TRIS = 1;

            MRF49XA_1_SDI_TRIS = 1;
            MRF49XA_1_SDO_TRIS = 0;
            MRF49XA_1_SCK_TRIS = 0;
            MRF49XA_1_SPI_SDO = 0;
            MRF49XA_1_SPI_SCK = 0;

            MRF49XA_1_nFSEL_TRIS = 0;
            MRF49XA_1_FINT_TRIS = 1;
            MRF49XA_1_nFSEL = 1;          // nFSEL inactive

            MRF49XA_1_SPICONCLR = 0xFFFFFFFF;       //Clear SPIxCON register

            #ifdef HARDWARE_SPI
                /* Enable SPI1, Set to Master Mode & Set CKE bit : Serial output
                * data changes on transition from active clock state to Idle
                * clock state */
                MRF49XA_1_SPICON = 0x00008120;

                /* PB Frequency can be maximum 40 MHz */
                if(pbFreq > (2 * MAX_SPI_CLK_FREQ_FOR_P2P)){
                    SPI_Brg = 1;
                    /* Continue the loop till you find SPI Baud Rate Reg Value */
                    while(1){
                        /* SPI Clock Calculation as per PIC32 Manual */
                        SPI_Clk_Freq = pbFreq / (2 * (SPI_Brg + 1));
                        if(SPI_Clk_Freq <= MAX_SPI_CLK_FREQ_FOR_P2P){
                            break;
                        }
                        SPI_Brg++;
                    }
                    #if defined MRF49XA_1_IN_SPI1
                    mSpiChnSetBrg (1, SPI_Brg);
                    #elif defined MRF49XA_1_IN_SPI2
                    mSpiChnSetBrg (2, SPI_Brg);
                    #elif defined MRF49XA_1_IN_SPI3
                        mSpiChnSetBrg (1A, SPI_Brg);
                    #endif
               }
               else{
                    #if defined MRF49XA_1_IN_SPI1
                    mSpiChnSetBrg (1, 0);
                    #elif defined MRF49XA_1_IN_SPI2
                    mSpiChnSetBrg (2, 0);
                    #elif defined MRF49XA_1_IN_SPI3
                        mSpiChnSetBrg (1A, 0);
                    #endif
               }
            #endif
        #endif
        #if defined(MRF49XA_2)
            MRF49XA_2_PHY_CS_TRIS = 0;
            MRF49XA_2_PHY_CS = 1;
            MRF49XA_2_PHY_RESETn_TRIS = 0;
            MRF49XA_2_PHY_RESETn = 1;

            MRF49XA_2_INT_TRIS = 1;

            MRF49XA_2_SDI_TRIS = 1;
            MRF49XA_2_SDO_TRIS = 0;
            MRF49XA_2_SCK_TRIS = 0;
            MRF49XA_2_SPI_SDO = 0;
            MRF49XA_2_SPI_SCK = 0;

            MRF49XA_2_nFSEL_TRIS = 0;
            MRF49XA_2_FINT_TRIS = 1;
            MRF49XA_2_nFSEL = 1;          // nFSEL inactive

            MRF49XA_2_SPICONCLR = 0xFFFFFFFF;       // Clear SPIxCON register

            #ifdef HARDWARE_SPI
                /* Enable SPI1, Set to Master Mode & Set CKE bit : Serial output
                * data changes on transition from active clock state to Idle
                * clock state */
                MRF49XA_2_SPICON = 0x00008120;

                /* PB Frequency can be maximum 40 MHz */
                if(pbFreq > (2 * MAX_SPI_CLK_FREQ_FOR_P2P)){
                    SPI_Brg = 1;
                    /* Continue the loop till you find SPI Baud Rate Reg Value */
                    while(1){
                        /* SPI Clock Calculation as per PIC32 Manual */
                        SPI_Clk_Freq = pbFreq / (2 * (SPI_Brg + 1));
                        if(SPI_Clk_Freq <= MAX_SPI_CLK_FREQ_FOR_P2P){
                            break;
                        }
                        SPI_Brg++;
                    }
                    #if defined MRF49XA_2_IN_SPI1
                    mSpiChnSetBrg (1, SPI_Brg);
                    #elif defined MRF49XA_2_IN_SPI2
                    mSpiChnSetBrg (2, SPI_Brg);
                    #elif defined MRF49XA_2_IN_SPI3
                        mSpiChnSetBrg (1A, SPI_Brg);
                    #endif
               }
               else{
                    #if defined MRF49XA_2_IN_SPI1
                    mSpiChnSetBrg (1, 0);
                    #elif defined MRF49XA_2_IN_SPI2
                    mSpiChnSetBrg (2, 0);
                    #elif defined MRF49XA_2_IN_SPI3
                        mSpiChnSetBrg (1A, 0);
                    #endif
               }
            #endif
        #endif
       

    // SPI & EXTERNAL INTERRUPTS PINS AND CONFIGURATION ----------------------//
#if (defined __32MX675F256L__ || defined ____32MX675F512__)
        /* Set the SPI Port Directions (SDO, SDI, SCK) for every SPI module.*/
            #if defined MRF49XA_1_IN_SPI1 || defined MRF49XA_2_IN_SPI1 || \
                defined MRF89XA_IN_SPI1   || defined MRF24J40_IN_SPI1  || \
                defined MRF24WB0M_IN_SPI1
                mPORTDSetPinsDigitalOut(BIT_0);     //SDO1
                mPORTDSetPinsDigitalOut(BIT_10);    //SCK1
                mPORTCSetPinsDigitalIn(BIT_4);      //SDI1
            #endif
            #if defined MRF49XA_1_IN_SPI2 || defined MRF49XA_2_IN_SPI2 || \
                defined MRF89XA_IN_SPI2   || defined MRF24J40_IN_SPI2  || \
                defined MRF24WB0M
                mPORTGSetPinsDigitalOut(BIT_8);     //SDO2
                mPORTGSetPinsDigitalOut(BIT_6);     //SCK2
                mPORTGSetPinsDigitalIn(BIT_7);      //SDI2
            #endif
            #if defined MRF49XA_1_IN_SPI3 || defined MRF49XA_2_IN_SPI3 || \
                defined MRF89XA_IN_SPI3   || defined MRF24J40_IN_SPI3  || \
                defined MRF24WB0M
                mPORTFSetPinsDigitalOut(BIT_8);     //SDO3
                mPORTDSetPinsDigitalOut(BIT_15);    //SCK3
                mPORTFSetPinsDigitalIn(BIT_2);      //SDI3
            #endif
//            #if defined MRF24WB0M_IN_SPI4
//                mPORTFSetPinsDigitalOut(BIT_5);     //SDO4
//                mPORTFSetPinsDigitalOut(BIT_13);    //SCK4
//                mPORTFSetPinsDigitalIn(BIT_4);      //SDI4
//            #endif

   #endif
#endif
       

    // TIMER 1 FOR TIME_SYNC -------------------------------------------------//
        #if defined(ENABLE_TIME_SYNC)   
        //TIMER 1 MAY BE USED FOR SLEEP MODE AND/OR FOR STACKS MAINTENANCE. IT
        //NEEDS ADAPTATION BEFORE ENABLING TIME_SYNC WITH TIMER 1 TOO!
            T1CON = 0;
            T1CON = 0x0012;
            T1CONSET = 0x8000;
            PR1 = 0xFFFF;
            IFS0bits.T1IF = 0;

            mT1IntEnable(1);
            mT1SetIntPriority(4);

            while(T1CONbits.TWIP);
            TMR1 = 0;
        #endif
    // TIMER 1 FOR NODE STACKS AUTO-MAINTENANCE ------------------------------//
        #if defined NODE_DOES_MAINTENANCE_TASKS
            T1CON = 0x0070;             //Disable timer, PBCLK source, PS=256
            TMR1  = 0x0000;             //Reset count
            PR1   = MAINTENANCE_PERIOD; //Set period.

            IPC1SET = 0x00000005;   //Set Priority level 1, Subpriority level 1
            IFS0CLR = 0x00000010;   //Clear T1IF
            IEC0SET = 0x00000010;   //Set T1IE
            //Timer will be triggered after initialization.
        #endif

    // IOPORT CN - For waking up the node manually. --------------------------//
        mPORTDSetPinsDigitalIn(BIT_5); // CN14
        CNCON = 0x8000;         //Module enabled.
        CNEN = 0x00004000;      //Enable CN14
        CNPUE = 0x00004000;     //Enable CN14 weak pull-up.
        ReadBUTTONS();          //Clear PORT mismatch condition.
        IFS1CLR = 0x00000001;   //Clear the CN interrupt flag status bit
        IPC6SET = 0x00180000;   //Set CN priority 6, subpriority 0.
        //It will be enabled only during sleep mode time interval
    //------------------------------------------------------------------------//


        																										// Lo modifico en el wifi config
        #if defined(ENABLE_NVM)     //REVIEW
            //EE_nCS_TRIS = 0;//FERNANDO, CUIDADO NO SE SI LA PILA REALMENTE FUNCIONA CON FLASH MEMORY
            //EE_nCS = 1;
        #endif

    // INTERRUPTION FLAGS AND EXT_INT PIN FINAL SETTINGS ---------------------//
        #if defined MRF49XA_1
            MRF49XA_1_IF = 0;
            if(MRF49XA_1_INT_PIN == 0){
                MRF49XA_1_IF = 1;
            }
        #endif
        #if defined MRF49XA_2
            MRF49XA_2_IF = 0;
            if(MRF49XA_2_INT_PIN == 0){
                MRF49XA_2_IF = 1;
            }
        #endif
        #if defined MRF89XA
            PHY_IRQ1 = 0;
        #endif
        #if defined MRF24J40
            MRF24J40_IF = 0;
            if(MRF24J40_INT_PIN == 0){
                MRF24J40_IF = 1;
            }
        #endif
}

#if defined ENABLE_CONSOLE
void WriteStringPC(const char *string){
    while (*string != '\0'){    // If next char is not the end of the string.
        while(TxNotRdyUART());  // Wait for Debug UART Tx buffer to accept more data.
        WriteUART(*string);     // Write a single char data.
        string++;               // Prepare for next data
        while(BusyUART());      // Wait for the transmission to finish.
    }
}
#endif