#include "WirelessProtocols/MCHP_API.h"
#include "GuilJa Utils Heads/RxBasico.h"
#include "WirelessProtocols/Console.h"


/******************************************************************************/
/*
 * Nombre: BOOL Comprobar_Recepcion(void)
 * Función: Comprobar si hay datos para recibir y mostrarlos por pantalla. Es
 *          más una función para debugear ya que no siempre tiene sentido
 *          mostrar los datos por pantalla.
 * Devuelve: -TRUE: si se han recibido datos.
 *           -FALSE: si no se han recibido datos.
 * Parametros: nada.
 *
 */
BOOL Comprobar_Recepcion(void)
{
    BYTE i;
    BOOL bReceivedMessage=FALSE;
    if (MiApp_MessageAvailable())
    {
        Printf("\r\n"); //XXX-Willy
        /*******************************************************************/
        //             If a packet has been received, following code prints out some of
        //             the information available in rxFrame.
        /*******************************************************************/
        #ifdef ENABLE_SECURITY
            if (rxMessage.flags.bits.secEn)
            {
                ConsolePutROMString((ROM char *) "Secured ");
            }
        #endif
        #ifndef TARGET_SMALL
            if (rxMessage.flags.bits.broadcast)
            {
                ConsolePutROMString((ROM char *) "Broadcast Packet with RSSI ");
            }
            else
            {
                ConsolePutROMString((ROM char *) "Unicast Packet with RSSI ");
            }
            PrintChar(rxMessage.PacketRSSI);
            if (rxMessage.flags.bits.srcPrsnt)
            {
                ConsolePutROMString((ROM char *) " from ");
                if (rxMessage.flags.bits.altSrcAddr)
                {
                    PrintChar(rxMessage.SourceAddress[1]);
                    PrintChar(rxMessage.SourceAddress[0]);
                }
                else
                {
                    for (i = 0; i < MY_ADDRESS_LENGTH; i++)
                    {
                        PrintChar(rxMessage.SourceAddress[MY_ADDRESS_LENGTH - 1 - i]);
                    }
                }
            }
        #endif
        ConsolePutROMString((ROM char *) ": ");

        //XXX-Willy. Lo comento para hacer mis pruebas.
        for (i = 0; i < rxMessage.PayloadSize; i++)
        {
            ConsolePut(rxMessage.Payload[i]);
        }
        //XXX-Willy. Para que almacene lo recibido en BufferRx y en el


        LED_2 ^= 1;

        /*******************************************************************/
        // Function MiApp_DiscardMessage is used to release the current
        // received message. After calling this function, the stack can
        // start to process the next received message.
        /*******************************************************************/
        MiApp_DiscardMessage();

        bReceivedMessage = TRUE;

        /*******************************************************************/
        // Following block update the total received and transmitted messages
        // on the LCD of the demo board.
        /*******************************************************************/
    }
    return bReceivedMessage;
}
