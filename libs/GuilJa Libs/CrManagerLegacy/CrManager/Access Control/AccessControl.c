/*****************************************************************************
 *
 *              AccessControl.c -- Access Control v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        AccessControl.c
 * Dependencies:    AccessControl.h
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
#include "CrManager/Access Control/AccessControl.h"

/******************************************************************************/
/*
 * Nombre: BOOL EnterRepoAccess(void)
 * Funci�n: Comprobar si el nodo que solicita acceder a la informaci�n del Repo
 *          del nodo "local" (de �ste nodo) tiene permiso para hacerlo.
 * Devuelve: -TRUE: si s� que tiene permitido el acceso.
 *           -FALSE: si no tiene permitido el acceso.
 * Parametros: nada. Pero habra que poner algo, ya sea la direcci�n corta, la
 *             absoluta, o alg�n indice de la tabla de conexiones o algo.
 *
 */
BOOL EnterRepoAccess(void)
{
    return TRUE;
}