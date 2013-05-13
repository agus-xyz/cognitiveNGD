/*****************************************************************************
 *
 *              GuilJa_MiWi_Utils.c -- GuilJa MiWi Utilities v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        GuilJa_MiWi_Utils.c
 * Dependencies:    GuilJa_MiWi_Utils.h
 * Processor:
 * BOARD:
 * Compiler:        C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         B105 -FAILURE IS NOT AN OPTION-
 *
 *****************************************************************************
 * File Description:
 *
 * Este archivo proporciona funciones de envío y recepción por MiWi muy
 * simplificadas para  no tener que realizar todas las llamadas y olvidarse un
 * poco del protocolo.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   8/02/2012    Initial revision
 *****************************************************************************/

#include "GuilJa Utils Heads/MiWi Utils/GuilJa_MiWi_Utils.h"

/*GuilJaALERT Originalmente estos dos buffers estaban como externs y se definian
 en el Buffer.c, así que para alguno de los ejemplos antiguos este codigo habrá
 que cambiarlo para utilizarlo.*/
//extern BYTE BufferTx[TX_BUFFER_SIZE];
//extern BYTE BufferRx[RX_BUFFER_SIZE];

/*****************************************************************************/
/*********************************FUNCIONES***********************************/
/*****************************************************************************/


/******************************************************************************/
/*
 * Nombre: BOOL GuilJa_MiWi_Send_Buffer(BYTE *Buffer, BYTE *Address)
 * Función: Enviar los datos contenido en el buffer que se pasa como parametro.
 * Devuelve: TRUE o FALSE
 * Parametros: El puntero al buffer y la direccion.
 *
 * NOTA:
 * 
 */
BOOL GuilJa_MiWi_Send_Buffer(BYTE *Buffer, BYTE *Address)
{
    BYTE i;

    MiApp_FlushTx(); //"Nos situamos en el inicio del buffer de tx de la pila.

    for(i = 0; i < TX_BUFFER_SIZE; i++)
    {
        MiApp_WriteData(Buffer[i]); /*Volcamos nuestro buffer al de tx de la
                                     pila.*/
    }

    /*TODO HECHO cambiar el Connection por address y coger el parametro.*/
//    if (MiApp_UnicastConnection(0, TRUE) == FALSE) /*Realizamos el envío del
//                                                    buffer de tx de la pila.*/
    if (MiApp_UnicastAddress(Address, TRUE, TRUE) == FALSE)
    {
        #if defined(DEBUG_VISUAL)
            Printf("\r\nUnicast Failed\r\n");
        #endif
        return FALSE;
    }
    else
    {
        Printf("\r\nUnicast GUILJAAAAAAAAAAAAAa\r\n");
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/*
 * Nombre: BOOL GuilJa_MiWi_Recvd_Buffer(BYTE *Buffer)
 * Función: Llenar el buffer que le pasamos como parámetro con los elementos que
 * llegan.
 * Devuelve: TRUE o FALSE segun se haya recibido correctamente.
 * Parametros: El buffer en el que queremos escribir y su tamaño.
 *
 * NOTA:
 * 
 */
BOOL GuilJa_MiWi_Rcvd_Buffer(RECEIVED_MESSAGE *Buffer, BYTE *SizeOfBuffer)
{
        BYTE tamBufferRx = *(SizeOfBuffer);
        BYTE i,j;
        BYTE indiceBuffer = 0;
        

        /*******************************************************************/
        // Function MiApp_MessageAvailable will return a boolean to indicate
        // if a message for application layer has been received by the
        // transceiver. If a message has been received, all information will
        // be stored in the rxMessage, structure of RECEIVED_MESSAGE.
        /*******************************************************************/
        if( MiApp_MessageAvailable())
        {
            //XXX-Willy. Limpiamos el buffer de recepción para escrituras sucesivas.
//            for (i = 0; i < tamBufferRx; i++)
//            {
//                Buffer->Payload[i] = 0;
//            }

            /*******************************************************************/
            // If a packet has been received, following code prints out some of
            // the information available in rxFrame.
            /*******************************************************************/
            //            #ifdef ENABLE_SECURITY
            //                if( rxMessage.flags.bits.secEn )
            //                {
            //                    ConsolePutROMString((ROM char *)"Secured ");
            //                }
            //            #endif
//                        #ifndef TARGET_SMALL
//                            if( rxMessage.flags.bits.broadcast )
//                            {
//                                ConsolePutROMString((ROM char *)"Broadcast Packet with RSSI ");
                                //TODO Guardar en el buffer el bit de broadcast.
//                            }
//                            else
//                            {
            //                    ConsolePutROMString((ROM char *)"Unicast Packet with RSSI ");
                                //TODO guardar en el buffer el bit de broadcast.
//                            }
                                //TODO guardar el RSSI.
            //                PrintChar(rxMessage.PacketRSSI);
//                            if( rxMessage.flags.bits.srcPrsnt )
//                            {
            //                    ConsolePutROMString((ROM char *)" from ");
            //                    if( rxMessage.flags.bits.altSrcAddr )
            //                    {
            //                        PrintChar(rxMessage.SourceAddress[1]);
            //                        PrintChar(rxMessage.SourceAddress[0]);
            //                    }
            //                    else
            //                    {
//                                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
//                                    {
                                        //TODO guardar en el buffer la direccion
                                        //del origen.

            //                            PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH-1-i]);
//                                    }
            //                    }
//                            }
//                        #endif
            //            ConsolePutROMString((ROM char *)": ");

            //XXX-Willy. Lo comento para hacer mis pruebas.
            //            for(i = 0; i < rxMessage.PayloadSize; i++)
            //            {
            //                ConsolePut(rxMessage.Payload[i]);
            //            }

            #ifndef TARGET_SMALL /*XXX-GuilJa.Este ifndef lo dejamos porque lo
            traia el ejemplo de MiWi para mirar el tema del flags. Debe ser que
            si definimos que nuesrto nodo tiene poca memoria no queremos guardar
            todo esto.*/
                /*Los flags del paquete recibido.*/
                Buffer->flags.Val = rxMessage.flags.Val;
                /*El ID de la PAN a la que pertenece el nodo origen.*/
                Buffer->SourcePANID = rxMessage.SourcePANID;
                /*La direccion del nodo origen.*/
                if(MACRxPacket.altSourceAddress) //Si se ha recibido la altaddress.
                {
                    /*Buscamos la direccion larga correspondiente a la corta recibida. Esto era
                     por el tema de que si el nodo emisor es conocido no habia ningun puntero a
                     la direccion larga, a pesar de que esta si se encuentra en el buffer de RX.*/
                    j = GuilJa_MiWi_Search4ShorAddrss(MACRxPacket.SourceAddress, ConnectionTable);
                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        Buffer->SourceAddress[i] = ConnectionTable[j].Address[i];
                    }
                }
                else
                {
                    for(i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        Buffer->SourceAddress[i] = MACRxPacket.SourceAddress[i];
                    }
                }
                /*El tamaño del payload del paquete recibido*/
                Buffer->PayloadSize = rxMessage.PayloadSize;
                /*El RSSI para el paquete recibido.*/
                Buffer->PacketRSSI= rxMessage.PacketRSSI;
                /*El LQI para el paquete recibido.*/
                Buffer->PacketLQI = rxMessage.PacketLQI;
            #endif

        for (i = 0; i < rxMessage.PayloadSize; i++) /*Copiamos el contenido.*/
        {
            Buffer->Payload[i] = rxMessage.Payload[i];
            //TODO a lo mejor podiamos poner una restriccion en este if para que
            //soltara un error si el tamaño del buffer es menor que el del
            //payload size.
        }

        /*******************************************************************/
        // Function MiApp_DiscardMessage is used to release the current
        // received message. After calling this function, the stack can
        // start to process the next received message.
        /*******************************************************************/
        MiApp_DiscardMessage();

        return TRUE;
    }
    else
        return FALSE;
    return FALSE;
}

/*********************************************************************
 * Function:        BYTE GuilJa_MiWi_Search4ShorAddrss(INPUT BYTE *DireccionCorta)
 *
 * PreCondition:    tempShortAddress and tempPANID are set to the device
 *                  that you are looking for
 *
 * Input:           INPUT BYTE *DireccionCorta, el puntero a la direccion corta
 *                  de la cual queremos saber el indice de la tabla.
 *
 * Output:          BYTE - the index of the network table entry of the
 *                  requested device.  0xFF indicates that the requested
 *                  device doesn't exist in the network table
 *
 * Side Effects:    None
 *
 * Overview:        This function looks up the index of a node or network
 *                  in the network table by short address.
 ********************************************************************/
BYTE GuilJa_MiWi_Search4ShorAddrss(INPUT BYTE *DireccionCorta, INPUT CONNECTION_ENTRY *Tabla)
{
    BYTE i;

    WORD_VAL tmpDireccionCorta;

    tmpDireccionCorta.v[0] = DireccionCorta[0];
    tmpDireccionCorta.v[1] = DireccionCorta[1];

    for(i=0;i<CONNECTION_SIZE;i++)
    {
        if(Tabla[i].status.bits.isValid && Tabla[i].status.bits.shortAddressValid)
        {
            if(Tabla[i].AltAddress.Val == tmpDireccionCorta.Val)
            {
                return i;
            }
        }
    }
    return 0xFF;
}


//XXX-GuilJA Esta función esta porque este archivo venia de ManejoDatos.c y la
//habia implementado alli para utilizar a modo de pasarela.
///******************************************************************************/
///*
// * Nombre: void PasarelaDatos(void)
// * Función: Con los datos que llegan se llena el buffer de transmision
// * directamente para que los datos puedan ser enviados de nuevo por ejemplo a
// * otro nodo.
// * Devuelve: nada
// * Parametros: el buffer pequeño de transmisión.
// *
// * NOTA: PayloadSize es un byte luego no puede ser mayor de 128.
// * Fijo para una cabecera de dos bytes. Se podría hacer variable añadiendo por
// * ejemplo un campo de entrada al método.
// */
//void PasarelaDatos(void)
//{
//    BYTE i;
//    for(i = 0; i < rxMessage.PayloadSize; i++)
//    {
//        BufferTx[i] = rxMessage.Payload[i];
//
//    }
//    EnvioDatosPaquete();
//
//    if(rxMessage.SourceAddress[0]==ConnectionTable[0].AltAddress.byte.LB && rxMessage.SourceAddress[1]==ConnectionTable[0].AltAddress.byte.HB)
//        MiApp_UnicastAddress(ConnectionTable[1].AltAddress.v, FALSE, TRUE);
//    else
//        MiApp_UnicastAddress(ConnectionTable[0].AltAddress.v, FALSE, TRUE);
//
//}

/*****************************************************************************/
/******************************FIN DE FUNCIONES*******************************/
/*****************************************************************************/
