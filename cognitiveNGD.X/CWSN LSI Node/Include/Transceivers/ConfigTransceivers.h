/*******************************************************************************
* FileName:	ConfigTransceivers.h    ¡¡¡NOT INCLUDED IN THE ORIGINAL STACK!!!
* Dependencies:
* Processor:
* Compiler:     Microchip C18 v3.04 or higher
*               Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher
* Company:	Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
*
* Copyright ? 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute
* Software only when embedded on a Microchip microcontroller or digital signal
* controller and used with a Microchip radio frequency transceiver, which are
* integrated into your product or third party product (pursuant to the terms in
* the accompanying license agreement).
*
* You should refer to the license agreement accompanying this Software for
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
* MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
* CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
* OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
* INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
* SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
* (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
********************************************************************************
* File Description:
*
* Microchip MiWi Stack variation for including several Microchip RF transceivers
* simultaneously in order to use different frequency bands.
* This file is where original "ConfigMRF24J40.h" and other transceiver config
* files within MiWi stack were MERGED IN A UNIQUE CONFIGURATION FILE.
* In this file you can enable/disable configuration options (most of them) for
* all MiWi transceivers considered in this design.
*
* See Also:
* MiWi protocols and transceivers are defined in "ConfigApp.h" file.
* MiWi tranceivers' definitions and common issues are all in "Transceivers.h"
* file.
*
* Original MiWi Stack:      Rev 4.1   6/3/2011.
* Author:  Juan Domingo Rebollo - LSI - Laboratorio de Sistemas Intergados - UPM
*******************************************************************************/
#if !defined __CONFIG_TRANSCEIVERS_H
    #define __CONFIG_TRANSCEIVERS_H
#endif

#include "HardwareProfile.h"
#include "WirelessProtocols/ConfigApp.h"
#include "Transceivers/Security.h"


//----------------------------------------------------------------------------//
//------------------------------- GENERAL ------------------------------------//
//----------------------------------------------------------------------------//

/******************************************************************************/
// SECURITY_KEY_xx defines xxth byte of security key used in the block cipher
/******************************************************************************/
#define SECURITY_KEY_00 0x00
#define SECURITY_KEY_01 0x01
#define SECURITY_KEY_02 0x02
#define SECURITY_KEY_03 0x03
#define SECURITY_KEY_04 0x04
#define SECURITY_KEY_05 0x05
#define SECURITY_KEY_06 0x06
#define SECURITY_KEY_07 0x07
#define SECURITY_KEY_08 0x08
#define SECURITY_KEY_09 0x09
#define SECURITY_KEY_10 0x0a
#define SECURITY_KEY_11 0x0b
#define SECURITY_KEY_12 0x0c
#define SECURITY_KEY_13 0x0d
#define SECURITY_KEY_14 0x0e
#define SECURITY_KEY_15 0x0f

/******************************************************************************/
// KEY_SEQUENCE_NUMBER defines the sequence number that is used to identify the
// key. Different key should have different sequence number, if multiple
// security keys are used in the application.
/******************************************************************************/
#define KEY_SEQUENCE_NUMBER 0x00

/******************************************************************************/
// FRAME_COUNTER_UPDATE_INTERVAL defines the NVM update interval for frame
// counter, when security is enabled. The same interval will be added to the
// frame counter read from NVM when Network Freezer feature is enabled.
/******************************************************************************/
#define FRAME_COUNTER_UPDATE_INTERVAL 1024

/******************************************************************************/
// BANK_SIZE defines the number of packet can be received and stored to wait for
// handling in MiMAC layer.
/******************************************************************************/
#define BANK_SIZE   1


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------//
//------------------------------ MRF49XA_0434 -----------------------------------//
//----------------------------------------------------------------------------//
#if defined MIWI_0434_RI
    /**************************************************************************/
    // FREQUENCY BAND: BAND_915, BAND_868 or BAND_434 are three supported
    // frequency bands for Microchip MRF49XA. Only one of them must be defined.
    // As long as there might be 2 MRF49XAs, the variables definitions can't be
    // named as in MiWi original stack in order to make the distinction. In the
    // case of 2 MRF49XA transceivers, they must define different bands.
    /**************************************************************************/
    #if !defined MRF49XA_0434
        #define MRF49XA_0434
    #endif
    /**************************************************************************/
    // DATA RATE: DATA_RATE_1200, DATA_RATE_9600, DATA_RATE_19200,
    // DATA_RATE_38400, DATA_RATE_57600 and DATA_RATE_115200 are six data rates
    // supported by Microchip MRF49XA transceivers in MiMAC interface. Only one
    // of them must be defined.
    /**************************************************************************/
    //#define MRF49XA_1_RATE_1200       //Juan: ¿Needs ACK timeout readjusting?
    //#define MRF49XA_1_RATE_9600
    //#define MRF49XA_1_RATE_19200
    //#define MRF49XA_1_RATE_38400
    //#define MRF49XA_1_RATE_57600
    #define MRF49XA_1_RATE_115200
        //DATA RATE VALIDATION
        #if !defined(MRF49XA_1_RATE_1200) && !defined(MRF49XA_1_RATE_9600) \
         && !defined(MRF49XA_1_RATE_19200)&& !defined(MRF49XA_1_RATE_38400)\
         && !defined(MRF49XA_1_RATE_57600)&& !defined(MRF49XA_1_RATE_115200)
            #error "Data rate must be defined for MRF49XA_1"
        #endif
        #undef MRF49XA_1_DR_DEFINED
        #if defined(MRF49XA_1_RATE_1200)
            #define MRF49XA_1_DR_DEFINED
        #endif
        #if defined(MRF49XA_1_RATE_9600)
            #if defined(MRF49XA_1_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_1"
            #endif
            #define MRF49XA_1_DR_DEFINED
        #endif
        #if defined(MRF49XA_1_RATE_19200)
            #if defined(MRF49XA_1_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_1"
            #endif
            #define MRF49XA_1_DR_DEFINED
        #endif
        #if defined(MRF49XA_1_RATE_38400)
            #if defined(MRF49XA_1_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_1"
            #endif
            #define MRF49XA_1_DR_DEFINED
        #endif
        #if defined(MRF49XA_1_RATE_57600)
            #if defined(MRF49XA_1_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_1"
            #endif
            #define MRF49XA_1_DR_DEFINED
        #endif
        #if defined(MRF49XA_1_RATE_115200)
            #if defined(MRF49XA_1_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_1"
            #endif
            #define MRF49XA_1_DR_DEFINED
        #endif
        #undef MRF49XA_1_DR_DEFINED
#endif

//----------------------------------------------------------------------------//
//------------------------------ MRF49XA_0868 -----------------------------------//
//----------------------------------------------------------------------------//
//ONLY FOR HAVING TWO MRF49XAs
#if defined MIWI_0868_RI
    /**************************************************************************/
    // FREQUENCY BAND: BAND_915, BAND_868 or BAND_434 are three supported
    // frequency bands for Microchip MRF49XA. Only one of them must be defined.
    // As long as there might be 2 MRF49XAs, the variables definitions can't be
    // named as in MiWi original stack in order to make the distinction. In the
    // case of 2 MRF49XA transceivers, they must define different bands.
    /**************************************************************************/
    #if !defined MRF49XA_0868
        #define MRF49XA_0868
    #endif
    /**************************************************************************/
    // DATA RATE: DATA_RATE_1200, DATA_RATE_9600, DATA_RATE_19200,
    // DATA_RATE_38400, DATA_RATE_57600 and DATA_RATE_115200 are six data rates
    // supported by Microchip MRF49XA transceivers in MiMAC interface. Only one
    // of them must be defined.
    /**************************************************************************/
    //#define MRF49XA_2_RATE_1200
    #define MRF49XA_2_RATE_9600
    //#define MRF49XA_2_RATE_19200
    //#define MRF49XA_2_RATE_38400
    //#define MRF49XA_2_RATE_57600
    //#define MRF49XA_2_RATE_115200
        //DATA RATE VALIDATION
        #if !defined(MRF49XA_2_RATE_1200) && !defined(MRF49XA_2_RATE_9600) \
         && !defined(MRF49XA_2_RATE_19200)&& !defined(MRF49XA_2_RATE_38400)\
         && !defined(MRF49XA_2_RATE_57600)&& !defined(MRF49XA_2_RATE_115200)
            #error "Data rate must be defined for MRF49XA_2"
        #endif
        #undef MRF49XA_2_DR_DEFINED
        #if defined(MRF49XA_2_RATE_1200)
            #define MRF49XA_2_DR_DEFINED
        #endif
        #if defined(MRF49XA_2_RATE_9600)
            #if defined(MRF49XA_2_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_2"
            #endif
            #define MRF49XA_2_DR_DEFINED
        #endif
        #if defined(MRF49XA_2_RATE_19200)
            #if defined(MRF49XA_2_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_2"
            #endif
            #define MRF49XA_2_DR_DEFINED
        #endif
        #if defined(MRF49XA_2_RATE_38400)
            #if defined(MRF49XA_2_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_2"
            #endif
            #define MRF49XA_2_DR_DEFINED
        #endif
        #if defined(MRF49XA_2_RATE_57600)
            #if defined(MRF49XA_2_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_2"
            #endif
            #define MRF49XA_2_DR_DEFINED
        #endif
        #if defined(MRF49XA_2_RATE_115200)
            #if defined(MRF49XA_2_DR_DEFINED)
                #error "Only one data rate can be defined for MRF49XA_2"
            #endif
            #define MRF49XA_2_DR_DEFINED
        #endif
        #undef MRF49XA_2_DR_DEFINED
#endif

//----------------------------------------------------------------------------//
//-------------------- CONFIGURATION OPTIONS FOR MRF24J40 --------------------//
//----------------------------------------------------------------------------//
    #if defined (MIWI_2400_RI)
        /**********************************************************************/
        // TURBO_MODE enables MRF24J40 transceiver to perform the communication
        // in proprietary modulation, which is not compliant to IEEE 802.15.4
        // specification. The data rate at turbo mode is up to 625Kbps.
        /**********************************************************************/
        //#define TURBO_MODE

        /**********************************************************************/
        // VERIFY_TRANSMIT configures the MRF24J40 transceiver to transmit data
        // in a block procedure, which ensures finish transmission before
        // continue other task. This block procedure ensures the delivery state
        // of transmitting known to the upper protocol layer, thus may be
        // necessary to detect transmission failure. However, this block
        // procedure slightly lower the throughput
        /*********************************************************************/
        #define VERIFY_TRANSMIT

        /**********************************************************************/
        // If MRF24J40MB module with external power amplifier and low noise
        // amplifier has been used, the stack needs to do output power
        // adjustment according to MRF24J40MB data sheet. Comment this part if
        // used other design of MRF24J40 with external PA and/or LNA. This
        // definition cannot be used with definition of MRF24J40MC
        /**********************************************************************/
        //#define MRF24J40MB

        /**********************************************************************/
        // If MRF24J40MC module with external power amplifier, low noise
        // amplifier and external antenna has been used, the stack needs to do
        // output power adjustment according to MRF24J40MC data sheet. Comment
        // this part if used other design of MRF24J40 with external PA and/or
        // LNA. This definition cannot be used with definition of MRF24J40MB
        /**********************************************************************/
        //#define MRF24J40MC

        /**********************************************************************/
        // SECURITY_LEVEL defines the security mode used in the application
        /**********************************************************************/
        #define MRF24J40_SEC_LEVEL_CTR           0
        #define MRF24J40_SEC_LEVEL_CBC_MAC_32    1
        #define MRF24J40_SEC_LEVEL_CBC_MAC_64    2
        #define MRF24J40_SEC_LEVEL_CBC_MAC_128   3
        #define MRF24J40_SEC_LEVEL_CCM_32        4
        #define MRF24J40_SEC_LEVEL_CCM_64        5
        #define MRF24J40_SEC_LEVEL_CCM_128       6

        #define MRF24J40_SECURITY_LEVEL MRF24J40_SEC_LEVEL_CCM_32

        #if defined(MRF24J40MB) && defined(MRF24J40MC)
            #error "MRF24J40MB and MRF24J40MC cannot be defined at the same time"
        #endif
        #if defined(MRF24J40MB) || defined(MRF24J40MC)
            #define UNDEFINED_LOCATION  0x00
            #define UNITED_STATES       0x01
            #define CANADA              0x02
            #define EUROPE              0x03
            /******************************************************************/
            // If MRF24J40MB/C module is used, the output power setting depends
            // on the country where the application is used. Define one of the
            // locations that this appliation will be applied. If none of the
            // location is set, US FCC setting for MRF24J40MB/C module will be
            // used in the stack. Check MRF24J40MB/C data sheet for details.
            /******************************************************************/
            #define APPLICATION_SITE    EUROPE
        #endif
    #endif

//----------------------------------------------------------------------------//
//------- COMMON OPTIONS AVAILABLE FOR MRF49XA TRANSCEIVERS --------//
//----------------------------------------------------------------------------//
//For simplicity and smaller stack size, they're applied to MRF49XAs and MRF89XA
    #if defined(MRF49XA_0434) || defined(MRF49XA_0868)

        /**********************************************************************/
        // ENABLE_CCA enables transceivers (49 or 89) to perform Clear Channel
        // Assessement before transmitting data in MiMAC layer.
        /**********************************************************************/
        #define ENABLE_CCA

        /**********************************************************************/
        // ENABLE_ACK enables transceivers (49 or 89) to automatically send back
        // an acknowledgement packet in MiMAC layer after receiving a packet,
        // when such acknowledgement is requested by the packet sender.
        /**********************************************************************/
        #define ENABLE_ACK

        /**********************************************************************/
        // ENABLE_RETRANSMISSION enables transceivers (49 or 89) to retransmit
        // the packet up to RETRANSMISSION_TIMES, if ENABLE_ACK is defined, and
        // a proper acknowledgement packet is not received by the sender in
        // predefined time period.
        /**********************************************************************/
        #define ENABLE_RETRANSMISSION

        /**********************************************************************/
        // SOURCE_ADDRESS_ABSENT disable the stack to transmit the source
        // address in the MAC layer, if the destination does not care where the
        // message comes from. This feature is highly application dependent.
        // This feature is only available for transceivers that support MiMAC
        // frame format.
        /**********************************************************************/
        //#define SOURCE_ADDRESS_ABSENT

        /**********************************************************************/
        // MAX_ALLOWED_TX_FAILURE defines the maximum number of tries to
        // transmit a packet before a transmission failure can be issued to the
        // upper protocol layer. Transmission failure under this condition
        // usually due to timeout from MRF49XA pin switch.
        /**********************************************************************/
        #define MAX_ALLOWED_TX_FAILURE  20

        /**********************************************************************/
        // RETRANSMISSION_TIMES defines the maximum retries that can be
        // performed if a proper acknowledgement packet is not received in
        // predefined time period, if ENABLE_RETRANSMISSION is defined.
        /**********************************************************************/
        #define RETRANSMISSION_TIMES    3

        /**********************************************************************/
        // CCA_TIMES defines the total number of Clear Channel Assessment in the
        // CCA procedure. CCA procedure perform CCA for CCA_TIMES and check if
        // the times of CCA failure beyond the number defined in CCA_THRESHOLD.
        // In the case that CCA failure times is beyond CCA_THRESHOLD, the whole
        // procedure must be repeated up to CCA_RETRIES times before
        // transmission failure can be flagged.
        /**********************************************************************/
        #define CCA_TIMES               5

        /**********************************************************************/
        // CCA_RETRIES defines the maximum retries can be performed in the case
        // of Clear Channel Assessment failure in the CCA procedure. CCA
        // procedure perform CCA for CCA_TIMES and check if the times of CCA
        // failure beyond the number defined in CCA_THRESHOLD. In the case that
        // CCA failure times is beyond CCA_THRESHOLD, the whole procedure must
        // be repeated up to CCA_RETRIES times before transmission failure can
        // be flagged.
        /**********************************************************************/
        #define CCA_RETRIES             4

        /**********************************************************************/
        // ACK_INFO_SIZE defines the number of acknowledgement information
        // structure can be stored to avoid duplicate packet to the protocol
        // layer.
        /**********************************************************************/
        #define ACK_INFO_SIZE           5

        /**********************************************************************/
        // SECURITY_LEVEL defines the security mode used in the application.
        /**********************************************************************/
        #define SECURITY_LEVEL SEC_LEVEL_CCM_16
     /**********************************************************************/
        // INFER_DEST_ADDRESS enables inferred destination address mode, which
        // does not transmit the destination address, but depends on the
        // software CRC to infer the destination address. Infer destination
        // address applies to only transceivers that support MiMAC frame format
        // and the CRC engine that supports this feature.
        /**********************************************************************/
        //#define INFER_DEST_ADDRESS

        /**********************************************************************/
        // CCA_THRESHOLD defines the threshold times of Clear Channel Assessment
        // failure in the CCA procedure. CCA procedure perform CCA for CCA_TIMES
        // and check if the times of CCA failure beyond the number defined in
        // CCA_THRESHOLD. In the case that CCA failure times is beyond
        // CCA_THRESHOLD, the whole procedure must be repeated up to CCA_RETRIES
        // times before transmission failure can be flagged.
        /**********************************************************************/
        #define MRF49XA_CCA_THRESHOLD           2

        /*********************************************************************/
        // XTAL_LD_CAP defines the capacitor load on the external crystal
        // as the clock to MRF49XA transceiver
        /*********************************************************************/
            #define XTAL_LD_CAP_85                  0x0000
            #define XTAL_LD_CAP_9                   0x0001
            #define XTAL_LD_CAP_95                  0x0002
            #define XTAL_LD_CAP_10                  0x0003
            #define XTAL_LD_CAP_105                 0x0004
            #define XTAL_LD_CAP_11                  0x0005
            #define XTAL_LD_CAP_115                 0x0006
            #define XTAL_LD_CAP_12                  0x0007
            #define XTAL_LD_CAP_125                 0x0008
            #define XTAL_LD_CAP_13                  0x0009
            #define XTAL_LD_CAP_135                 0x000A
            #define XTAL_LD_CAP_14                  0x000B
            #define XTAL_LD_CAP_145                 0x000C
            #define XTAL_LD_CAP_15                  0x000D
            #define XTAL_LD_CAP_155                 0x000E
            #define XTAL_LD_CAP_16                  0x000F

        #define XTAL_LD_CAP     XTAL_LD_CAP_10

        /*********************************************************************/
        // CRYSTAL_PPM defines the accuracy of the external crystal in PPM
        /*********************************************************************/
        #define CRYSTAL_PPM     10

        /*********************************************************************/
        // LNA_GAIN defines the internal low noise amplifier gain for
        // MRF49XA transceiver.
        /*********************************************************************/
            #define LNA_GAIN_0_DB                   0x0000
            #define LNA_GAIN_N_6_DB                 0x0008
            #define LNA_GAIN_N_14_DB                0x0010
            #define LNA_GAIN_N_20_DB                0x0018

        #define MRF49XA_LNA_GAIN        LNA_GAIN_0_DB

        /*********************************************************************/
        // TX_POWER defines the output power for MRF49XA
        /*********************************************************************/
            #define TX_POWER_0_DB                   0x0000
            #define TX_POWER_N_2_5_DB               0x0001
            #define TX_POWER_N_5_DB                 0x0002
            #define TX_POWER_N_7_5_DB               0x0003
            #define TX_POWER_N_10_DB                0x0004
            #define TX_POWER_N_12_5_DB              0x0005
            #define TX_POWER_N_15_DB                0x0006
            #define TX_POWER_N_17_5_DB              0x0007

        #define MRF49XA_TX_POWER        TX_POWER_0_DB

        /*********************************************************************/
        // RSSI_THRESHOLD defines the threshold for the RSSI digital output
        /*********************************************************************/
            #define RSSI_THRESHOLD_103              0x0000
            #define RSSI_THRESHOLD_97               0x0001
            #define RSSI_THRESHOLD_91               0x0002
            #define RSSI_THRESHOLD_85               0x0003
            #define RSSI_THRESHOLD_79               0x0004
            #define RSSI_THRESHOLD_73               0x0005

        #define MRF49XA_RSSI_THRESHOLD  RSSI_THRESHOLD_79

    #endif
//EOF