/*****************************************************************************
 *
 *              AccessControl.h -- Access Control v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        AccessControl.h
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
 * Este archivo implementa el control de acceso al repositorio.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   5/4/2012    Initial revision
 *****************************************************************************/
#ifndef __ACCESSCONTROL_H_
#define __ACCESSCONTROL_H_

#include "GenericTypeDefs.h"

/******************************************************************************/
/*
 * Nombre: BOOL EnterRepoAccess(void)
 * Función: Comprobar si el nodo que solicita acceder a la información del Repo
 *          del nodo "local" (de éste nodo) tiene permiso para hacerlo.
 * Devuelve: -TRUE: si sí que tiene permitido el acceso.
 *           -FALSE: si no tiene permitido el acceso.
 * Parametros: nada. Pero habra que poner algo, ya sea la dirección corta, la
 *             absoluta, o algún indice de la tabla de conexiones o algo.
 *
 */
BOOL EnterRepoAccess(void);

#endif