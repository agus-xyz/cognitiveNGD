/*****************************************************************************
 *
 *              Buffer.c -- Mis Librerias
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        Buffer.c
 * Dependencies:    GenericTypeDefs.h, Buffer.h, ConfigApp.h, Console.h
 * Processor:
 * BOARD:
 * Compiler:        C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         B105 -FAILURE IS NOT AN OPTION-
 *
 *****************************************************************************
 * File Description:
 *
 * Funciones de creación y llenado de un macro buffer con el fin de
 * enviar por la interfaz que sea un numero relativamente grande de paquetes y
 * estudiar el comportamiento de la red.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   10/2/2012    Initial revision
 *****************************************************************************/


#include "GenericTypeDefs.h"
#include "GuilJa Utils Heads/Buffer.h"
#include "ConfigApp.h"
#include "WirelessProtocols/Console.h"

UINT16 tamano=30000;//El tamaño del buffer que queremos crear.


BYTE Buffer[30000]; //Un buffer grande.
BYTE BufferTx[TX_BUFFER_SIZE];
BYTE BufferRx[RX_BUFFER_SIZE];

/******************************************************************************/
/*
 * Nombre:  CreaBufferTx(BYTE tamanoPaqueteEfectivo)
 * Función: llenar de bits el buffer que hemos creado. Lo llenara de numeros
 *          incrementales con el objetivo de que los datos sean heterogeneos
 *          porque quizás esto favorezca la aparición de errores.
 * Devuelve: numero de posiciones escritos en el buffer.
 * Parametros: el tamaño que queremos que tenga cada paquete.
 *
 * NOTA: Esta pensado para que para que haya menos de 2 elevado a 16 paquetes.
 *
 */
UINT16 CreaBufferTx(BYTE tamanoPaqueteEfectivo)
{
    BYTE j;
    UINT16 numeroPaquetes;
    UINT16 indPaquete;//Un indice para saber el numero de paquete por el que vamos.
    numeroPaquetes = (tamano/tamanoPaqueteEfectivo); //podriamos ponerlo para que mirara
                                           //si la division es entera pero
                                           //tampoco hay que forzar :).

        for(indPaquete=0; indPaquete<numeroPaquetes ; indPaquete++)
        {
            for(j=0; j<tamanoPaqueteEfectivo; j++)
            {
                Buffer[indPaquete*tamanoPaqueteEfectivo+j]= j;
            }
        }

    return tamanoPaqueteEfectivo*indPaquete;
}

/******************************************************************************/
/*
 * Nombre: void RellenaBuffer(void)
 * Función: Llenar el buffer de grande recepción con los elementos que le
 * llegan y que ha almacenado en el buffer pequeño de recepción.
 * Suprime las cabecera para obtener una copia que debería ser exacta del buffer
 *  creado en transmisión.
 * Devuelve: nada
 * Parametros: el buffer pequeño de recepción y el indice del paquete que es.
 *
 * NOTA: Esta pensado para que para que haya menos de 2 elevado a 16 paquetes.
 * Fijo para una cabecera de dos bytes. Se podría hacer variable añadiendo por
 * ejemplo un campo de entrada al método.
 */
void RellenaBuffer(void)
{
    BYTE i;
    BYTE cabec=2;
    BYTE tamanoPaquete= sizeof(BufferRx)-cabec;
    UINT16 indPaquete;

    indPaquete= (BufferRx[0]-1)*256+BufferRx[1];

    for(i=2; i<(tamanoPaquete+cabec); i++)
    {
        Buffer[indPaquete*tamanoPaquete+i-cabec] = BufferRx[i];
    }

}



/******************************************************************************/
/*
 * Nombre: void RellenaBufferTx(UINT16 indPaquete)
 * Función: Llenar el buffer de transmision con los elementos que le
 *          correspondan del Buffer grande. Por simplificación, puesto que se
 *          trata de hacer pruebas, éste método también rellena la cabecera que
 *          yo he creado para los paquetes.
 * Devuelve: nada
 * Parametros: el indice del paquete que es.
 *
 * NOTA: Esta pensado para que para que haya menos de 2 elevado a 16 paquetes.
 */
void RellenaBufferTx(UINT16 indPaquete)
{
    BYTE i;
    BYTE cabec=2;
    BYTE tamanoPaquete= sizeof(BufferTx)-cabec;
#if defined CRMODULE && defined DATA_OVER_VCC
    BufferTx[0] = indPaquete / 256 + 2; /*Le sumo 2, uno por la cabecera de MiWi
                                         y el otro por la de CRMODULE. 0x00 es
                                         para los mensajes de control de MiWI y
                                         0x01 para el modulo cognitivo.*/
#else
    BufferTx[0]=indPaquete/256+1; /*Le sumo uno porque si la primera posicion de
                                   * los datos que envio es 0 el receptor parece
                                   * que se queda colgado. En realidad es porque
                                    el primer BYTE del payload si es 0x00 está
                                   reservado por MiWi para indicar que es un
                                   mensaje del tipo MiWi Wireless Networking
                                   Protocol Stack.*/
#endif

    BufferTx[1]=indPaquete%256;

    for(i=cabec; i<(tamanoPaquete+cabec); i++)
    {

        BufferTx[i]=Buffer[indPaquete*tamanoPaquete+i-cabec];
    }
    
}

/******************************************************************************/
/*
 * Nombre: int sum_array(int BufferTx[], int num_elements)
 * Función: sumar los elementos de un array que le pasamos como parametro.
 * Devuelve: el valor de la suma.
 * Parametros: el array sobre el que queremos que haga la suma y el indice del
 *             array a partir del cual queremos sumar. Por si hay cabeceras he
 *             hecho esto.
 *
 */
int sum_array(int inicioSuma)
{
   int i, sum=0;
   int num_elements = sizeof(BufferTx);
   for (i=inicioSuma; i<num_elements; i++)
   {
	 sum = sum + BufferTx[i];
   }
   return(sum);
}

/******************************************************************************/
/*
 * Nombre: BOOL CheckSum(BYTE BufferTx[], BYTE cabecera)
 * Función: comprobar que la función sum devuelve la suma de la progresion
 *          aritmética.
 * Devuelve: verdadero o falso.
 * Parametros: el array sobre el que queremos que compruebe la suma y el numero
 *             de posiciones de cabecera para que no los tenga en cuenta a la hora de
 *             realizar la suma.
 *
 */
BYTE CheckSum(BYTE Buffer[], BYTE cabecera)
{
    int n=sizeof(Buffer)-cabecera;
    int suma=0;
    int i;
    for (i=0; i<n; i++)
    {
        suma = suma + i;
    }
    //int suma= (n)*(0+(n-1))/2; //Suma de una progresión aritmetica
    if(suma=sum_array(cabecera))
        return 1;
    return 0;

}

/******************************************************************************/
/*
 * Nombre: void LimpiaBuffer(BYTE Buffer[])
 * Función: vacía el buffer que le pasemos como parámetro para asegurar que al
 *          escribir en el sucesivamente no haya datos de escrituras previas.
 * Devuelve: Nada.
 * Parametros: el array que queremos que relleno de ceros.
 *
 */
void LimpiaBuffer(void)
{
    UINT16 tamBuffer=sizeof(Buffer);
    UINT16 i;
    for(i=0;i<tamBuffer; i++)
    {
        Buffer[i]=0;
    }
}
void LimpiaBufferTx(void)
{
    BYTE tamBufferTx=sizeof(BufferTx);
    BYTE i;
    for(i=0;i<tamBufferTx; i++)
    {
        BufferTx[i]=0;
    }
}
void LimpiaBufferRx(void)
{
    BYTE tamBufferRx=sizeof(BufferRx);
    BYTE i;
    for(i=0;i<tamBufferRx; i++)
    {
        BufferRx[i]=0;
    }
}