/*
 * File:   ManejoDatos.h
 * Author: guilja
 *
 * Created on 16 de febrero de 2012, 12:22
 */

#ifndef ManejoDatos_H
#define ManejoDatos_H

#include "ConfigApp.h"


//TODO: USER CODE

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
void EnvioDatosPaquete(void);

/******************************************************************************/
/*
 * Nombre: void ReciboDatos(int BufferRx[])
 * Función: Llenar el buffer de recepción pequeño con los elementos que le
 * llegan.
 * Devuelve: nada
 * Parametros: el buffer pequeño de recepción.
 *
 * NOTA: Esta pensado para que haya menos de 2 elevado a 16 paquetes.
 * Fijo para una cabecera de dos bytes. Se podría hacer variable añadiendo por
 * ejemplo un campo de entrada al método.
 */
void ReciboDatos(void);

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
void PasarelaDatos(void);




#endif
