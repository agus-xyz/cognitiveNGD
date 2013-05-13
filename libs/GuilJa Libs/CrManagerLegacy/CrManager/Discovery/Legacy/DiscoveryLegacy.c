/*****************************************************************************
 *
 *              Discovery.c -- Discovery Module v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        Discovery.c
 * Dependencies:    Discovery.h, ConfigDiscovery.h
 * Processor:
 * BOARD:
 * Compiler:        C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         B105 -FAILURE IS NOT AN OPTION-
 *
 *****************************************************************************
 * File Description:
 *
 * Este archivo se ocupa de las funciones propias del sensado del entorno.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   5/4/2012    Initial revision
 *****************************************************************************/
#include "CrManager/Discovery/Discovery.h"
#include "CrManager/Discovery/ConfigDiscovery.h"
#include "ConfigApp.h"
#include "GenericTypeDefs.h"
#include "WirelessProtocols/MCHP_API.h"
#include "../Include/WirelessProtocols/Console.h"
#include "Transceivers/MRF49XA/ConfigMRF49XA.h"
#include "Transceivers/MRF49XA/MRF49XA.h"
#if defined(MRF24J40)
//    #include "MisLibrerias/MRF24J40Experimental.h"
#endif

/************************ VARIABLES ********************************/

// Scan Duration formula
//  60 * (2 ^ n + 1) symbols, where one symbol equals 16us
#define B105_SCAN_DURATION_0 SYMBOLS_TO_TICKS(120)
#define B105_SCAN_DURATION_1 SYMBOLS_TO_TICKS(180)
#define B105_SCAN_DURATION_2 SYMBOLS_TO_TICKS(300)
#define B105_SCAN_DURATION_3 SYMBOLS_TO_TICKS(540)
#define B105_SCAN_DURATION_4 SYMBOLS_TO_TICKS(1020)
#define B105_SCAN_DURATION_5 SYMBOLS_TO_TICKS(1980)
#define B105_SCAN_DURATION_6 SYMBOLS_TO_TICKS(3900)
#define B105_SCAN_DURATION_7 SYMBOLS_TO_TICKS(7740)
#define B105_SCAN_DURATION_8 SYMBOLS_TO_TICKS(15420)
#define B105_SCAN_DURATION_9 SYMBOLS_TO_TICKS(30780)
#define B105_SCAN_DURATION_10 SYMBOLS_TO_TICKS(61500)
#define B105_SCAN_DURATION_11 SYMBOLS_TO_TICKS(122940)
#define B105_SCAN_DURATION_12 SYMBOLS_TO_TICKS(245820)
#define B105_SCAN_DURATION_13 SYMBOLS_TO_TICKS(491580)
#define B105_SCAN_DURATION_14 SYMBOLS_TO_TICKS(983100)

const ROM DWORD B105_ScanTime[15] = {B105_SCAN_DURATION_0,B105_SCAN_DURATION_1,B105_SCAN_DURATION_2,B105_SCAN_DURATION_3,
    B105_SCAN_DURATION_4,B105_SCAN_DURATION_5,B105_SCAN_DURATION_6,B105_SCAN_DURATION_7,B105_SCAN_DURATION_8,B105_SCAN_DURATION_9,
    B105_SCAN_DURATION_10, B105_SCAN_DURATION_11, B105_SCAN_DURATION_12, B105_SCAN_DURATION_13,B105_SCAN_DURATION_14
};


//Los siguientes arrays estan declarados en el experimental MRF24J40Experimental.c y son
//para hacer Dumps de los registros de dicho transceptor.
//extern BYTE RegistrosCortos[64];
//extern BYTE RegistrosLargos[77];
//extern BYTE RegistrosCortosTemp[64];
//extern BYTE RegistrosLargosTemp[77];
//
//extern BYTE RegistrosCortos2[64];
//extern BYTE RegistrosLargos2[77];
//extern BYTE RegistrosCortosTemp2[64];
//extern BYTE RegistrosLargosTemp2[77];

//Una variable de este módulo para guardar el canal temporal, digamos que lo que no queremos es
//que al realizar el sensado cambiemos la variable de pila que tiene la informacion del canal
//en el que se esta trabajando porque no queremos que el sensado cambie el canal en el que
//trabaja el nodo permanentemente.
BYTE B105_currentChannel=0;

//PARA PROBAR CARRIER SENSE. Un array de 32 BYTES en el que guardaremos el TESTRSSI de cada uno
// de los canales cuando realizamos escaneo de portadoras.
BYTE CHANNTESTCS[32];


/************************************************************************************
 * Function:
 *      void B105_CCASetMode(INPUT BYTE AssessmentMode)
 *
 * Summary:
 *      This function set the operating mode for the CCA
 *
 * Description:
 *      Con esta función preparamos el módulo RF para realizar el CCA en modo de
 *      detección de energía o en modo de detección de portadora.
 *
 * IMPORTANTE:
 *      Ésta función modifica registros del transceptor. En la pila de MiWi no se utilizan
 *      estos registros aparentemente. Aún asi los backapeamos-restauramos desde donde hacemos
 *      el sensado que es en primera instancia B105_NoiseDetection.
 *
 *
 *
 * PreCondition:
 *      Hardware initialization on MCU has been done.
 *
 * Parameters:
 *      BYTE AssessmentMode -   The mode to perform noise assessment. The possible
 *                              assessment modes are
 *                              * CHANNEL_ASSESSMENT_CARRIER_SENSE Carrier sense detection mode
 *                              * CHANNEL_ASSESSMENT_ENERGY_DETECT Energy detection mode
 *
 *
 * Returns:
 *      Nada.
 *
 * Example:
 *      <code>
 *      // Ponerlo en modo detección de portadora.
 *     B105_CCASetMode(CHANNEL_ASSESSMENT_CARRIER_SENSE);
 *      </code>
 *
 * Remarks:
 *      None
 *
 *****************************************************************************************/

void B105_CCASetMode(INPUT BYTE AssessmentMode)
{

    switch(AssessmentMode)
    {
        case CHANNEL_ASSESSMENT_ENERGY_DETECT:
            #if defined(MRF24J40)
                /* Program CCA mode using RSSI */
                PHYSetShortRAMAddr(WRITE_BBREG2,0x80);
                /* Program CCA, RSSI threshold values */
                PHYSetShortRAMAddr(WRITE_RSSITHCCA,0x60);
                break;
            #endif
        case CHANNEL_ASSESSMENT_CARRIER_SENSE:
            #if defined(MRF24J40)
                //XXX-Willy. Set the CCA Mode to Carrier Sense ya configure its
                //threshold as recommended in the datasheet.
                PHYSetShortRAMAddr(WRITE_BBREG2, 0x78);
            #endif
            break;
    }



}



#if defined(ENABLE_ED_SCAN)

/************************************************************************************
 * Function:
 *      BYTE B105_ChannelAssesment(BYTE AssessmentMode)
 *
 * Summary:
 *      This function perform the noise detection on current operating channel
 *
 * Description:
 *      This is the primary MiMAC interface for the protocol layer to
 *      perform the noise detection scan. Not all assessment modes are supported
 *      for all RF transceivers.
 *
 * PreCondition:
 *      MiMAC initialization has been done.
 *
 * Parameters:
 *      BYTE AssessmentMode -   The mode to perform noise assessment. The possible
 *                              assessment modes are
 *                              * CHANNEL_ASSESSMENT_CARRIER_SENSE Carrier sense detection mode
 *                              * CHANNEL_ASSESSMENT_ENERGY_DETECT Energy detection mode
 *                              Por el momento no hay ninguna diferencia en como actua el
 *                              método en función del modo seleccionado pero lo dejamos así
 *                              para que se sencillo añadirla en caso de que se quiera hacer en
 *                              un futuro. La deteccion del nivel de energía no funciona en el
 *                              modo de deteccion de portadoras.
 *
 *
 *
 * Returns:
 *      A byte to indicate the noise level at current channel.
 *
 * Example:
 *      <code>
 *      NoiseLevel = MiMAC_ChannelAssessment(CHANNEL_ASSESSMENT_CARRIER_SENSE);
 *      </code>
 *
 * Remarks:
 *      None
 *
 *****************************************************************************************/
BYTE B105_ChannelAssessment(INPUT BYTE AssessmentMode)
{
    BYTE RSSIcheck;
    BYTE TESTCS;

    switch (AssessmentMode)
    {
        case CHANNEL_ASSESSMENT_ENERGY_DETECT:
            #if defined(ENABLE_PA_LNA)
                PHYSetLongRAMAddr(TESTMODE, 0x08); // Disable automatic switch on PA/LNA
                #if defined(MRF24J40MC)
                    PHYSetShortRAMAddr(WRITE_GPIODIR, 0x0F); // Set GPIO direction
                    PHYSetShortRAMAddr(WRITE_GPIO, 0x0C); // Enable LNA
                #else
                    PHYSetShortRAMAddr(WRITE_GPIODIR, 0x0F); // Set GPIO direction
                    PHYSetShortRAMAddr(WRITE_GPIO, 0x04); // Enable LNA
                #endif
            #endif
            //B105_CCASetMode(CHANNEL_ASSESSMENT_ENERGY_DETECT); //XXX-Willy. Lo hemos cambiado de sitio
            //porque no interesaba tenerlo aqui debido
            //a que se repetía muchas veces al sensar.
            #if defined(MRF24J40)
                // calculate RSSI for firmware request
                PHYSetShortRAMAddr(WRITE_BBREG6, 0x80);

                // Firmware Request the RSSI
                RSSIcheck = PHYGetShortRAMAddr(READ_BBREG6);
                while ((RSSIcheck & 0x01) != 0x01)
                {
                    RSSIcheck = PHYGetShortRAMAddr(READ_BBREG6);
                }

                // read the RSSI
                RSSIcheck = PHYGetLongRAMAddr(0x210);

                // enable RSSI attached to received packet again after
                // the energy scan is finished
                PHYSetShortRAMAddr(WRITE_BBREG6, 0x40);
            #endif
            #if defined(ENABLE_PA_LNA)
                #if defined(MRF24J40MC)
                    PHYSetShortRAMAddr(WRITE_GPIO, 0x08);
                    PHYSetShortRAMAddr(WRITE_GPIODIR, 0x08);
                #else
                    PHYSetShortRAMAddr(WRITE_GPIO, 0);
                    PHYSetShortRAMAddr(WRITE_GPIODIR, 0x00);
                #endif
                PHYSetLongRAMAddr(TESTMODE, 0x0F);
              #endif

            return RSSIcheck;
            break;

        case CHANNEL_ASSESSMENT_CARRIER_SENSE:
            //B105_CCASetMode(CHANNEL_ASSESSMENT_CARRIER_SENSE);//XXX-Willy. Lo hemos cambiado de sitio
            //porque no interesaba tenerlo aqui debido
            //a que se repetía muchas veces al sensar.
            //                    //XXX-Willy. Experimental. Guardará también el RSSI
            //                    // calculate RSSI for firmware request
            //                    PHYSetShortRAMAddr(WRITE_BBREG6, 0x80);
            //
            //                    // Firmware Request the RSSI
            //                    RSSIcheck = PHYGetShortRAMAddr (READ_BBREG6);
            //                    while ((RSSIcheck & 0x01) != 0x01)
            //                    {
            //                        RSSIcheck = PHYGetShortRAMAddr (READ_BBREG6);
            //                    }
            //
            //                    // read the RSSI
            //                    RSSIcheck = PHYGetLongRAMAddr(0x210);
            //
            //                    // enable RSSI attached to received packet again after
            //                    // the energy scan is finished
            //                    PHYSetShortRAMAddr(WRITE_BBREG6, 0x40);
            //                    return RSSIcheck;
            #if defined(MRF24J40)
                TESTCS = PHYGetLongRAMAddr(TESTRSSI);
                //                    Printf("\r\nNo hace nada porque no funciona la deteccion de portadoras\r\n");
            #endif
            return TESTCS;
            break;
    }
}
#endif

/************************************************************************************
 * Function:
 *      BOOL B105_SetChannel(BYTE channel, BYTE offsetFreq)
 *
 * Summary:
 *      This function set the operating channel for the RF transceiver
 *
 * Description:
 *      This is the primary MiMAC interface for the protocol layer to
 *      set the operating frequency of the RF transceiver. Valid channel
 *      number are from 0 to 31. For different frequency band, data rate
 *      and other RF settings, some channels from 0 to 31 might be
 *      unavailable. Paramater offsetFreq is used to fine tune the center
 *      frequency across the frequency band. For transceivers that follow
 *      strict definition of channels, this parameter may be discarded.
 *      The center frequency is calculated as
 *      (LowestFrequency + Channel * ChannelGap + offsetFreq)
 *
 * PreCondition:
 *      Hardware initialization on MCU has been done.
 *
 * Parameters:
 *      BYTE channel -  Channel number. Range from 0 to 31. Not all channels
 *                      are available under all conditions.
 *      BYTE offsetFreq -   Offset frequency used to fine tune the center
 *                          frequency. May not apply to all RF transceivers
 *
 * Returns:
 *      A boolean to indicates if channel setting is successful.
 *
 * Example:
 *      <code>
 *      // Set center frequency to be exactly channel 12
 *      MiMAC_SetChannel(12, 0);
 *      </code>
 *
 * Remarks:
 *      None
 *
 * NOTA:
 *     El offset frequency estaría por descubrir en que registro se toca, quizas el RFOPT
 *     pero habría que comprobarlo con el analizador de espectros.
 *****************************************************************************************/
BOOL B105_SetChannel(INPUT BYTE channel, INPUT BYTE offsetFreq)
{
    if (channel < 11 || channel > 26)
    {
        return FALSE;
    }

    #if defined(ENABLE_PA_LNA) && (defined(MRF24J40MB) || defined(MRF24J40MC))
        if (channel == 26)
        {
            return FALSE;
        }
    #endif

    //        MACCurrentChannel = channel; //XXX-Willy. Esta variable solo está en el MRF24J40.
    //Para cambio temporales en los que no vamos a hacer
    //podemos obviarla, pero si vamos a recibir mensajes
    //tendremos que cambiarla allí.
    #if defined(MRF24J40)
        PHYSetLongRAMAddr(RFCTRL0, ((channel - 11) << 4) | 0x03);
        PHYSetShortRAMAddr(WRITE_RFCTL, 0x04);
        PHYSetShortRAMAddr(WRITE_RFCTL, 0x00);
    #endif
    return TRUE;
}



/************************************************************************************
 * Function:
 *      BOOL B105_MiApp_SetChannel(BYTE channel)
 *
 * Summary:
 *      This function set the operating channel for the RF transceiver
 *
 * Description:
 *      La función para cambiar de canal de MiApp, que llama a la de MiMAC.
 *
 * PreCondition:
 *      Hardware initialization on MCU has been done.
 *
 * Parameters:
 *      BYTE channel -  Channel number. Range from 0 to 31. Not all channels
 *                      are available under all conditions.
 *
 * Returns:
 *      A boolean to indicates if channel setting is successful.
 *
 * Example:
 *      <code>
 *      // Set center frequency to be exactly channel 12
 *      B105_MiApp_SetChannel(12);
 *      </code>
 *
 * Remarks:
 *      None
 *
 *****************************************************************************************/
BOOL B105_MiApp_SetChannel(BYTE channel)
{
    if( B105_SetChannel(channel, 0) )
    {
        B105_currentChannel = channel; //XXX-Willy. No queremos modificar parametros por eso
                                       //nos creamos nuestra propia variable en vez de utilizar
                                       //currentChannel.
        #if defined(ENABLE_NETWORK_FREEZER)
            nvmPutCurrentChannel(&currentChannel);
        #endif
        return TRUE;
    }
    return FALSE;
}

/************************************************************************************
 * Function:
 *      BYTE B105_NoiseDetection(  DWORD ChannelMap, BYTE ScanDuration,
 *                                  BYTE DetectionMode, BYTE *NoiseLevel)
 *
 * Summary:
 *      This function perform a noise scan and returns the channel with least noise
 *
 * Description:
 *      This is the primary user interface functions for the application layer to
 *      perform noise detection on multiple channels.
 *
 * PreCondition:
 *      Protocol initialization has been done. ESTOS ES IMPORTANTE.
 *
 * Parameters:
 *      DWORD ChannelMap -  The bit map of channels to perform noise scan. The 32-bit
 *                          double word parameter use one bit to represent corresponding
 *                          channels from 0 to 31. For instance, 0x00000003 represent to
 *                          scan channel 0 and channel 1.
 *      BYTE ScanDuration - The maximum time to perform scan on single channel. The
 *                          value is from 5 to 14. The real time to perform scan can
 *                          be calculated in following formula from IEEE 802.15.4
 *                          specification
 *                              960 * (2^ScanDuration + 1) * 10^(-6) second
 *      BYTE DetectionMode -    The noise detection mode to perform the scan. The two possible
 *                              scan modes are
 *                              * NOISE_DETECT_ENERGY   Energy detection scan mode
 *                              * NOISE_DETECT_CS       Carrier sense detection scan mode
 *      BYTE *NoiseLevel -  The noise level at the channel with least noise level
 *
 * Returns:
 *      The channel that has the lowest noise level
 *
 * Example:
 *      <code>
 *      BYTE NoiseLevel;
 *      OptimalChannel = MiApp_NoiseDetection(0xFFFFFFFF, 10, NOISE_DETECT_ENERGY, &NoiseLevel);
 *      </code>
 *
 * Remarks:
 *      None
 *
 * NOTA:
 *     Hay muchas lineas que están comentadas. Esto es porque se utilizó esta función para hacer
 *     pruebas e intentar descubrir la localización del bit que indica si el canal está libre u
 *     ocupado ya que esto no está en la documentación. He dejado las lineas porque aunque el
 *     resultado no fuera todo lo satisfactorio que se hubiera deseado si que mostró algunas
 *     cosillas interestantes.
 *
 *****************************************************************************************/
BYTE B105_NoiseDetection(INPUT DWORD ChannelMap, INPUT BYTE ScanDuration, INPUT BYTE DetectionMode, OUTPUT BYTE *RSSIValue) {
    BYTE i;
    BYTE OptimalChannel;
    BYTE minRSSI = 0xFF;
    DWORD channelMask = 0x00000001;
    MIWI_TICK t1, t2;

    #if defined(MRF24J40)
        //Hacemos un backup de los registros que modificamos durante el proceso de sensado, que son
        //el del modo de CCA y el de RSSI threshold.
        BYTE BackupBBREG2 = PHYGetShortRAMAddr(READ_BBREG2);
        BYTE BackupRSSITHCCA = PHYGetShortRAMAddr(READ_RSSITHCCA);
    #endif
    switch (DetectionMode) {
        case NOISE_DETECT_ENERGY:
            //                    if( DetectionMode != NOISE_DETECT_ENERGY )
            //                    {
            //                        return 0xFF;
            //                    }

            ConsolePutROMString((ROM char*) "\r\nEnergy Scan Results:");

//            BOOL InitRegistro = TRUE; //XXX-Willy. Para que se guarden inicialmente un dump de los
//            BOOL InitRegistro2 = TRUE; //XXX-Willy. registros

            B105_CCASetMode(CHANNEL_ASSESSMENT_ENERGY_DETECT);
            i = 0;
            while (i < 32) {
                if (ChannelMap & FULL_CHANNEL_MAP & (channelMask << i)) {
                    BYTE RSSIcheck;
                    BYTE maxRSSI = 0;
                    BYTE j, k;

                    /* choose appropriate channel */
                    B105_MiApp_SetChannel(i);

                    t2 = MiWi_TickGet();

                    while (1) {
                        RSSIcheck = B105_ChannelAssessment(CHANNEL_ASSESSMENT_ENERGY_DETECT);

                        if (RSSIcheck > maxRSSI) {
                            maxRSSI = RSSIcheck;
                            //                                    if( maxRSSI> PHYGetShortRAMAddr(READ_RSSITHCCA))
                            //                                    {
                            //                                        if(InitRegistro2)
                            //                                        {
                            //                                            DelayMsTimer(1);
                            //                                            CopiarRegistros(LongAddressReg, RegistrosLargos2);
                            //                                            CopiarRegistros(ShortAddressReg, RegistrosCortos2);
                            //                                            InitRegistro2=FALSE;
                            //                                        }
                            //                                        DelayMsTimer(1);
                            //                                        CopiarRegistros(ShortAddressReg, RegistrosCortosTemp2);
                            //                                        CopiarRegistros(LongAddressReg, RegistrosLargosTemp2);
                            //                                        OperarRegistros(LongAddressReg, OpAND, RegistrosLargos2, RegistrosLargosTemp2);
                            //                                        OperarRegistros(ShortAddressReg, OpAND, RegistrosCortos2, RegistrosCortosTemp2);

                            //                                        PrintDec((0x20&PHYGetShortRAMAddr(READ_TXSR)));
                            //                                        DelayMsTimer(1);
                            //                                        PrintDec((0x20&PHYGetShortRAMAddr(READ_TXSR)));
                            //                                    }
                            //                                    else
                            //                                    {
                            //                                        if(InitRegistro)
                            //                                        {
                            ////                                            DelayMsTimer(1);
                            //                                            CopiarRegistros(LongAddressReg, RegistrosLargos);
                            ////                                            CopiarRegistros(ShortAddressReg, RegistrosCortos);
                            //                                            InitRegistro=FALSE;
                            //                                        }
                            ////                                        DelayMsTimer(1);
                            //                                        CopiarRegistros(LongAddressReg, RegistrosLargosTemp);
                            ////                                        CopiarRegistros(ShortAddressReg, RegistrosCortosTemp);
                            ////                                        OperarRegistros(ShortAddressReg, OpAND, RegistrosCortos, RegistrosCortosTemp);
                            //                                        OperarRegistros(LongAddressReg, OpAND, RegistrosLargos, RegistrosLargosTemp);
                            //                                    }
                        }
                        //                                else
                        //                                {
                        //                                    if(InitRegistro)
                        //                                    {
                        ////                                        DelayMsTimer(1);
                        //                                        CopiarRegistros(LongAddressReg, RegistrosLargos);
                        ////                                        CopiarRegistros(ShortAddressReg, RegistrosCortos);
                        //                                        InitRegistro=FALSE;
                        //                                    }
                        ////                                    DelayMsTimer(1);
                        //                                    CopiarRegistros(LongAddressReg, RegistrosLargosTemp);
                        ////                                    CopiarRegistros(ShortAddressReg, RegistrosCortosTemp);
                        //                                    OperarRegistros(LongAddressReg, OpAND, RegistrosLargos, RegistrosLargosTemp);
                        ////                                    OperarRegistros(ShortAddressReg, OpAND, RegistrosCortos, RegistrosCortosTemp);
                        //                                }


                        t1 = MiWi_TickGet();
                        if (MiWi_TickGetDiff(t1, t2) > ((DWORD) (B105_ScanTime[ScanDuration]))) {
                            // if scan time exceed scan duration, prepare to scan the next channel
                            break;
                        }
                    }

                    Printf("\r\nChannel ");
                    PrintDec(i);
                    Printf(": ");
                    j = maxRSSI / 5;
                    for (k = 0; k < j; k++) {
                        ConsolePut('-');
                    }
                    Printf(" ");
                    PrintChar(maxRSSI);

                    //XXX-Willy. En caso de que el RSSI medido esté por encima del de Threshold
                    //leemos el CCFAIL habiendo retrasado 1msg para asegurarnos que le damos tiempo
                    //a actualizarse.
                    //Aparentemente esta prueba fue un fracaso puesto que no cambiaba de ninguna
                    //manera ese bit.
                    //                            if( maxRSSI> PHYGetShortRAMAddr(READ_RSSITHCCA))
                    //                            {
                    //                                PrintDec((0x20&PHYGetShortRAMAddr(READ_TXSR)));
                    //                                DelayMsTimer(1);
                    //                                PrintDec((0x20&PHYGetShortRAMAddr(READ_TXSR)));
                    //                            }

                    if (maxRSSI < minRSSI) {
                        minRSSI = maxRSSI;
                        OptimalChannel = i;
                        if (RSSIValue) {
                            *RSSIValue = minRSSI;
                        }
                    }
                }
                i++;
            }

            //                    OperarRegistros(LongAddressReg, OpXOR, RegistrosLargos, RegistrosLargos2);
            ////                    OperarRegistros(ShortAddressReg, OpXOR, RegistrosCortos, RegistrosCortos2);
            //                    ImprimirRegistro(LongAddressReg,RegistrosLargos);
            ////                    ImprimirRegistro(ShortAddressReg,RegistrosCortos);

            //                    return OptimalChannel;
            break;

        case NOISE_DETECT_CS:
            ConsolePutROMString((ROM char*) "\r\nCarrier Sense Results:");
            B105_CCASetMode(CHANNEL_ASSESSMENT_CARRIER_SENSE);
            i = 0;
            while (i < 32) {
                if (ChannelMap & FULL_CHANNEL_MAP & (channelMask << i)) {
                    BYTE TESTCS;
                    BYTE maxRSSI = 0;
                    BYTE j, k;

                    /* choose appropriate channel */
                    B105_MiApp_SetChannel(i);

                    //XXX-Willy. Aquí es donde vamos a poner la inicialización de los parametros
                    //para que se haga el carrier sense.

                    t2 = MiWi_TickGet();

                    while (1) {
                        TESTCS = B105_ChannelAssessment(CHANNEL_ASSESSMENT_CARRIER_SENSE);

                        CHANNTESTCS[i] = TESTCS;

                        t1 = MiWi_TickGet();
                        if (MiWi_TickGetDiff(t1, t2) > ((DWORD) (B105_ScanTime[ScanDuration]))) {
                            // if scan time exceed scan duration, prepare to scan the next channel
                            break;
                        }
                    }

                    Printf("\r\nChannel ");
                    PrintDec(i);
                    Printf(": ");
                    //                            j = maxRSSI/5;
                    //                            for(k = 0; k < j; k++)
                    //                            {
                    //                                ConsolePut('-');
                    //                            }
                    Printf(" TESTRSSI= ");
                    PrintDec(TESTCS);

                    //                            if( maxRSSI < minRSSI )
                    //                            {
                    //                                minRSSI = maxRSSI;
                    //                                OptimalChannel = i;
                    //                                if( RSSIValue )
                    //                                {
                    //                                    *RSSIValue = minRSSI;
                    //                                }
                    //                            }
                }
                i++;
            }

            //                    return OptimalChannel;
            break;

    }

    #if defined(MRF24J40)
        //XXX-Willy.Restauramos los registros y el canal.
        PHYSetShortRAMAddr(WRITE_BBREG2, BackupBBREG2);
        PHYSetShortRAMAddr(WRITE_RSSITHCCA, BackupRSSITHCCA);
    #endif

    MiApp_SetChannel(currentChannel); //XXX-Willy. Como hemos tocado el canal para hacer
    //escaneo pero no hemos tocado currentChannel pues
    //restauramos el canal con currentChannel.
    return OptimalChannel;
}
