/*****************************************************************************
 *
 *              Repository.c -- Repository Module v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        Repository.c
 * Dependencies:    Repository.h
 * Processor:
 * BOARD:
 * Compiler:        C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         B105 -FAILURE IS NOT AN OPTION-
 *
 *****************************************************************************
 * File Description:
 *
 * Este archivo desarrolla las funciones y datos del modulo repository
 * (CR Module).
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   5/4/2012    Initial revision
 *****************************************************************************/

/************************ HEADERS **********************************/

#include "plib.h"
#include "CrManager/Repository/Repository.h"
#include "CrManager/Repository/ConfigRepository.h"
#include "ConfigApp.h"
#include "WirelessProtocols/MiWi/MiWi.h"
#include "WirelessProtocols/MCHP_API.h"


/************************ VARIABLES **********************************/

BYTE INDICE_TABLA_INFO_NODO = 0;

/*********************************************************************

 * Type: Structure
 *
 * Info: Una estructura con los datos de información que queremos guardar
 *       de los nodos. Basicamente se trata de la información que guarda
 *       el ActiveScan.
 ********************************************************************/
typedef struct
{
    BYTE	Channel;                        // Operating Channel of the PAN
    BYTE        Address[MY_ADDRESS_LENGTH];     // Responding device address
    WORD_VAL    PANID;                          // PAN Identifier
    BYTE        RSSIValue;                      // RSSI value for the response
    BYTE        LQIValue;                       // LQI value for the response
    rtccTime    Hora;                         // La hora.XXX-Willy.
    rtccDate    Fecha;                          // La fecha.XXX-Willy.

    union
    {
        BYTE        Val;
        struct
        {
            BYTE    Role:	2;              // Role of the responding device in the PAN
            BYTE    Sleep:	1;              // Whether the responding device goes to sleep when idle
            BYTE    SecurityEn:	1;              // Whether the responding device is capable of securing the data
            BYTE    RepeatEn:	1;              // Whether the responding device allow repeat
            BYTE    AllowJoin:	1;              // Whether the responding device allows other device to join
            BYTE    Direct:	1;              // Whether the responding device in radio range or through a repeater
            BYTE    altSrcAddr: 1;              // Whether the Address is alternative network address or permanent address
        } bits;
    } Capability;
    #if ADDITIONAL_NODE_ID_SIZE > 0
        BYTE        PeerInfo[ADDITIONAL_NODE_ID_SIZE];  // Additional Node ID information, if defined in application layer
    #endif
} NODE_INFO;

NODE_INFO ARRAY_INFO_NODOS[TAMANO_ARRAY_INFO_NODOS];

/************************ FUNCTIONS **********************************/

    /*********************************************************************
     * Function:        BOOL ComprobarNodo(void)
     *
     * PreCondition:
     *
     *
     * Input:           None
     *
     * Output:          None
     *
     * Side Effects:
     *
     *
     * Overview: Comprueba si alguno de los Nodos almacenados en la tabla de
     *           Active_Scan_Result actual tiene la misma dirección de alguno
     *           de los Nodos que ya están almacenados en nuestra tabla
     *           ARRAY_INFO_NODOS.
     *
     ********************************************************************/
    BYTE ComprobarNodo(BYTE IndiceDeEscaneoActivo)
    {
        BYTE i,j;
        for(i=0; i < INDICE_TABLA_INFO_NODO; i++)
        {
            for(j=0; j < IndiceDeEscaneoActivo; j++)
            {
                if((ActiveScanResults[j].Address[0]=ARRAY_INFO_NODOS[i].Address[0]) && (ActiveScanResults[j].Address[1]=ARRAY_INFO_NODOS[i].Address[1]))
                    return i;
            }
        }
        return INDICE_TABLA_INFO_NODO;
    }
    /*********************************************************************
     * Function:        void GuardarInfo(void)
     *
     * PreCondition:
     *
     *
     * Input:           None
     *
     * Output:          None
     *
     * Side Effects:
     *
     *
     * Overview:
     *
     *
     *
     ********************************************************************/
    BOOL GuardarInfo(TIPOINFO tipoinfo, BYTE IndiceDeEscaneoActivo)
    {
        BYTE i,j,tempIndice;

        switch (tipoinfo)
        {
            case ESCANEO:
                for(j=0; j < IndiceDeEscaneoActivo; j++)
                {
                    tempIndice=ComprobarNodo(IndiceDeEscaneoActivo);
                    if(tempIndice!=INDICE_TABLA_INFO_NODO)
                    {
                        if(INDICE_TABLA_INFO_NODO < TAMANO_ARRAY_INFO_NODOS)
                        {
                            RtccGetTimeDate(&ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].Hora, &ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].Fecha);

                            ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].Channel=ActiveScanResults[j].Channel;
                            ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].Address[0]=ActiveScanResults[j].Address[0];
                            ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].Address[1]=ActiveScanResults[j].Address[1];
                            ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].PANID=ActiveScanResults[j].PANID;
                            ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].RSSIValue=ActiveScanResults[j].RSSIValue;
                            ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].LQIValue=ActiveScanResults[j].LQIValue;
                            ARRAY_INFO_NODOS[INDICE_TABLA_INFO_NODO].Capability.Val=ActiveScanResults[j].Capability.Val ;

                            INDICE_TABLA_INFO_NODO++;
                        }
                        else
                        {
                            //La table está completa.
                        }
                    }
                    else
                    {
                        //Si ya existe el nodo actualizamos su información. Solo debería cambiar la fecha y la hora,
                        //el RSSI y el LQIValue.
                        RtccGetTimeDate(&ARRAY_INFO_NODOS[tempIndice].Hora, &ARRAY_INFO_NODOS[tempIndice].Fecha);

                        ARRAY_INFO_NODOS[tempIndice].Channel=ActiveScanResults[j].Channel;
                        ARRAY_INFO_NODOS[tempIndice].Address[0]=ActiveScanResults[j].Address[0];
                        ARRAY_INFO_NODOS[tempIndice].Address[1]=ActiveScanResults[j].Address[1];
                        ARRAY_INFO_NODOS[tempIndice].PANID=ActiveScanResults[j].PANID;
                        ARRAY_INFO_NODOS[tempIndice].RSSIValue=ActiveScanResults[j].RSSIValue;
                        ARRAY_INFO_NODOS[tempIndice].LQIValue=ActiveScanResults[j].LQIValue;
                        ARRAY_INFO_NODOS[tempIndice].Capability.Val=ActiveScanResults[j].Capability.Val ;
                    }
                }
                break;
            case MENSAJERX:
                break;
            case SALTOCANAL:
                break;
            default:
                break;
        }
    }
