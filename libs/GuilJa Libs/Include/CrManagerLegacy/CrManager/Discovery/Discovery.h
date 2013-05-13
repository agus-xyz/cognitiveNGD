/*****************************************************************************
 *
 *              Discovery.h -- Discovery v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:
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
 * El header del Discovery.c
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   5/4/2012    Initial revision
 *****************************************************************************/
#ifndef  _DISCOVERY_H_
#define  _DISCOVERY_H_

#include "GenericTypeDefs.h"
#include "WirelessProtocols/MCHP_API.h"
#include "../Include/WirelessProtocols/Console.h"
#if defined(MRF24J40Exp)
    #include "MisLibrerias/MRF24J40Experimental.h"
#endif
#if defined(MRF49XA) || defined(MRF49XA_alt)
    #include "Transceivers/MRF49XA/ConfigMRF49XA.h"
    #include "Transceivers/MRF49XA/MRF49XA.h"
#endif

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
 *     Esta es la funcion que utilizaremos para mirar el ruido que hay en uno o varios canales
 *     dependiendo del ChannelMap que pongamos.
 *
 *****************************************************************************************/
BYTE B105_NoiseDetection(INPUT DWORD ChannelMap, INPUT BYTE ScanDuration, INPUT BYTE DetectionMode, OUTPUT BYTE *RSSIValue);

#endif