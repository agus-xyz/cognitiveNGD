/*******************************************************************************
 *                  Circular Buffer
 *******************************************************************************
 * FileName:        CircularBuffer.h
 * Dependencies:    Compiler.h
 * Author:          Fernando López Lara   - Laboratorio de Sistemas Integrados
 * Revision:        Juan Domingo Rebollo  - Laboratorio de Sistemas Integrados
 ******************************************************************************/

/* Includes *******************************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"

/* Definitions ****************************************************************/
#define CIRCULAR_BUFFER_LENGTH      512
#define USB_OUT_BUFFER_MAX_LENGTH   64
#define USB_IN_BUFFER_MAX_LENGTH    64
#define CIRC_BUFFER_ERROR_CODE      0x7E    //Fernando - Printable symbol '~'

/* Data types and structures **************************************************/
typedef struct{

	UINT16 nextRead;        //Next data position to be read
	UINT16 nextWrite;       //Next data position to be written
	UINT16 dataLength;      //Counter for data available in the buffer
	char dataBuffer[CIRCULAR_BUFFER_LENGTH];        //Data

}CIRCULAR_BUFFER;

/* Function prototypes ********************************************************/
void initCircularBuffer(CIRCULAR_BUFFER *circ_buffer);
char getCircularBuffer(CIRCULAR_BUFFER *circ_buffer);
char putCircularBuffer(CIRCULAR_BUFFER *circ_buffer, char *data);
UINT16 getCircularBufferLength(CIRCULAR_BUFFER *circ_buffer);
char copyArrayToCircularBuffer(CIRCULAR_BUFFER *circ_buffer, char *data, INT16 inputLength);