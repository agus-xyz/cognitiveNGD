/*
 * File:   Buffer.h
 * Author: guilja
 *
 * Created on 10 de febrero de 2012, 12:37
 */

#ifndef Buffer_H
#define Buffer_H

//TODO: USER CODE

#include "GenericTypeDefs.h"
#include "ConfigApp.h"

//int tamano=100000;//El tama�o del buffer que queremos crear.
//
//int Buffer[100000/4]; //Un buffer grande.
//#define tamano 100000;//El tama�o del buffer que queremos crear.

/******************************************************************************/
/*
 * Nombre: UINT32 RellenaBuffer(UINT32 tamanoPaqueteEfectivo)
 * Funci�n: llenar de bits el buffer que hemos creado. Lo llenara de numeros
 *          incrementales con el objetivo de que los datos sean heterogeneos
 *          porque quiz�s esto favorezca la aparici�n de errores.
 * Devuelve: numero de posiciones escritos en el buffer.
 * Parametros: el tama�o que queremos que tenga cada paquete.
 *
 * NOTA: Esta pensado para que para que haya menos de 2 elevado a 16 paquetes.
 *
 */
UINT16 CreaBufferTx(BYTE tamanoPaqueteEfectivo);


/******************************************************************************/
/*
 * Nombre: void RellenaBuffer(void)
 * Funci�n: Llenar el buffer de recepci�n con los elementos que le llegan.
 * Suprime las cabecera para obtener una copia que deber�a ser exacta del buffer
 *  creado en transmisi�n.
 * Devuelve: nada
 * Parametros: el buffer peque�o de recepci�n y el indice del paquete que es.
 *
 * NOTA: Esta pensado para que para que haya menos de 2 elevado a 16 paquetes.
 * Fijo para una cabecera de dos bytes. Se podr�a hacer variable a�adiendo por
 * ejemplo un campo de entrada al m�todo.
 */
void RellenaBuffer(void);

/******************************************************************************/
/*
 * Nombre: void RellenaBufferTx(int BufferTx[], int indPaquete)
 * Funci�n: Llenar el buffer de transmision con los elementos que le
 *          correspondan del Buffer grande. Por simplificaci�n, puesto que se
 *          trata de hacer pruebas, �ste m�todo tambi�n rellena la cabecera de
 *          los paquetes.
 * Devuelve: nada
 * Parametros: el buffer de transmisi�n y el indice del paquete que es.
 *
 * NOTA: Esta pensado para que para que haya menos de 2 elevado a 16 paquetes.
 */
void RellenaBufferTx(UINT16 indPaquete);


/******************************************************************************/
/*
 * Nombre: int sum_array(int BufferTx[], int num_elements)
 * Funci�n: sumar los elementos de un array que le pasamos como parametro.
 * Devuelve: el valor de la suma.
 * Parametros: el array sobre el que queremos que haga la suma y el indice del
 *             array a partir del cual queremos sumar. Por si hay cabeceras he
 *             hecho esto.
 *
 */
int sum_array(int inicioSuma);

/******************************************************************************/
/*
 * Nombre: BOOL CheckSum(int BufferTx[], int cabecera)
 * Funci�n: comprobar que la funci�n sum devuelve la suma de la progresion
 *          aritm�tica.
 * Devuelve: verdadero o falso.
 * Parametros: el array sobre el que queremos que compruebe la suma y el numero
 *             de posiciones de cabecera para que no los tenga en cuenta a la hora de
 *             realizar la suma.
 *
 */
BYTE CheckSum(BYTE Buffer[], BYTE cabecera);

/******************************************************************************/
/*
 * Nombre: void LimpiaBuffer(BYTE Buffer[])
 * Funci�n: vac�a el buffer que le pasemos como par�metro para asegurar que al
 *          escribir en el sucesivamente no haya datos de escrituras previas.
 * Devuelve: Nada.
 * Parametros: el array que queremos que relleno de ceros.
 *
 */
void LimpiaBuffer(void);
void LimpiaBufferTx(void);
void LimpiaBufferRx(void);



#endif
