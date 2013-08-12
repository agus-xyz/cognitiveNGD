/*******************************************************************************
 * File:   MainAppCode_NodeTest.c
 * Author: Juan Domingo Rebollo - Laboratorio de Sistemas Integrados - UPM
 *
 * File Description: Main function involving Application specific code. The HAL
 *                   is designed for including basically the header "NodeHAL.h".
 *                   This file intends to be both an example of the node use and
 *                   HAL Test and Usage code (App. Code)
 * Change History:
 * Rev   Date         Description
 ******************************************************************************/

#include "NodeHAL.h"
#include "WirelessProtocols/Console.h"    //Pruebas


#if defined MIWI_2400_RI
    radioInterface ri = MIWI_2400;
    BYTE NumChannels = MIWI2400NumChannels;
    BYTE powerStep = 8;
    BYTE ri_RI_MASK = MIWI_2400_RI_MASK;
#elif defined MIWI_0868_RI
    radioInterface ri = MIWI_0868;
    BYTE NumChannels = MIWI0868NumChannels;
    BYTE powerStep = 32;
    BYTE ri_RI_MASK = MIWI_0868_RI_MASK;
#elif defined MIWI_0434_RI
    radioInterface ri = MIWI_0434;
    BYTE NumChannels = MIWI0434NumChannels;
    BYTE powerStep = 32;
    BYTE ri_RI_MASK = MIWI_0434_RI_MASK;
#endif


void InitAppVariables(){
    //Si las hubiera...
}

void PrintTestAddress(BYTE AddrMode, BYTE* Address){
    BYTE i;
    BYTE j = 0;
    Printf("\r\nAddrMode: ");
    switch(AddrMode){
        case LONG_MIWI_ADDRMODE:
            Printf("LONG_ADDR    Address: ");
            j = MY_ADDRESS_LENGTH;
            break;
        case SHORT_MIWI_ADDRMODE:
            Printf("SHORT_ADDR   Address: ");
            j = 2;
            break;
        default:
            Printf("ERROR");
            break;
    }
    for(i=0; i<j; i++){
        PrintChar(*(Address+j-1-i));
    }
}

int mainApp(void) {

INIT_STAGE:
    InitAppVariables();
    InitNode();
  //  ConsoleInit();
BYTE uno = 1;
BYTE diez = 10;
//    Printf("\n\r");
    //PrintDec(uno);
  //  Printf("\n\r");
   // Printf("\n\r");
   // Printf("\n\r");
    //PrintDec(diez);

/*
    BYTE a = 'a';
    BYTE b = 'b';
    BYTE c = 'c';

    ConsoleInit();
while(1){

    DelayMs(500);
    /*Printf("\r\n1 Starting MiWi(TM) LSI-CWSN Stack ...");
    Printf("\r\n2 Starting MiWi(TM) LSI-CWSN Stack ...");
    Printf("\r\n3 Starting MiWi(TM) LSI-CWSN Stack ...");
    Printf("\r\n4 Starting MiWi(TM) LSI-CWSN Stack ...");
    Printf("\r\n5 Starting MiWi(TM) LSI-CWSN Stack ...");
    Printf("\r\n6 Starting MiWi(TM) LSI-CWSN Stack ...");
    Printf("\r\n7 Starting MiWi(TM) LSI-CWSN Stack ...");
    Printf("\r\n8 Starting MiWi(TM) LSI-CWSN Stack ...");

    //ConsolePut('a');
    //ConsolePut('a');
    //ConsolePut('a');
    //ConsolePut('b');
    //ConsolePut('c');
    Printf("\r\nesto va ");

} */
  // while(!USB_Console_Tasks());
    
   // Printf("\r\n PASO POR AQUI ...");
   // Printf("\r\n PASO POR AQUI TB...");
    //SWDelay(20000);

//STAGE_TEMP:
//    Printf("\r\nDurmiendo Nodo... ");
//    //SleepNode(MIWI_0434, 5000);
//    //SleepNode(NONE, 5000);
//    SleepNode((MIWI_0434 | MIWI_2400), 5000);
//    Printf("YA!");
//    while(OSCCONbits.SLOCK == 0);
//    //SWDelay(2000);
 //   goto STAGE07;


    #if defined NODE_NOP
        while (1){      //Bucle infinito. Mantiene las pilas y no hace nada mas.
            #if defined NOP_JOINS_AND_LEAVES_LOOP
                SWDelay(6000);
                #if defined MIWI_0434_RI
                    Printf("\r\nSearching for PANs and establishing connections "
                           "in 434 MHZ.");
                    if(MiApp_SearchConnection(9, 0xFFFFFFFF, MIWI_0434)){
                        MiApp_EstablishConnection(0, 0, MIWI_0434);
                    }
                    SWDelay(6000);
                    DumpConnection(0xFF);
                #endif
//                #if defined MIWI_2400_RI
//                    Printf("\r\nSearching for PANs and establishing connections "
//                           "in 2,4 GHZ.");
//                    if(MiApp_SearchConnection(9, 0xFFFFFFFF, MIWI_2400)){
//                        MiApp_EstablishConnection(0, 0, MIWI_2400);
//                    }
//                    SWDelay(6000);
//                    DumpConnection(0xFF);
//                #endif
//                #if defined MIWI_0434_RI
//                    Printf("\r\nRemoving 434 MHz interface - connection slot 0.");
//                    MiApp_RemoveConnection(0, MIWI_0434);
//                    SWDelay(6000);
//                    DumpConnection(0xFF);
//                #endif
//                #if defined MIWI_2400_RI
//                    Printf("\r\nRemoving 2,4 GHz interface - connection slot 0.");
//                    MiApp_RemoveConnection(0, MIWI_2400);
//                    SWDelay(6000);
//                    DumpConnection(0xFF);
//                #endif
            #elif defined NOP_DUMPS_CONN_TABLE
                SWDelay(5000);
                DumpConnection(0xFF);
            #endif
        }
    #endif

    BYTE SMS1[4] = "Hola";
    BYTE SMS2[7] = "Que tal";
    BYTE SMS3[4] = "Bien";
    BYTE SMS4[7] = "Secreto";
    BYTE *beatles = "When I find myself in times of trouble, Mother Mary comes to me,"
                    " speaking words of wisdom, let it be. And in my hour of darkness"
                    " she is standing right in front of me, speaking words of wisdom,"
                    " let it be.";
    BYTE *LoremIpsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                       "Praesent fermentum dolor vitae nunc dignissim at sollicitudin "
                       "enim ullamcorper. Pellentesque eu nulla ut enim varius aliquet"
                       " ut quis sapien. Aliquam sodales felis id nisi sagittis "
                       "vulputate volutpat.";

    BYTE TestAddress[8];
    BYTE memStoreTest[500];
    while(1){
        BYTE i, j, k, data;
        UINT16 delay;

    //------------------------------------------------------------------------//
    // Pruebas... USAR goto STAGExx para saltarse trozos de test.             //
    //------------------------------------------------------------------------//
    //goto RF_FUNCTIONS_MAXRATE;
    //goto TX_RX_MAXRATE;
    //goto SAVE_RESTORE;
    //goto POWER_DISSIPATION_TEST;
    goto DEMO_PFC;
    //goto STAGE05;

STAGE01:        //COMPROBACION DEL ESTADO INICIAL.

        Printf("\r\n\nPruebas de comprobacion del estado inicial.\r");
        i = GetStatusFlags();
        Printf("\r\nStatusFlags: ");
        PrintChar(i);
        i = GetFreeTXBufSpace(ri);   //Deberia ser igual al tama?o del BUFFER
        Printf("\r\nCapacidad libre del buffer de TX de MiWi: ");
        PrintDec(i);
        i = GetPayloadToRead(ri);    //Deberia ser 0
        Printf("\r\nDatos pendientes de leer de MiWi: ");
        PrintDec(i);
        
        SWDelay(1000);

STAGE02:        //DORMIR Y DESPERTAR INTERFACES Y NODO.

        Printf("\r\n\nPruebas de dormir y despertar interfaces o el nodo completo.\r");
        Printf("\r\nDuerme, MiWi!");
        i = SleepRadioInterface(ri);
        Printf(" Valor de retorno: ");
        PrintChar(i);
        i = GetStatusFlags();
        Printf("\r\n(Comprobacion) StatusFlags: ");
        PrintChar(i);
        Printf("\r\nIntenta dormir MiWi cuando ya esta dormido.");
        i = SleepRadioInterface(ri);
        Printf(" Valor de retorno: ");
        PrintChar(i);
        i = GetStatusFlags();
        Printf("\r\n(Comprobacion) StatusFlags: ");
        PrintChar(i);
        Printf("\r\nDespierta, MiWi!");
        i = WakeUpRadioInterface(ri);
        Printf(" Valor de retorno: ");
        PrintChar(i);
        i = GetStatusFlags();
        Printf("\r\n(Comprobacion) StatusFlags: ");
        PrintChar(i);
        Printf("\r\nDuerme, mi amado nodo cognitivo!");
        i = SleepNode(NONE, 16000);
        Printf(" Valor de retorno: ");
        PrintChar(i);
        i = GetStatusFlags();
        Printf("\r\n(Comprobacion) StatusFlags: ");
        PrintChar(i);

        SWDelay(1000);

STAGE03:        //CAMBIOS DE CANAL Y GETTER DEL CANAL

        Printf("\r\n\nPruebas de canal inicial y cambios de canal.\r");
        i = GetOpChannel(ri);    //ri: Canal tras escaneo
        Printf("\r\n(Inicializacion) Canal de MiWi: ");
        PrintDec(i);
        Printf("\r\nIntenta cambiar a un canal no permitido.");
        i = SetChannel(ri, 20);      //Intenta poner canal no permitido.
        Printf(" Valor de retorno: ");
        PrintChar(i);
        i = GetOpChannel(ri);    //Sigue canal anterior
        Printf("\r\n(Comprobacion) Canal de MiWi: ");
        PrintDec(i);
        Printf("\r\nCambio a un canal permitido (canal 1).");
        i = SetChannel(ri, 1);       //Pone un canal permitido.
        Printf(" Valor de retorno: ");
        PrintChar(i);
        i = GetOpChannel(ri);
        Printf("\r\n(Comprobacion) Canal de MiWi: ");
        PrintDec(i);

        SWDelay(1000);

    //goto STAGE05;

STAGE04:        //REESCANEO DE CANALES.

        Printf("\r\n\nPruebas de reescaneo de canales.\rValores guardados:");
        for(i=0; i<NumChannels; i++){
            Printf("\r\nCanal: ");
            PrintDec(i);
            Printf("    RSSI: ");
            j= GetScanResult(ri, i, &k);
            (j) ? Printf("Error"): PrintChar(k);
        }
        //Reescaneo
        i = DoChannelScanning(ri, &k);
        Printf("\r\n\nCanal con menor ruido: ");
        PrintDec(i);
        Printf("   RSSI: ");
        PrintChar(k);

        for(i=0; i<NumChannels; i++){         
            Printf("\r\nCanal: ");
            PrintDec(i);
            Printf("    RSSI: ");
            j = GetScanResult(ri, i, &k);
            (j) ? Printf("Error"): PrintChar(k);
        }

        i = GetStatusFlags();
        Printf("\r\n(Comprobacion) StatusFlags: ");
        PrintChar(i);

        SWDelay(1000);

        Printf("\r\nEscaneo de todas las interfaces:\r");
        i = DoChannelScanning(ALL_MIWI, &k);
        if(k == MIWI_0434_RI_MASK){
            Printf("\r\nMiWi a 434 MHz es la interfaz con el canal menos ruidoso."
                   " Canal: ");
            PrintDec(i);
            Printf("   RSSI: ");
            GetScanResult(MIWI_0434, i, &k);
            PrintChar(k);
        }else if(k == MIWI_0868_RI_MASK){
            Printf("\r\nMiWi a 868 MHz es la interfaz con el canal menos ruidoso."
                   " Canal: ");
            PrintDec(i);
            Printf("   RSSI: ");
            GetScanResult(MIWI_0868, i, &k);
            PrintChar(k);
        }else{
            Printf("\r\nMiWi a 2,4 GHz es la interfaz con el canal menos ruidoso."
                   " Canal: ");
            PrintDec(i);
            Printf("   RSSI: ");
            GetScanResult(MIWI_2400, i, &k);
            PrintChar(k);
        }
        goto FINAL_STAGE;

STAGE05:    //BUFFERS DE TX Y RX. ENVIO, RECEPCION Y COMPROBACION DE UN PAQUETE BROADCAST.

        #if defined NODE_1
            Printf("\r\n\nPruebas de buffer de TX y rx. Envio broadcast de paquetes.\r");
            Printf("\r\nIntenta leer un dato recibido inexistente...");
            i = GetRXData(ri, &j);
            if (i == NO_ERROR){
                Printf("\r\nError: ha interpretado que hay un dato.");
            }
            Printf(" Valor de retorno: ");
            PrintChar(i);

            i = GetPayloadToRead(ri);
            Printf("\r\n(Comprobacion) Datos pendientes de leer de MiWi:");
            PrintDec(i);

            Printf("\r\nLlenamos el buffer de TX y enviamos un paquete broadcast.");
            i = 0;
            while(GetFreeTXBufSpace(ri) > 0){     //Hasta llenar el buffer de transmision
                //j = PutTXData(ri, 0x00);//For checking MiWi Fix
                j = PutTXData(ri, beatles[i]);
                if (j != NO_ERROR){
                    Printf("\r\nFallo al escribir en el buffer: ");
                    PrintChar(j);
                }
                else
                    i++;
            }
            Printf("\r\nSe escribieron en el buffer ");
            PrintDec(i);
            Printf(" caracteres.\rAhora que esta lleno, intento meter otro dato.");
            j = PutTXData(ri, i);
            Printf(" PutTXData devuelve: ");
            PrintChar(j);

            i = GetStatusFlags();
            Printf("\r\n(Comprobacion) StatusFlags: ");
            PrintChar(i);

            Printf("\r\nEnvio broadcast de lo que se ha escrito.");
            while(TRUE){
                //Enviar paquete con los datos que haya en el buffer de la interfaz
                i = SendPckt(ri, BROADCAST_ADDRMODE, NULL);
                Printf("\r\nMiMAC_SendPacket devuelve: ");
                PrintChar(i);
                if (i != NO_ERROR){
                    Printf(" => FALLO");
                    i = GetFreeTXBufSpace(ri);
                    Printf("\r\nCapacidad libre del buffer de TX: ");
                    PrintDec(i);
                }
                else{
                    Printf(" => OK");
                    //Al enviarlo, se inicializa de nuevo el buffer de TX y queda
                    //preparado para la siguiente transmisi?n.
                    i = GetFreeTXBufSpace(ri);   //Deberia ser igual al tama?o del BUFFER
                    Printf("\r\nLa capacidad del buffer es: ");
                    PrintDec(i);
                    break;  //TRANSMISION CORRECTA, SALE DEL BUCLE
                }
            }
            i = GetStatusFlags();
            Printf("\r\n(Comprobacion) StatusFlags: ");
            PrintChar(i);

            SWDelay(2000);

        #elif defined NODE_2
            Printf("\r\n\nPrueba de recepcion de un paquete broadcast.\r");
            while (TRUE){
                i = WhichRIHasData();

                if (i & ri_RI_MASK){
                    DumpRXPckt(ri);

                    j = GetPayloadToRead(ri);
                    Printf("\r\nPaquete recibido.\rPayload (bytes): ");
                    PrintDec(j);
                    i = GetRSSI(ri, &j);
                    Printf("    RSSI: ");
                    (i) ? Printf("Error"): PrintChar(j);
                    i = GetLQI(ri, &j);
                    Printf("    LQI: ");
                    (i) ? Printf("Error"): PrintChar(j);
                    i = GetRXSourceAddr(ri, TestAddress);
                    PrintTestAddress(i, TestAddress);

                    Printf("\r\nEl mensaje dice lo siguiente:\r");
                    while(GetPayloadToRead(ri) > 0){
                        GetRXData(ri, &data);
                        ConsolePut(data);
                    }
                    break;
                }
            }
            SWDelay(500);

        #endif
        goto STAGE05;        //Pruebas
        //goto FINAL_STAGE;

STAGE06:    //TX Y RX DE PAQUETES BROADCAST CON SEGURIDAD HABILITADA.

        #if defined NODE_1
            Printf("\r\n\nEnvio Broadcast con seguridad habilitada.");
            i = GetFreeTXBufSpace(ri);
            Printf("\r\nCapacidad libre del buffer de TX: ");
            PrintDec(i);
            Printf("\r\nComprobamos el flag de seguridad en StatusFlags: ");
            i = GetStatusFlags();
            PrintChar(i);
            Printf("\r\nActivo el flag de seguridad. SetSecurityLevel devuelve: ");
            i = SetSecurityLevel(1);
            PrintChar(i);
            Printf("\r\nComprobamos el flag de seguridad en StatusFlags: ");
            i = GetStatusFlags();
            PrintChar(i);

            i = 0;
            while(i < sizeof(SMS4)){
                j = PutTXData(ri, SMS4[i]);
                if (j){
                    Printf("\r\nFallo al escribir en el buffer. Codigo de error: ");
                    PrintChar(j);
                }else{
                    i++;
                }
            }
            Printf("\r\nEnvio broadcast. Se escribieron en el buffer ");
            PrintDec(i);
            Printf(" caracteres.");
            while(TRUE){
                //Enviar paquete con los datos que haya en el buffer de la interfaz
                i = SendPckt(ri, BROADCAST_ADDRMODE, NULL);
                Printf("\r\nMiMAC_SendPacket devuelve: ");
                PrintChar(i);
                if (i==0){
                    Printf(" => OK");
                    break;  //TRANSMISI?N CORRECTA, SALE DEL BUCLE
                }
                else{
                    Printf(" => FALLO");
                    //SWDelay(500);
                }
            }
            Printf("\r\nDesactivo el flag de seguridad. Valor de retorno: ");
            i= SetSecurityLevel(0);
            PrintChar(i);
            Printf("\r\n(Comprobacion) StatusFlags: ");
            i = GetStatusFlags();
            PrintChar(i);

            SWDelay(2000);


        #elif defined NODE_2
            Printf("\r\n\nRecepcion de un paquete broadcast con seguridad.\r");
            while (TRUE){
                i = WhichRIHasData();

                if (i & ri_RI_MASK){
                    DumpRXPckt(ri);
                    Printf("\r\nEl mensaje dice lo siguiente:\r");
                    while(GetPayloadToRead(ri) >0){
                        GetRXData(ri, &data);
                        ConsolePut(data);
                    }
                    break;
                }
            }
            SWDelay(500);
        #endif

        goto STAGE08;
        
STAGE07:    //PRUEBAS DE POTENCIA DE TRANSMISION Y ALCANCE EN RECEPCION.

        #if defined NODE_1
            Printf("\r\n\nAjuste de la potencia de transmision. Pruebas de alcance.");
            //Rampa de bajada
            j = 0x00;
            do{
                i = SetTXPower(ri, j);
                Printf("    SetTXPower devuelve: ");
                PrintChar(i);
                i = 0;
                while(i < sizeof(SMS1)){
                    k = PutTXData(ri, SMS1[i]);
                    if (k){
                        Printf("\r\nFallo al escribir en el buffer. Codigo de error: ");
                        PrintChar(k);
                    }else{
                        i++;
                    }
                }
                while(PutTXData(ri, j) != 0);   //Escribo el valor de la potencia de TX.
                i++;
                Printf("\r\nEnvio broadcast. Se escribieron en el buffer ");
                PrintDec(i);
                Printf(" caracteres.");
                //Enviar paquete con los datos que haya en el buffer de la interfaz.
                while (TRUE){
                    i = SendPckt(ri, BROADCAST_ADDRMODE, NULL);
                    Printf("\r\nMiMAC_SendPacket devuelve: ");
                    PrintChar(i);
                    if (i==0){
                        Printf(" => OK");
                        j += powerStep;
                        break;
                    }
                    else{
                        Printf(" => FALLO");
                    }
                }
                SWDelay(1000);
            }while (j != 0);    // Last step (0xF8 or 0xE0) + powerStep = 0 (Ramp end)

            SWDelay(2000);

            //Rampa de subida
            //j = 0xFF - (powerStep - 1);
            j = 0;
            do{
                j -= powerStep;
                i = SetTXPower(ri, j);
                Printf("    SetTXPower devuelve: ");
                PrintChar(i);
                while(i < sizeof(SMS1)){
                    k = PutTXData(ri, SMS1[i]);
                    if (k){
                        Printf("\r\nFallo al escribir en el buffer. Codigo de error: ");
                        PrintChar(k);
                    }else{
                        i++;
                    }
                }
                //Me aseguro de que escribe el valor de la potencia de transmision.
                while(PutTXData(ri, j) != 0);
                i++;
                Printf("\r\nEnvio broadcast. Se escribieron en el buffer ");
                PrintDec(i);
                Printf(" caracteres.");
                //Enviar paquete con los datos que haya en el buffer de la interfaz
                while (TRUE){
                    i = SendPckt(ri, BROADCAST_ADDRMODE, NULL);
                    Printf("\r\nMiMAC_SendPacket devuelve: ");
                    PrintChar(i);
                    if (i==0){
                        Printf(" => OK");
                        break;
                    }
                    else{
                        Printf(" => FALLO");
                    }
                }
                SWDelay(1000);
            }while(j != 0);     // Last step (0x08 or 0x20) - powerStep = 0 (Ramp end)

            SWDelay(10000);

        #elif defined NODE_2
            Printf("\r\n\nRecepcion con distintas potencias. Pruebas de alcance.\r"
                   "Numero de paquetes que se esperan recibir: ");
            k = 512/powerStep;      //k paquetes que envian las rampas de potencia
            PrintDec(k);
            WORD TimeOut = 7500;    //Timeout (aprox.) en decenas de milisegundo
            while(TRUE){
                i = WhichRIHasData();

                if (i & ri_RI_MASK){
                    j = GetPayloadToRead(ri);
                    k--;
                    Printf("\r\nPaquete recibido.\rPayload (bytes): ");
                    PrintDec(j);
                    i = GetRSSI(ri, &j);
                    Printf("    RSSI: ");
                    (i) ? Printf("Error"): PrintChar(j);
                    i = GetLQI(ri, &j);
                    Printf("    LQI: ");
                    (i) ? Printf("Error"): PrintChar(j);
                    i = GetRXSourceAddr(ri, TestAddress);
                    PrintTestAddress(i, TestAddress);

                    Printf("\r\nEl mensaje dice lo siguiente:\r");
                    while(GetPayloadToRead(ri) >1){
                        GetRXData(ri, &data);
                        ConsolePut(data);
                    }
                    //Ultimo dato (potencia transmitida).
                    GetRXData(ri, &data);
                    PrintChar(data);
                }
                if (k == 0){
                    break;
                }
                SWDelay(10);
                if(--TimeOut == 0){
                    Printf("\r\nNo se recibieron todos los paquetes esperados en el "
                           "tiempo prefijado. Se aborta la recepcion.");
                    break;
                }
            }
            SWDelay(500);
        #endif

STAGE08:

        #if defined NODE_1
            Printf("\r\n\nEnvio Unicast usando la direccion larga predefinida del "
                   "nodo 2.");
            TestAddress[0] = 0x00;
            TestAddress[1] = 0x11;
            TestAddress[2] = 0x22;
            TestAddress[3] = 0x33;
            TestAddress[4] = 0x44;
            TestAddress[5] = 0x55;
            TestAddress[6] = 0x66;
            TestAddress[7] = 0x22;   //Distintivo de la direcci?n del nodo 2.

            i=0;
            while(i < sizeof(SMS2)){
                j = PutTXData(ri, SMS2[i]);
                if (j){
                    Printf("\r\nFallo al escribir en el buffer. Codigo de error: ");
                    PrintChar(j);
                }else{
                    i++;
                }
            }
            Printf("\r\nSe escribieron en el buffer ");
            PrintDec(i);
            Printf(" caracteres.\rMensaje: 'Que tal'\rDireccion destino: ");
            PrintTestAddress(LONG_MIWI_ADDRMODE, TestAddress);

            while(TRUE){
                //Enviar paquete con los datos que haya en el buffer de la interfaz
                i = SendPckt(ri, LONG_MIWI_ADDRMODE, TestAddress);
                Printf("\r\nMiMAC_SendPacket devuelve: ");
                PrintChar(i);
                if (i==0){
                    Printf(" => OK");
                    break;  //TRANSMISION CORRECTA, SALE DEL BUCLE
                }
                else{
                    Printf(" => FALLO");
                    //SWDelay(500);
                }
            }
            SWDelay(500);

            Printf("\r\n\nEsperando respuesta...");
            while (TRUE){
                i = WhichRIHasData();

                if (i & ri_RI_MASK){
                    DumpRXPckt(ri);
                    break;
                }
            }
            SWDelay(500);

        #elif defined NODE_2
            Printf("\r\n\nRecepcion de un paquete unicast.\r");
            while (TRUE){
                i = WhichRIHasData();

                if (i & ri_RI_MASK){
                    DumpRXPckt(ri);
                    i = GetPayloadToRead(ri);
                    if (i != sizeof(SMS2)){
                        Printf("\r\nEl mensaje no tiene la longitud esperada. Dice lo"
                               " siguiente: ");
                        while(GetPayloadToRead(ri) >0){
                            GetRXData(ri, &data);
                            ConsolePut(data);
                        }
                    }
                    else{
                        Printf("\r\nEl mensaje dice lo siguiente:\r");
                        j = 0;
                        k = i;                      //Numero de caracteres erroneos.
                        while(GetPayloadToRead(ri) >0){
                            GetRXData(ri, &data);
                            if(data == SMS2[j]){
                                k--;
                            }
                            ConsolePut(data);
                            j++;
                        }

                        if(k){
                            Printf("\r\nHay errores. Numero de caracteres incorrectos: ");
                            PrintDec(k);
                        }else{
                            Printf("\r\nMensaje correcto. Envio la respuesta.\r");

                            SWDelay(1500);
                            TestAddress[0] = 0x00;
                            TestAddress[1] = 0x11;
                            TestAddress[2] = 0x22;
                            TestAddress[3] = 0x33;
                            TestAddress[4] = 0x44;
                            TestAddress[5] = 0x55;
                            TestAddress[6] = 0x66;
                            TestAddress[7] = 0x11;  //Distintivo de la direcci?n del nodo 1.

                            i=0;
                            while(i < sizeof(SMS3)){
                                j = PutTXData(ri, SMS3[i]);
                                if (j){
                                    Printf("\r\nFallo al escribir en el buffer. Codigo de error: ");
                                    PrintChar(j);
                                }else{
                                    i++;
                                }
                            }
                            Printf("\r\nEnvio unicast. Se escribieron en el buffer ");
                            PrintDec(i);
                            Printf(" caracteres.\rMensaje: 'Bien'\rDireccion destino: ");
                            PrintTestAddress(LONG_MIWI_ADDRMODE, TestAddress);

                            while(TRUE){
                                //Enviar paquete con los datos que haya en el buffer de la interfaz
                                i = SendPckt(ri, LONG_MIWI_ADDRMODE, TestAddress);
                                Printf("\r\nMiMAC_SendPacket devuelve: ");
                                PrintChar(i);
                                if (i==0){
                                    Printf(" => OK");
                                    break;  //TRANSMISI?N CORRECTA, SALE DEL BUCLE
                                }
                                else{
                                    Printf(" => FALLO");
                                    //SWDelay(500);
                                }
                            }
                            SWDelay(500);
                        }
                    }
                    break;
                }
            }
            SWDelay(1000);
        #endif

        goto FINAL_STAGE;

STAGE09:

        j = SaveConnTable(memStoreTest);
        j *= MIWI_CONN_ENTRY_SIZE;
        for (i=0; i<j; i++){
            PrintChar(memStoreTest[i]);
        }


FINAL_STAGE:
        //goto...

        DumpConnTable();
        Printf("\r\n\nFIN DEL TESTBENCH.");
        while (TRUE);           //Enable to check if indirect messages expire
        break;


TX_RX_MAXRATE:
        #if defined NODE_1
            TestAddress[0] = 0x00;
            TestAddress[1] = 0x11;
            TestAddress[2] = 0x22;
            TestAddress[3] = 0x33;
            TestAddress[4] = 0x44;
            TestAddress[5] = 0x55;
            TestAddress[6] = 0x66;
            TestAddress[7] = 0x22;   //Distintivo de la direcci?n del nodo 2.
//            TestAddress[0] = 0x00;
//            TestAddress[1] = 0x00;
        while(1){
            i = 255;
            Printf("\r\nRafaga de paquetes...\rTiempo inicial y final:\r");
            Print32Dec(ReadCoreTimer());
            ConsolePut('\r');
            while(i-- > 1){
                while(GetFreeTXBufSpace(ri)){
                    PutTXData(ri, i);
                }
                //SendPckt(ri, BROADCAST_ADDRMODE, NULL);
                SendPckt(ri, LONG_MIWI_ADDRMODE, TestAddress);
                //SendPckt(ri, SHORT_MIWI_ADDRMODE, TestAddress);
            }
            Print32Dec(ReadCoreTimer());
            Printf("\r\nPaquetes exitosos: ");
            Print32Dec(GetSentPckts(ri));
            SWDelay(5000);
        }
        #elif defined NODE_2
        i = 0;
        Printf("\r\nRafaga de paquetes...\rTiempo inicial y final:\r");
        while(1){
            if (WhichRIHasData() & ri_RI_MASK){
                while(GetPayloadToRead(ri) > 0){
                    GetRXData(ri, &(memStoreTest[i]));
                }
                i++;
            }
               
            if (i >= 254){
                Printf("\r\nFoo!\r");
                break;
            }
               
        }

        for(i=0; i < 255; i++){
            PrintChar(memStoreTest[i]);
            PrintChar(i);
            ConsolePut('\r');
        }
        Printf("\r\nCOJONES YA!");
        while(1){
            while(GetPayloadToRead(ri) > 0){
                GetRXData(ri, &(memStoreTest[i]));
            }
        }
        #endif
        break;

RF_FUNCTIONS_MAXRATE:
                        
        i = 200;
        Printf("\r\nCambio de canal. Tiempo inicial y final:\r");
        Print32Dec(ReadCoreTimer());
        ConsolePut('\r');
        while(i-- > 0){
            SetChannel(ri, 1);
        }
        Print32Dec(ReadCoreTimer());
        SWDelay(2000);

        i = 200;
        Printf("\r\nCambio de potencia de transmision. Tiempo inicial y final:\r");
        Print32Dec(ReadCoreTimer());
        ConsolePut('\r');
        while(i-- > 0){
            SetTXPower(ri, i);
        }
        Print32Dec(ReadCoreTimer());
        SWDelay(2000);

        i = 200;
        Printf("\r\nDormir/Despertar interfaz. Tiempo inicial y final:\r");
        Print32Dec(ReadCoreTimer());
        ConsolePut('\r');
        while(i-- > 0){
            SleepRadioInterface(ri);
            WakeUpRadioInterface(ri);
        }
        Print32Dec(ReadCoreTimer());
        SWDelay(2000);

        break;

SAVE_RESTORE:

        Printf("\r\nGuardando la tabla de conexiones.\r");
        i = SaveConnTable(memStoreTest);
        ConsolePut('\r');
        i = 0;
        while(i < MIWI_CONN_ENTRY_SIZE){
            PrintChar(memStoreTest[i]);
            ConsolePut(' ');
            i++;
        }
        ConsolePut('\r');

        //Restaurar la tabla con una �nica conexi�n con todos los campos a 0xFF.
        for (i=0;i<MIWI_CONN_ENTRY_SIZE; i++){
            memStoreTest[i] = 0xFF;
        }
        Printf("\r\nRestaurando la tabla de conexiones.\r");
        RestoreConnTable(memStoreTest, 1);
        DumpConnTable();

        Printf("\r\nGuardando la tabla de rutado.\r");
        SaveRoutingTable(ALL_MIWI,memStoreTest);
        i = 0;
        while(i < 34){
            PrintChar(memStoreTest[i]);
            ConsolePut(' ');
            i++;
        }

        //Restaurar la tabla con una �nica todos los campos a 0x04.
        i = 0;
        while(i < 34){
            memStoreTest[i]= 0x04;
            i++;
        }
        Printf("\r\nRestaurando la tabla de conexiones.\r");
        RestoreRoutingTable(ALL_MIWI,memStoreTest);
        DumpConnTable();
        while (1);


POWER_DISSIPATION_TEST:
        while(1){
            DelayMs(3000);
            Printf("\r\nDurmiendo 868\r");
            SleepRadioInterface(MIWI_0868);
            DelayMs(8000);
            Printf("\r\nDurmiendo 2,4\r");
            SleepRadioInterface(MIWI_2400);
            DelayMs(8000);
            Printf("\r\nDurmiendo nodo\r");
            SleepNode(NONE, 10000);
            Printf("\r\nDespertando\r");
            //WakeUpRadioInterface(ALL);
            SetTXPower(ri,0);
            DelayMs(8000);
            Printf("\r\nTransmitiendo\r");
            i=20;
            while(i>0){
                while(PutTXData(ri,i));
                while(SendPckt(ri, BROADCAST_ADDRMODE, NULL));
                i--;
            }
            DelayMs(3000);
            Printf("\r\nReposo\r\n");
        }

DEMO_PFC:
        #if defined NODE_1
            delay = 6400;
            TestAddress[0] = 0x00;
            TestAddress[1] = 0x11;
            TestAddress[2] = 0x22;
            TestAddress[3] = 0x33;
            TestAddress[4] = 0x44;
            TestAddress[5] = 0x55;
            TestAddress[6] = 0x66;
            TestAddress[7] = 0x22;   //Distintivo de la direcci?n del nodo 2.
            j = 0;
            i = 0;
            DiscardTXData(ri);
            while(j < 4){
                for(k=0; k<8; k++){
                    PutTXData(ri, LoremIpsum[i]);
                    i++;
                }

                while(SendPckt(ri, LONG_MIWI_ADDRMODE, TestAddress));

                if(i == 0){
                    j++;
                }
                SWDelay(delay);
                if(delay > 3200){
                    delay -= 800;
                }else if(delay > 400){
                    delay -= 400;
                }else if(delay >100){
                    delay -= 100;
                }else if(delay > 20){
                    delay -= 20;
                }else if(delay > 1){
                    delay -= 1;
                }else{
                    delay = 6400;
                }


            }
        #elif defined NODE_2
            while(1){
                if(WhichRIHasData() & ri_RI_MASK){
                    while(GetPayloadToRead(ri)>0){
                        GetRXData(ri, &i);
                        ConsolePut(i);
                    }
                }
            }
        #endif

    }
    return 0;
}

// Exception handler:
static enum {
    EXCEP_IRQ = 0,                  // interrupt
    EXCEP_AdEL = 4,                 // address error exception (load or ifetch)
    EXCEP_AdES,                     // address error exception (store)
    EXCEP_IBE,                      // bus error (ifetch)
    EXCEP_DBE,                      // bus error (load/store)
    EXCEP_Sys,                      // syscall
    EXCEP_Bp,                       // breakpoint
    EXCEP_RI,                       // reserved instruction
    EXCEP_CpU,                      // coprocessor unusable
    EXCEP_Overflow,                 // arithmetic overflow
    EXCEP_Trap,                     // trap (possible divide by zero)
    EXCEP_IS1 = 16,                 // implementation specfic 1
    EXCEP_CEU,                      // CorExtend Unuseable
    EXCEP_C2E                       // coprocessor 2
} _excep_code;

static unsigned int _epc_code;
static unsigned int _excep_addr;

// this function overrides the normal _weak_ generic handler
void _general_exception_handler(void){
    Printf("\r\n\nGeneral Exception Handler...\r\r");

    asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
    asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));

    _excep_code = (_excep_code & 0x0000007C) >> 2;
    while (1) {
        // Examine _excep_code to identify the type of exception
        // Examine _excep_addr to find the address that caused the exception
        Nop();
        Nop();
        Nop();
    }
}//End of exception handler
