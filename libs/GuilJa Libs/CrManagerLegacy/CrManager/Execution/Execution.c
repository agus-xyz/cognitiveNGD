/*****************************************************************************
 *
 *              Execution.c -- Execution Module v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        Execution.c
 * Dependencies:    Execution.h
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
#include "CrManager/Execution/Execution.h"
#include "WirelessProtocols/MiWi/MiWi.h"
#include "WirelessProtocols/MCHP_API.h"



/*********************************************************************
 * Function:        void ExecutorChannHo(INPUT BYTE HopeToChannel)
 *
 * PreCondition:
 *
 *
 * Input:           Canal al que va a saltar el nodo.
 *
 * Output:          None
 *
 * Side Effects:
 *
 *
 * Overview: Realiza un cambio de canal
 *
 ********************************************************************/
void ExecutorChannHop(INPUT BYTE HopToChannel)
{
    StartChannelHopping(HopToChannel);
}
