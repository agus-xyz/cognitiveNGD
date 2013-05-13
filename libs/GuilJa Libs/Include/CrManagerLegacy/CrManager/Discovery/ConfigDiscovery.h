/*****************************************************************************
 *
 *              ConfigDiscovery.c -- ConfigDiscovery v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:
 * Dependencies:
 * Processor:
 * BOARD:
 * Compiler:        C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         B105 -FAILURE IS NOT AN OPTION-
 *
 *****************************************************************************
 * File Description:
 *
 * Este archivo se ocupa de configurar el modulo discovery, entendiendo por
 * configuración funciones tales como activar la salida por el puerto serie
 * para debugeo.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   5/4/2012    Initial revision
 *****************************************************************************/
#ifndef  _CONFIGDISCOVERY_H_
#define  _CONFIGDISCOVERY_H_

/*********************************************************************/
// DEBUG_VISUAL nos sirve para decidir si sacamos por el puerto serie, 
// o USB segun lo tengamos configurado, las lineas de debugeo.
/*********************************************************************/
#define DEBUG_VISUAL

/*********************************************************************/
//CARRIER_SCAN. La comentamos de tal manera que lancemos un error en
//compilación si intentamos utilizar el escaneo de portadoras. Ya que
//por el momento no sabemos como utilizarlo en el MRF24J40 suponiendo
//que sea posible puesto que oficialmente no esta disponible.
/*********************************************************************/
//#define CARRIER_SCAN

/*********************************************************************/
// MRF24J40Exp sirve para tener disponibles funciones que he creado con
// el fin de experimentar con los registros en busca del bit de CCA o
// tambien por si encontraba el registro donde se indica la deteccion
// de portadoras.
/*********************************************************************/
//#define MRF24J40Exp

#endif