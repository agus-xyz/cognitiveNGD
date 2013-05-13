/* 
 * File:   SystemProfile.h
 * Author: guilja
 *
 * Created on 23 de julio de 2012, 12:24
 */

#ifndef SYSTEMPROFILE_H
#define	SYSTEMPROFILE_H

/************************ HEADERS **********************************/
#include "ConfigApp.h"
#include <peripheral/spi.h>

    //Guillermo. Configuración del SPI

    #define putcSPI1(data)         SpiChnPutC(SPI_CHANNEL4, data) //Redefino la función con SPI_CHANNEL4 que es donde
    #define getcSPI1()             SpiChnGetC(SPI_CHANNEL4);      //está puesto el módulo MiWi.



#if defined(PROTOCOL_P2P)
    #include "WirelessProtocols/P2P/P2P.h"
#elif defined(PROTOCOL_MIWI)
    #include "WirelessProtocols/MiWi/MiWi.h"
#elif defined(PROTOCOL_MIWI_PRO)
    #include "WirelessProtocols/MiWiPRO/MiWiPRO.h"
#endif

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEMPROFILE_H */

