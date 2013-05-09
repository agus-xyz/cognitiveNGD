#if defined(MRF24J40)
    #include "MisLibrerias/MRF24J40Experimental.h"

    #include "GenericTypeDefs.h"

    #define BaseShortRegAdd (0x00)
    #define BaseLongRegAdd (0x0200)

    #define TamRegCortos 64
    #define TamRegLargos 77

    BYTE RegistrosCortos[64];
    BYTE RegistrosLargos[77];
    BYTE RegistrosCortosTemp[64];
    BYTE RegistrosLargosTemp[77];

    BYTE RegistrosCortos2[64];
    BYTE RegistrosLargos2[77];
    BYTE RegistrosCortosTemp2[64];
    BYTE RegistrosLargosTemp2[77];

    void CopiarRegistros (BYTE TipoRegistro, BYTE Registro[])
    {
        #if defined(MRF24J40)
            BYTE i;
            switch(TipoRegistro)
            {
                case ShortAddressReg:
                    for(i=0; i< TamRegCortos; i++)
                    {
                        Registro[i] = PHYGetShortRAMAddr(BaseShortRegAdd+2*i);
                    }
                    break;
                case LongAddressReg:
                    for(i=0; i< TamRegLargos; i++)
                    {
                        Registro[i] = PHYGetLongRAMAddr(BaseLongRegAdd+i);
                    }
                    break;
                case BothLongAndShortReg:
                    for(i=0; i< TamRegCortos; i++)
                    {
                        Registro[i] = PHYGetShortRAMAddr(BaseShortRegAdd+2*i);
                    }
                    for(i=0; i< TamRegLargos; i++)
                    {
                        Registro[i] = PHYGetLongRAMAddr(BaseLongRegAdd+i);
                    }
                    break;
                default:
                    Printf("\r\nEl tipo de registro seleccionado no existe.");

            }
        #endif
    }

    void OperarRegistros(BYTE TipoRegistro, BYTE Operacion, BYTE Destino[], BYTE Origen[])
    {
        BYTE i;
        switch(TipoRegistro)
        {
            case ShortAddressReg:
                switch (Operacion)
                {
                    case OpAND:
                        for(i=0; i< TamRegCortos; i++)
                        {
                            Destino[i] = Destino[i] & Origen[i];
                        }
                        break;
                    case OpOR:
                        for(i=0; i< TamRegCortos; i++)
                        {
                            Destino[i] = Destino[i] | Origen[i];
                        }
                        break;
                    case OpXOR:
                        for(i=0; i< TamRegCortos; i++)
                        {
                            Destino[i] = Destino[i] ^ Origen[i];
                        }
                        break;
                    default:
                        Printf("\r\nLa operacion seleccionada no esta contemplada.");
                }
                break;
            case LongAddressReg:
                switch (Operacion)
                {
                    case OpAND:
                        for(i=0; i< TamRegLargos; i++)
                        {
                            Destino[i] = Destino[i] & Origen[i];
                        }
                        break;
                    case OpOR:
                        for(i=0; i< TamRegLargos; i++)
                        {
                            Destino[i] = Destino[i] | Origen[i];
                        }
                        break;
                    case OpXOR:
                        for(i=0; i< TamRegLargos; i++)
                        {
                            Destino[i] = Destino[i] ^ Origen[i];
                        }
                        break;
                    default:
                        Printf("\r\nLa operacion seleccionada no esta contemplada.");
                }
                break;
            case BothLongAndShortReg:
                switch (Operacion)
                {
                    case OpAND:
                        for(i=0; i< TamRegCortos; i++)
                        {
                            Destino[i] = Destino[i] & Origen[i];
                        }
                        for(i=0; i< TamRegLargos; i++)
                        {
                            Destino[i] = Destino[i] & Origen[i];
                        }
                        break;
                    case OpOR:
                        for(i=0; i< TamRegCortos; i++)
                        {
                            Destino[i] = Destino[i] | Origen[i];
                        }
                        for(i=0; i< TamRegLargos; i++)
                        {
                            Destino[i] = Destino[i] | Origen[i];
                        }
                        break;
                    case OpXOR:
                        for(i=0; i< TamRegCortos; i++)
                        {
                            Destino[i] = Destino[i] ^ Origen[i];
                        }
                        for(i=0; i< TamRegLargos; i++)
                        {
                            Destino[i] = Destino[i] ^ Origen[i];
                        }
                        break;
                    default:
                        Printf("\r\nLa operacion seleccionada no esta contemplada.");
                }
                break;
            default:
                Printf("\r\nEl tipo de registro seleccionado no existe.");

        }

    }

    void ImprimirRegistro(BYTE TipoRegistro, BYTE Registro[])
    {
        BYTE i;
        Printf("\r\n");

        switch(TipoRegistro)
        {
            case ShortAddressReg:
                for(i=0; i< TamRegCortos; i++)
                {
                    PrintDec(Registro[i]);
                    Printf("\r\n");
                }
                break;
            case LongAddressReg:
                for(i=0; i< TamRegLargos; i++)
                {
                    PrintDec(Registro[i]);
                    Printf("\r\n");
                }
                break;
            case BothLongAndShortReg:
                for(i=0; i< TamRegCortos; i++)
                {
                    PrintDec(RegistrosCortos[i]);
                    Printf("\r\n");
                }
                for(i=0; i< TamRegLargos; i++)
                {
                    PrintDec(RegistrosLargos[i]);
                    Printf("\r\n");
                }
                break;
            default:
                Printf("\r\nEl tipo de registro seleccionado no existe.");

        }

    }
#endif