/*****************************************************************************
 *
 *              ConfigCommModule.h -- Communication Module Configuration v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        ConfigCommModule.h
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
 * Este archivo configura el modulo Communication Module (CR Module).
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   23/10/2012    Initial revision
 *****************************************************************************/

#ifndef  _CONFIGHAL_H_
#define  _CONFIGHAL_H_



/*****************************************************************************/
/**************************CONFIGURACION SOFTWARE*****************************/
/***********************************MIWI**************************************/
/*****************************************************************************/

/*****************************************************************************/
/*Unas definiciones tanto para el software como para el hardware pero siempre
 relacionado con la configuracion de como queremos que actuen los
 transceptores y por eso lo pones en la capa de abstraccion hardware.*/
/*****************************************************************************/
    /******************************GUILLERMO******************************/
    //Define si la plataforma actuará como nodo1 o como nodo2
    #define Nodo1 //Guillermo. ENABLE SLEEP debe estar DEShabitilitado
//    #define Nodo2
    #define VaciaBuffer //XXX-Willy. Para vaciar el Buffer grande en el
                         //que vamos a meter los datos que nos vayan
                         //llegando. Comentado si Tx, si Rx lo descomentamos.
//    #define USB_DEBUG //XXX-Willy. Para imprimir las "trazas" de debugeo por
                      //el USB en vez de la UART.
#define DATA_OVER_VCC //XXX-Guilja para enviar datos de app sobre el VCC.

    /****************************FIN*GUILLERMO****************************/

    /*********************************************************************/
    // ENABLE_CONSOLE will enable the print out on the hyper terminal
    // this definition is very helpful in the debugging process
    /*********************************************************************/
    #define ENABLE_CONSOLE

    /*********************************************************************/
    // HARDWARE_SPI enables the hardware SPI implementation on MCU
    // silicon. If HARDWARE_SPI is not defined, digital I/O pins will
    // be used to bit-bang the RF transceiver
    /*********************************************************************/
    #define HARDWARE_SPI

    //------------------------------------------------------------------------
    // Definition of Protocol Stack. ONLY ONE PROTOCOL STACK CAN BE CHOSEN
    //------------------------------------------------------------------------
        /*********************************************************************/
        // PROTOCOL_P2P enables the application to use MiWi P2P stack. This
        // definition cannot be defined with PROTOCOL_MIWI.
        /*********************************************************************/
        //#define PROTOCOL_P2P

        /*********************************************************************/
        // PROTOCOL_MIWI enables the application to use MiWi mesh networking
        // stack. This definition cannot be defined with PROTOCOL_P2P.
        /*********************************************************************/
        #define PROTOCOL_MIWI


        /*********************************************************************/
        // PROTOCOL_MIWI_PRO enables the application to use MiWi PRO stack.
        // This definition cannot be defined with PROTOCOL_P2P or PROTOCOL_MIWI.
        /*********************************************************************/
        //#define PROTOCOL_MIWI_PRO

            /*********************************************************************/
            // NWK_ROLE_COORDINATOR is not valid if PROTOCOL_P2P is defined. It
            // specified that the node has the capability to be coordinator or PAN
            // coordinator. This definition cannot be defined with
                // NWK_ROLE_END_DEVICE.
            /*********************************************************************/
            //#define NWK_ROLE_COORDINATOR //Guillermo.

            /****************************GUILLERMO********************************/
            //Antes era siempre Coordinator y entonces el dispositivo que dormía, al
            //despertar daba el triple Data Request Failed y se ponía a resync y
            //nunca volvía a encontrar el coordinador. Al fin y al cabo según el
            //protocolo el coordindador no puede dormir y lo que ocurría en Nodo2
            //era que teníamos un coordinador y a la vez el enable_sleep. Así que
            //ahora parece que todo funciona correctamente.
            #if defined(Nodo1)
                #define NWK_ROLE_COORDINATOR
            #elif defined(Nodo2)
                #define NWK_ROLE_END_DEVICE
            #endif
            /**************************FIN GUILLERMO*******************************/


    //------------------------------------------------------------------------
    // Definition of RF Transceiver. ONLY ONE TRANSCEIVER CAN BE CHOSEN
    //------------------------------------------------------------------------

        /*********************************************************************/
        // Definition of MRF24J40 enables the application to use Microchip
        // MRF24J40 2.4GHz IEEE 802.15.4 compliant RF transceiver. Only one
        // RF transceiver can be defined.
        /*********************************************************************/
        #define MRF24J40


        /*********************************************************************/
        // Definition of MRF49XA enables the application to use Microchip
        // MRF49XA subGHz proprietary RF transceiver. Only one RF transceiver
        // can be defined.
        /*********************************************************************/
//        #define MRF49XA
//        #define MRF49XA_alt //Para configurar si tenemos conectados el segundo
                           // módulo de MRF49XA o no. OJO!! No puede estar
                           //conectado a la vez que el WIFI.


        /*********************************************************************/
        // Definition of MRF89XA enables the application to use Microchip
        // MRF89XA subGHz proprietary RF transceiver
        /*********************************************************************/
        //#define MRF89XA


    /*********************************************************************/
    // ENABLE_NETWORK_FREEZER enables the network freezer feature, which
    // stores critical network information into non-volatile memory, so
    // that the protocol stack can recover from power loss gracefully.
    // The network infor can be saved in data EPROM of MCU, external
    // EEPROM or programming space, if enhanced flash is used in MCU.
    // Network freezer feature needs definition of NVM kind to be
    // used, which is specified in HardwareProfile.h
    /*********************************************************************/
    //#define ENABLE_NETWORK_FREEZER


    /*********************************************************************/
    // MY_ADDRESS_LENGTH defines the size of wireless node permanent
    // address in byte. This definition is not valid for IEEE 802.15.4
    // compliant RF transceivers.
    /*********************************************************************/
    #define MY_ADDRESS_LENGTH       4

    /*********************************************************************/
    // EUI_x defines the xth byte of permanent address for the wireless
    // node
    /*********************************************************************/
    #define EUI_7 0x11
    #define EUI_6 0x22
    #define EUI_5 0x33
    #define EUI_4 0x44
    #define EUI_3 0x55
    #define EUI_2 0x66
    #define EUI_1 0x77

    #if defined(Nodo1)
//        #define EUI_0 0x01 //XXX-GuilJa Receptor
        #define EUI_0 0x05 //XXX-GuilJa Emisor
    #elif defined(Nodo2)
        #define EUI_0 0x05
    #endif
    /*********************************************************************/
    // TX_BUFFER_SIZE defines the maximum size of application payload
    // which is to be transmitted
    /*********************************************************************/
    #define TX_BUFFER_SIZE 79 //Por hacerlo igual que el de recepcion

    /*********************************************************************/
    // RX_BUFFER_SIZE defines the maximum size of application payload
    // which is to be received
    /*********************************************************************/
    #define RX_BUFFER_SIZE 79 //El max para el MRF24J40

    /*********************************************************************/
    // MY_PAN_ID defines the PAN identifier. Use 0xFFFF if prefer a
    // random PAN ID.
    /*********************************************************************/

    //Por defecto el programa pone 0x1234, el resto lo he añadido yo.
    //Guillermo.
    #if defined(Nodo1)  //Guillermo.
    #define MY_PAN_ID                       0x1234
    #elif defined(Nodo2)                            //Guillermo.
    #define MY_PAN_ID                       0xFFFF  //Guillermo.
    #endif                                          //Guillermo.

    /*********************************************************************/
    // ADDITIONAL_NODE_ID_SIZE defines the size of additional payload
    // will be attached to the P2P Connection Request. Additional payload
    // is the information that the devices what to share with their peers
    // on the P2P connection. The additional payload will be defined by
    // the application and defined in main.c
    /*********************************************************************/
    #define ADDITIONAL_NODE_ID_SIZE   2 /*GuilJa en el primero guardararemos las
                las peticiones aceptadas por el nodo y en el segundo las
                */

    /*********************************************************************/
    // P2P_CONNECTION_SIZE defines the maximum P2P connections that this
    // device allowes at the same time.
    /*********************************************************************/
    #define CONNECTION_SIZE             10


    /*********************************************************************/
    // TARGET_SMALL will remove the support of inter PAN communication
    // and other minor features to save programming space
    /*********************************************************************/
    //#define TARGET_SMALL

    /*********************************************************************/
    // ENABLE_PA_LNA enable the external power amplifier and low noise
    // amplifier on the RF board to achieve longer radio communication
    // range. To enable PA/LNA on RF board without power amplifier and
    // low noise amplifier may be harmful to the transceiver.
    /*********************************************************************/
    //#define ENABLE_PA_LNA


    /*********************************************************************/
    // ENABLE_HAND_SHAKE enables the protocol stack to hand-shake before
    // communicating with each other. Without a handshake process, RF
    // transceivers can only broadcast, or hardcoded the destination address
    // to perform unicast.
    /*********************************************************************/
    #define ENABLE_HAND_SHAKE


    /*********************************************************************/
    // ENABLE_SLEEP will enable the device to go to sleep and wake up
    // from the sleep
    /*********************************************************************/
    #if defined(Nodo1)
    //#define ENABLE_SLEEP
    #elif defined(Nodo2)
    #define ENABLE_SLEEP
    #endif

    /*********************************************************************/
    // ENABLE_ED_SCAN will enable the device to do an energy detection scan
    // to find out the channel with least noise and operate on that channel
    /*********************************************************************/
    #if defined(Nodo1)
    #define ENABLE_ED_SCAN
    #elif defined(Nodo2)
    //#define ENABLE_ED_SCAN
    #endif

    /*********************************************************************/
    // ENABLE_ACTIVE_SCAN will enable the device to do an active scan to
    // to detect current existing connection.
    /*********************************************************************/
    #define ENABLE_ACTIVE_SCAN


    /*********************************************************************/
    // ENABLE_SECURITY will enable the device to encrypt and decrypt
    // information transferred
    /*********************************************************************/
    #define ENABLE_SECURITY

    /*********************************************************************/
    // ENABLE_INDIRECT_MESSAGE will enable the device to store the packets
    // for the sleeping devices temporily until they wake up and ask for
    // the messages
    /*********************************************************************/
    #if defined(Nodo1)
    #define ENABLE_INDIRECT_MESSAGE
    #elif defined(Nodo2)
    //#define ENABLE_INDIRECT_MESSAGE
    #endif

    /*********************************************************************/
    // ENABLE_BROADCAST will enable the device to broadcast messages for
    // the sleeping devices until they wake up and ask for the messages
    /*********************************************************************/
    #define ENABLE_BROADCAST


    /*********************************************************************/
    // RFD_WAKEUP_INTERVAL defines the wake up interval for RFDs in second.
    // This definition is for the FFD devices to calculated various
    // timeout. RFD depends on the setting of the watchdog timer to wake
    // up, thus this definition is not used.
    /*********************************************************************/
    #define RFD_WAKEUP_INTERVAL     8


    /*********************************************************************/
    // ENABLE_FREQUENCY_AGILITY will enable the device to change operating
    // channel to bypass the sudden change of noise
    /*********************************************************************/
    #define ENABLE_FREQUENCY_AGILITY


    // Constants Validation

    #if !defined(MRF24J40) && !defined(MRF49XA) && !defined(MRF89XA) && !defined(MRF49XA_alt)
        #error "One transceiver must be defined for the wireless application"
    #endif

    #if (defined(MRF24J40) && defined(MRF49XA)) || (defined(MRF24J40) && defined(MRF89XA)) || (defined(MRF49XA) && defined(MRF89XA))
        #error "Only one transceiver can be defined for the wireless application"
    #endif

    #if !defined(PROTOCOL_P2P) && !defined(PROTOCOL_MIWI) && !defined(PROTOCOL_MIWI_PRO)
        #error "One Microchip proprietary protocol must be defined for the wireless application."
    #endif

    #if defined(ENABLE_FREQUENCY_AGILITY)
        #define ENABLE_ED_SCAN
    #endif

    #if MY_ADDRESS_LENGTH > 8
        #error "Maximum address length is 8"
    #endif

    #if MY_ADDRESS_LENGTH < 2
        #error "Minimum address length is 2"
    #endif

    #if defined(MRF24J40)
        #define IEEE_802_15_4
        #undef MY_ADDRESS_LENGTH
        #define MY_ADDRESS_LENGTH 8
    #endif

    #if defined(ENABLE_NETWORK_FREEZER)
        #define ENABLE_NVM
    #endif

    #if defined(ENABLE_ACTIVE_SCAN) && defined(TARGET_SMALL)
        #error  Target_Small and Enable_Active_Scan cannot be defined together
    #endif

    #if defined(ENABLE_INDIRECT_MESSAGE) && !defined(RFD_WAKEUP_INTERVAL)
        #error "RFD Wakeup Interval must be defined if indirect message is enabled"
    #endif

    #if (RX_BUFFER_SIZE > 127)
        #error RX BUFFER SIZE too large. Must be <= 127.
    #endif

    #if (TX_BUFFER_SIZE > 127)
        #error TX BUFFER SIZE too large. Must be <= 127.
    #endif

    #if (RX_BUFFER_SIZE < 10)
        #error RX BUFFER SIZE too small. Must be >= 10.
    #endif

    #if (TX_BUFFER_SIZE < 10)
        #error TX BUFFER SIZE too small. Must be >= 10.
    #endif

    #if (CONNECTION_SIZE > 0xFE)
        #error NETWORK TABLE SIZE too large.  Must be < 0xFF.
    #endif

/*****************************************************************************/
/************************FIN CONFIGURACION SOFTWARE***************************/
/***********************************MIWI**************************************/
/*****************************************************************************/


#endif
