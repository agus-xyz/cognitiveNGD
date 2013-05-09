/******************************************************************************/
/*
 * Creado por: Guillermo Jara
 * Fecha:10-02-2012
 * Objetivo: Funciones de creación y llenado de un macro buffer con el fin de
 * enviar por la interfaz que sea un numero relativamente grande de paquetes y
 * estudiar el comportamiento de la red.
 */
#include <p32xxxx.h>
#include "ConfigApp.h"


#include "WirelessProtocols/Console.h"
#include "WirelessProtocols/P2P/P2P.h"
#include "WirelessProtocols/SymbolTime.h"
#include "Transceivers/Transceivers.h"
#include "WirelessProtocols/MCHP_API.h"
#include "WirelessProtocols/NVM.h"
#include "GuilJa Utils Heads/Buffer.h"
#include "GuilJa Utils Heads/ManejoDatos.h"

/*Tienen que estar definidos en alguno de los modulos que incluya*/
extern BYTE BufferTx[];
extern BYTE BufferRx[];

#if defined DATA_OVER_VCC && defined CRMODULE
    extern RECEIVED_MESSAGE BufferRecepcionPrueba;
#endif
    
/******************************************************************************/
/*
 * Nombre: void EnvioDatosPaquete(int BufferTx[])
 * Función: Enviar los datos contenido en el buffer pequeño de transmisión.
 * Devuelve: nada
 * Parametros: nada
 *
 * NOTA:
 * 
 */
void EnvioDatosPaquete(void)
{
    BYTE i;
    for(i = 0; i < TX_BUFFER_SIZE; i++)
    {
        MiApp_WriteData(BufferTx[i]);
    }
}

/******************************************************************************/
/*
 * Nombre: void ReciboDatos(int BufferRx[])
 * Función: Llenar el buffer de recepción pequeño con los elementos que le
 * llegan.
 * Devuelve: nada
 * Parametros: el buffer pequeño de recepción.
 *
 * NOTA: PayloadSize es un byte y no puede ser mayor de 128.
 * Fijo para una cabecera de dos bytes. Se podría hacer variable añadiendo por
 * ejemplo un campo de entrada al método.
 */
void ReciboDatos(void)
{
    BYTE i;
#if defined DATA_OVER_VCC && defined CRMODULE
    for(i = 0; i < BufferRecepcionPrueba.PayloadSize; i++)
    {
        BufferRx[i] = BufferRecepcionPrueba.Payload[i]; //XXX-GuilJa directamente al buffer de optimizer
#else
    for(i = 0; i < rxMessage.PayloadSize; i++)
    {
        BufferRx[i] = rxMessage.Payload[i];
#endif
 //       ConsolePut(rxMessage.Payload[i]);
    }

}
/******************************************************************************/
/*
 * Nombre: void PasarelaDatos(void)
 * Función: Con los datos que llegan se llena el buffer de transmision
 * directamente para que los datos puedan ser enviados de nuevo por ejemplo a
 * otro nodo.
 * Devuelve: nada
 * Parametros: el buffer pequeño de transmisión.
 *
 * NOTA: PayloadSize es un byte luego no puede ser mayor de 128.
 * Fijo para una cabecera de dos bytes. Se podría hacer variable añadiendo por
 * ejemplo un campo de entrada al método.
 */
void PasarelaDatos(void)
{
    BYTE i;
    for(i = 0; i < rxMessage.PayloadSize; i++)
    {
        BufferTx[i] = rxMessage.Payload[i];

    }
    EnvioDatosPaquete();

    if(rxMessage.SourceAddress[0]==ConnectionTable[0].AltAddress.byte.LB && rxMessage.SourceAddress[1]==ConnectionTable[0].AltAddress.byte.HB)
        MiApp_UnicastAddress(ConnectionTable[1].AltAddress.v, FALSE, TRUE);
    else
        MiApp_UnicastAddress(ConnectionTable[0].AltAddress.v, FALSE, TRUE);

}
