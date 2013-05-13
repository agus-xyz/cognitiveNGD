/*****************************************************************************
 *
 *              GuilJa_MiWi_Utils.h -- GuilJa MiWi Utilities Header v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        GuilJa_MiWi_Utils.h
 * Dependencies:    Console.h, MCHP_API.h, ManejoDatos.h
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

#ifndef GuilJa_MiWi_Utils_H
#define GuilJa_MiWi_Utils_H

#include "WirelessProtocols/Console.h"
#include "WirelessProtocols/MCHP_API.h"
#include "GuilJa Utils Heads/MiWi Utils/Config_GuilJa_MiWi_Utils.h"

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
BOOL GuilJa_MiWi_Send_Buffer(BYTE *Buffer, BYTE *Address);

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
BOOL GuilJa_MiWi_Rcvd_Buffer(RECEIVED_MESSAGE *Buffer, BYTE *SizeOfBuffer);

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
BYTE GuilJa_MiWi_Search4ShorAddrss(INPUT BYTE *DireccionCorta, INPUT CONNECTION_ENTRY *Tabla);


//XXX-GuilJA Esta función esta porque este archivo venia de ManejoDatos.c y la
//habia implementado alli para utilizar a modo de pasarela.
///******************************************************************************/
///*
// * Nombre: void PasarelaDatos(void)
// * Funci�n: Con los datos que llegan se llena el buffer de transmision
// * directamente para que los datos puedan ser enviados de nuevo por ejemplo a
// * otro nodo.
// * Devuelve: nada
// * Parametros: el buffer peque�o de transmisi�n.
// *
// * NOTA: PayloadSize es un byte luego no puede ser mayor de 128.
// * Fijo para una cabecera de dos bytes. Se podr�a hacer variable a�adiendo por
// * ejemplo un campo de entrada al m�todo.
// */
//void PasarelaDatos(void);

/*****************************************************************************/
/******************************FIN DE FUNCIONES*******************************/
/*****************************************************************************/

#endif
