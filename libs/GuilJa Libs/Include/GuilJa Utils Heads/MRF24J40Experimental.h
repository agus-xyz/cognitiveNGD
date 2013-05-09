#include "Transceivers/MRF24J40/MRF24J40.h"
#include "GenericTypeDefs.h"
#include "WirelessProtocols/Console.h"

#define ShortAddressReg (0x00)
#define LongAddressReg (0x01)
#define BothLongAndShortReg (0x02)

#define OpAND (0x00)
#define OpOR (0x01)
#define OpXOR (0x02)


extern BYTE RegistrosCortos[64];
extern BYTE RegistrosLargos[77];
extern BYTE RegistrosCortosTemp[64];
extern BYTE RegistrosLargosTemp[77];

extern BYTE RegistrosCortos2[64];
extern BYTE RegistrosLargos2[77];
extern BYTE RegistrosCortosTemp2[64];
extern BYTE RegistrosLargosTemp2[77];


void CopiarRegistros (BYTE TipoRegistro, BYTE Registro[]);
void OperarRegistros(BYTE TipoRegistro, BYTE Operacion, BYTE Destino[], BYTE Origen[]);
void ImprimirRegistro(BYTE TipoRegistro, BYTE Registro[]);
