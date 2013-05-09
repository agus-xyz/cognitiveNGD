/*****************************************************************************
 *
 *              CommModule.c -- Communication Module v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        CommModule.c
 * Dependencies:    CommModule.h, ConfigCommModule.h
 * Processor:
 * BOARD:
 * Compiler:        C32 01.00.02 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         B105 -FAILURE IS NOT AN OPTION-
 *
 *****************************************************************************
 * File Description:
 *
 * Este archivo implementa el modulo de comunicaciones.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   23/10/2012    Initial revision
 *****************************************************************************/
#include "CRModule/HAL/HAL.h"
#include "CRModule/HAL/ConfigHAL.h"

/*Parte necesaria para MiWi*/
#define LIGHT   0x01
#define SWITCH  0x02

#if defined(Nodo1)
//////Tal y como viene en el ejemplo de MiWi_DE feature node 1.
//    #if ADDITIONAL_NODE_ID_SIZE > 0
//         BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {LIGHT};
//    #endif
//#elif defined(Nodo2)
//    #if ADDITIONAL_NODE_ID_SIZE > 0
//        BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {SWITCH};
//    #endif
//#endif
//XXX-GuilJa ahora lo ponemos con la info que nosotros queremos.
    #if ADDITIONAL_NODE_ID_SIZE > 0
        BYTE AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {25, 23};
    #endif
#endif
//Fin de parte necesaria para MiWi

    
//Funciones personales de la HAl
/*que quizas deberia proporcionar la HAL que herede el CR_Module puesto que
 * recordar que el sub-modulo HAL presente en este proyecto debería ser
provisional.*/
//PRUEBA
void CRM_HAL_ChanHop(BYTE DestChan)
{
    MiApp_SetChannel(DestChan);
}