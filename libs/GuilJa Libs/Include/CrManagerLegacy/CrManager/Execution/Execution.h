/*****************************************************************************
 *
 *              Execution.h -- Execution Module v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        Execution.h
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
 * Implementa las funciones de ejecucion que llevaran a cabo en el nodo las
 * decisiones tomadas por el optimizador.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   5/4/2012    Initial revision
 *****************************************************************************/
#ifndef __EXECUTION_H_
#define __EXECUTION_H_

#include "WirelessProtocols/MCHP_API.h"


//XXX-Willy. Definida en MiWi.c para cambiar de canal. La declaro aquí porque
//no está declarada ni en MiWi.h ni en MCHP_API.h y con el fin de modificar lo
//mínimo la pila.
void StartChannelHopping(INPUT BYTE OptimalChannel);


void ExecutorChannHop(INPUT BYTE HopToChannel);

#endif