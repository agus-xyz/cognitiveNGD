/*****************************************************************************
 *
 *              Repository.h -- Repository Module v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        Repository.h
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
 * Este archivo desarrolla las funciones y datos del modulo repository
 * (CR Module).
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   5/4/2012    Initial revision
 *****************************************************************************/

#ifndef  _REPOSITORY_H_
#define  _REPOSITORY_H_

#include <GenericTypeDefs.h>

/************************ VARIABLES **********************************/
typedef enum {ESCANEO=0, MENSAJERX=1, SALTOCANAL=2}TIPOINFO;

/************************ FUNCTIONS **********************************/

    /*********************************************************************
     * Function:        void GuardarInfo(void)
     *
     * PreCondition:
     *
     *
     * Input:           None
     *
     * Output:          None
     *
     * Side Effects:
     *
     *
     * Overview:
     *
     *
     *
     ********************************************************************/
    BOOL GuardarInfo(TIPOINFO tipoinfo, BYTE IndiceDeEscaneoActivo);
    


#endif
