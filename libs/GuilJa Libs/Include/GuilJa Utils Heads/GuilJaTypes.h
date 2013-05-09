/*****************************************************************************
 *
 *              GuilJaTypes.h -- GuilJaTypes v0.1
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
 * Un header donde crearé tipos extra genéricos para incluir en proyectos.
 *
 * Change History:
 *  Rev   Date(m/d/y)   Description
 *  0.1   6/11/2012    Initial revision
 *****************************************************************************/
#ifndef  _GUILJATYPES_H_
#define  _GUILJATYPES_H_

#define INPUT
#define OUTPUT
#define IOPUT

/*CRMODULE*/
//Una definicion de un tipo para definir los sub-modulos cognitivos, le añadimos
//una que note que queremos comunicarnos con un nodo externo.
typedef enum _SUB_MODULE {SubM_AccCtrl = 0x01, SubM_Disc, SubM_Exec, SubM_Opt, SubM_Poli, SubM_Repo, SubM_Mssngr, SubM_Ext} SUB_MODULE;
//Para definir los dos procesadores de mensajes.
typedef enum _MSSGPR {VCC = 0x01, NMM} MSSGPR;
/*fin de CRMODULE*/


#endif
