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
 * Funci�n: Enviar los datos contenido en el buffer peque�o de transmisi�n.
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
 * Funci�n: Llenar el buffer de recepci�n peque�o con los elementos que le
 * llegan.
 * Devuelve: nada
 * Parametros: el buffer peque�o de recepci�n.
 *
 * NOTA: Esta pensado para que haya menos de 2 elevado a 16 paquetes.
 * Fijo para una cabecera de dos bytes. Se podr�a hacer variable a�adiendo por
 * ejemplo un campo de entrada al m�todo.
 */
void ReciboDatos(void);

/******************************************************************************/
/*
 * Nombre: void PasarelaDatos(void)
 * Funci�n: Con los datos que llegan se llena el buffer de transmision
 * directamente para que los datos puedan ser enviados de nuevo por ejemplo a
 * otro nodo.
 * Devuelve: nada
 * Parametros: el buffer peque�o de transmisi�n.
 *
 * NOTA: PayloadSize es un byte luego no puede ser mayor de 128.
 * Fijo para una cabecera de dos bytes. Se podr�a hacer variable a�adiendo por
 * ejemplo un campo de entrada al m�todo.
 */
void PasarelaDatos(void);




#endif
