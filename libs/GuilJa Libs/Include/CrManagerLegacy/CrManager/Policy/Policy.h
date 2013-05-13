/*****************************************************************************
 *
 *              Policy.h -- Policy Module v0.1
 *
 *****************************************************************************
 *
 * Author:          Guillermo Jara Luengos
 * FileName:        Policy.h
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
 * Este archivo implementa el modulo correspondiente a las politicas del
 * modulo cognitivo (CR Module).
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   5/4/2012    Initial revision
 *****************************************************************************/
#ifndef __POLICY_H_
#define __POLICY_H_

#include "GenericTypeDefs.h"


/******************************************************************************/
/*
 * Nombre: typedef Policies
 * Función: Definir un tipo de elemento, que además es una estructura, que
 *          especifique los pesos que tienen diferentes parametros como consumo,
 *           seguridad, etc. y de esta forma tomar decisiones en cada nodo y en
 *          la red de acuerdo a los pesos de las politicas.
 */
typedef struct
{
    BYTE PowerConsm;
    BYTE Noise;
    BYTE LQI;
    BYTE Security;
}Policies;

#endif