/*******************************************************************************
* File:   DevNAT.c
* Author: Juan Domingo Rebollo - Laboratorio de Sistemas Integrados (LSI) - UPM
*
* File Description: 
*   Device Network Address Translation (DevNAT) provides a set of functions and
*   variables to manage single stack addresses as a unique device address.
******************************************************************************/
#include "NodeHAL.h"
#include "WirelessProtocols/ConfigApp.h"
#include "HardwareProfile.h"

/* DEFINITIONS ****************************************************************/
#define UNREG_ADDR_MASK                 0x4000
//#define NEW_ERASABLE_REG_MASK         0xC000  //Register using DevAddr
#define REG_ERASABLE_EUI_MASK           0xC100  //Register using permMIWIAddr
#define REG_VALID_MIWI_CONINDEX_MASK    0x8400
#define REG_VALID_MIWI_ALTADDR_MASK     0x8200
#define REG_VALID_MIWI_PERMADDR_MASK    0x8100
#define REG_VALID_WIFI_ADDR_MASK        0x8800

#if DEV_ADDR_TABLE_SIZE > 255                   //Defined in NodeHAL.h
    #undef DEV_ADDR_TABLE_SIZE
    #define DEV_ADDR_TABLE_SIZE 255     
    //BYTE size index - 1 position reserved (0xFF) for invalid index.
#endif

/*******************************************************************************
 * MiWi interface has to ways to unicast a message: giving a connection index or
 * an address. In P2P Protocol addresses are permanent, but in MIWI Protocol
 * addresses can be either alternative ("short", 16 bits) or permanent (64 bits)
 ******************************************************************************/
#if !defined PROTOCOL_P2P
    typedef struct{
        union{
            UINT16 Address;
            struct{ BYTE B0; BYTE B1; }AddrBytes;
        };
    }altMiwiAddr;       //Alternative or Short MIWI address.
#endif

typedef struct{
    union{
        UINT64 Address;
        struct{ BYTE B0; BYTE B1; BYTE B2; BYTE B3;
                BYTE B4; BYTE B5; BYTE B6; BYTE B7; }AddrBytes;
    };
}permMiwiAddr;          //EUI or permanent MIWI address.


#ifdef WIFI_2400_RI
    typedef struct{
        union{
            UINT32 Address;
            struct{ BYTE B0; BYTE B1; BYTE B2; BYTE B3; }AddrBytes;
        };
    }WifiAddr;          //¿IPv4 address?
#endif

/*******************************************************************************
 * Device Address Info (devAddrInfo) is a struct of several fields and flags
 * which inform about the registered node, its addresses, interfaces and the
 * interaction which this node.
 ******************************************************************************/
typedef struct{
    union{
        UINT16 Info;
        struct{
            //First Byte: Activity and device's known interfaces.
            BYTE DevUsesMIWI0434   : 1; //Bit 0 - This node has MiWi 0434 ri
            BYTE DevUsesMIWI0868   : 1; //Bit 1 - This node has MiWi 0868 ri
            BYTE DevUsesMIWI2400   : 1; //Bit 2 - This node has MiWi 2400 ri
            BYTE DevUsesWIFI2400   : 1; //Bit 3 - This node has WiFi 2400 ri
            BYTE Activity          : 4; //Bits 4-7: For erasing policy.

            //Second Byte: Valid addresses and register options.
            BYTE ValidPermMIWIAddr : 1; //Bit 8 - Stored permMIWIAddr validity.
          #ifndef PROTOCOL_P2P
            BYTE ValidAltMIWIAddr  : 1; //Bit 9 - Stored AltMIWIAddr validity.
          #else
            BYTE                   : 1;
          #endif
            BYTE ValidMIWIConIndex : 1; //Bit 10 - Stored MIWIConIndex validity.
          #ifdef WIFI_2400_RI
            BYTE ValidWIFIAddr     : 1; //Bit 11 - Stored WIFIAddr validity.
          #else
            BYTE                   : 1;
          #endif
            BYTE            : 1; //Bit 12: Reserved     - Future use.
            BYTE Banned     : 1; //Bit 13: Banned       - '1' = RX/TX disabled
            BYTE Erasable   : 1; //Bit 14: Erasable     - Rewrite/Erase enable
            BYTE Registered : 1; //Bit 15: Registered   - '0' if record is free
        }Bits;
    };
}devAddrInfo;

typedef struct{
    permMiwiAddr PermMIWIAddr[DEV_ADDR_TABLE_SIZE];

    #ifndef PROTOCOL_P2P
        altMiwiAddr AltMIWIAddr[DEV_ADDR_TABLE_SIZE];
    #endif

    BYTE MIWIConIndex[DEV_ADDR_TABLE_SIZE];

    #ifdef WIFI_2400_RI
        WifiAddr WIFIAddr[DEV_ADDR_TABLE_SIZE];
    #endif

    devAddrInfo DevAddrInfo[DEV_ADDR_TABLE_SIZE];
}devNAT_Table;

/* VARIBALES ******************************************************************/

/* Device Address Table contains [DEV_ADDR_TABLE_SIZE] records for storing info
 * and addresses of a node in the network                                     */
static devNAT_Table AddrTable;

static BYTE RegisteredNodes;        //Number of registered nodes.

/* The following variables aim to be like a cache of one position per interface.
 * As the communication with a node may involve exchanging several messages, the
 * NodeHAL stores the index for getting the last peer address. These variables 
 * are quite useful if the AddressTable is large, as they prevent the node from 
 * resolving the same address during the peer to peer communication. Conversely,
 * these varibles tend to be ineficient when the table size is small.*/
#ifdef MIWI_0434_RI
    BYTE MIWI0434LastIndex;
#endif
#ifdef MIWI_0868_RI
    BYTE MIWI0868LastIndex;
#endif
#ifdef MIWI_2400_RI
    BYTE MIWI2400LastIndex;
#endif
#ifdef WIFI_2400_RI
    BYTE WIFI2400LastIndex;
#endif

////////////////////////////////////////////////////////////////////////////////
/* DevNAT FUNCTIONS ***********************************************************/
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Function:    void InitDevAddrTable()
 * Description: Initialises the device address table. All records are set by
 *              default to unregistered erasable, not banned, contaning no valid
 *              network addresses and the node using no radio interfaces.
 * PreCondition:None
 * Parameters:  None
 * Returns:     None
 ******************************************************************************/
void InitDevAddrTable(){
    BYTE i;
    for (i=0; i<DEV_ADDR_TABLE_SIZE; i++){
        AddrTable.PermMIWIAddr[i].Address = 0;
        
        #if !defined PROTOCOL_P2P
            AddrTable.AltMIWIAddr[i].Address = 0;
        #endif

        AddrTable.MIWIConIndex[i] = 0;
        
        #if defined (WIFI_2400_RI)
            AddrTable.WIFIAddr[i].Address = 0;
        #endif

        AddrTable.DevAddrInfo[i].Info = UNREG_ADDR_MASK;
    }
    #if defined MIWI_0434_RI
        MIWI0434LastIndex = 0;
    #endif
    #if defined MIWI_0868_RI
        MIWI0868LastIndex = 0;
    #endif
    #if defined MIWI_2400_RI
        MIWI2400LastIndex = 0;
    #endif
    #if defined WIFI_2400_RI
        WIFI2400LastIndex = 0;
    #endif
    RegisteredNodes = 0;
}

/*******************************************************************************
 * Function:    BYTE GetIndexFromMIWIConIndex(BYTE MIWIConIndex)
 * Description: This function search in the devNAT table for a REGISTERED node
 *              with a VALID MiWi connection index which matches with the MiWi
 *              connection index given as a parameter.
 * PreCondition:devNAT Table has been initialised.
 * Parameters:  MIWI connection index which will be searched in the table.
 * Returns:     The node's register index if the search was successful or 0xFF
 *              (invalid index) if none of the registered nodes have the
 *              requested valid connection index.
 * Remarks:     Do not confuse MiWi connection indexes with devNAT node register
 *              indexes. The former are from MiWi connection table; the latter
 *              are used for reading and writing info in the devNAT table.
 ******************************************************************************/
BYTE GetIndexFromMIWIConIndex(BYTE MIWIConIndex){
    BYTE i;
    for(i=0; i<DEV_ADDR_TABLE_SIZE; i++){
        if (AddrTable.DevAddrInfo[i].Info & REG_VALID_MIWI_CONINDEX_MASK){
            if (MIWIConIndex == AddrTable.MIWIConIndex[i]){
                return i;
            }
        }
    }
    return 0xFF; //ConIndex not found. Return invalid index OxFF.
}

/*******************************************************************************
 * Function:    BYTE GetIndexFromPermMIWIAddr(UINT64 permMIWIAddr)
 * Description: This function search in the devNAT table for a REGISTERED node
 *              with a VALID MiWi permanent address which matches with the MiWi
 *              permanent address given as a parameter.
 * PreCondition:devNAT Table has been initialised.
 * Parameters:  MIWI permanent address which will be searched in the table.
 * Returns:     The node's register index if the search was successful or 0xFF
 *              (invalid index) if none of the registered nodes have the
 *              requested valid MiWi permanent address.
 ******************************************************************************/
BYTE GetIndexFromPermMIWIAddr(UINT64 permMIWIAddr){
    BYTE i;
    for(i=0; i<DEV_ADDR_TABLE_SIZE; i++){
        if (AddrTable.DevAddrInfo[i].Info & REG_VALID_MIWI_PERMADDR_MASK){
            if (permMIWIAddr == AddrTable.PermMIWIAddr[i].Address){
                return i;
            }
        }
    }
    return 0xFF; //Address not found. Return invalid index OxFF.
}

#ifndef PROTOCOL_P2P
/*******************************************************************************
 * Function:    BYTE GetIndexFromAltMIWIAddr(UINT16 altMIWIAddr)
 * Description: This function search in the devNAT table for a REGISTERED node
 *              with a VALID MiWi alternative address that matches with the MiWi
 *              alternative address given as a parameter.
 * PreCondition:devNAT Table has been initialised.
 * Parameters:  MIWI alternative address which will be searched in the table.
 * Returns:     The node's register index if the search was successful or 0xFF
 *              (invalid index) if none of the registered nodes have the
 *              requested valid MiWi alternative address.
 ******************************************************************************/
BYTE GetIndexFromAltMIWIAddr(UINT16 altMIWIAddr){
    BYTE i;
    for(i=0; i<DEV_ADDR_TABLE_SIZE; i++){
        if (AddrTable.DevAddrInfo[i].Info & REG_VALID_MIWI_ALTADDR_MASK){
            if (altMIWIAddr == AddrTable.AltMIWIAddr[i].Address){
                return i;
            }
        }
    }
    return 0xFF; //Address not found. Return invalid index OxFF.
}
#endif

#ifdef WIFI_2400_RI
/*******************************************************************************
 * Function:    BYTE GetIndexFromWIFIAddr(UINT32 WIFIAddr)
 * Description: This function search in the devNAT table for a REGISTERED node
 *              with a VALID WiFi address that matches with the WiFi address
 *              given as a parameter.
 * PreCondition:devNAT Table has been initialised.
 * Parameters:  WiFi address which will be searched in the table.
 * Returns:     The node's register index if the search was successful or 0xFF
 *              (invalid index) if none of the registered nodes have the
 *              requested valid WiFi address.
 ******************************************************************************/
BYTE GetIndexFromWIFIAddr(UINT32 WIFIAddr){
    BYTE i;
    for(i=0; i<DEV_ADDR_TABLE_SIZE; i++){
        if (AddrTable.DevAddrInfo[i].Info & REG_VALID_WIFI_ADDR_MASK){
            if (WIFIAddr == AddrTable.WIFIAddr[i].Address){
                return i;
            }
        }
    }
    return 0xFF; //Address not found. Return invalid index OxFF.
}
#endif

//¿Define DevAddr and use it to register a node or use permMIWIAddr instead? (!)
BYTE RegisterDevice (UINT64 permMIWIAddr){
    BYTE index;
    if (RegisteredNodes < DEV_ADDR_TABLE_SIZE){
        //Check for a free record to register the node.
        for (index=0; index<DEV_ADDR_TABLE_SIZE; index++){ 
            if (AddrTable.DevAddrInfo[index].Bits.Registered == 0){
                AddrTable.PermMIWIAddr[index].Address = permMIWIAddr;
                AddrTable.DevAddrInfo[index].Info = REG_ERASABLE_EUI_MASK;
                RegisteredNodes++;                                  
                return index;
            }
        }
    }
    return 0xFF; //Else: Number of registered nodes are equal to table capacity
}

/*******************************************************************************
 * Function:    BYTE ChooseRegisterToRemove()
 * Description: Invoke this function to know which is the most suitable register
 *              to be removed or rewritten due to its activity indicator. Then
 *              you may use the devNAT function RemoveRegisterFromTable to free
 *              the record in the table. The less activity that a node has, the
 *              more probably is that its register will be drawn out.
 * PreCondition:devNAT Table has been initialised.
 * Parameters:  None
 * Returns:     Index of the register whose activity is the lowest or an invalid
 *              index (0xFF) if there are no erasable registers to remove.
 * Remarks:     It doesn't care of the registered flag, so a registered node's
 *              index with zero activity may be returned. This function is aimed
 *              to be invoked when there are no free positions in the table,
 *              i.e. the number of registered nodes reaches the table capacity.
 ******************************************************************************/
BYTE ChooseRegisterToRemove(){
    BYTE i;
    BYTE index = 0xFF;          //Provisional invalid index
    BYTE activity = 0;          //Provisional zero activity indicator
    //Erasing policy: guess which removable register has less activity
    for (i=0; i<DEV_ADDR_TABLE_SIZE; i++){
        if(AddrTable.DevAddrInfo[i].Bits.Erasable == 1){    //Check if erasable
            if(AddrTable.DevAddrInfo[i].Bits.Activity < activity){
                activity = AddrTable.DevAddrInfo[i].Bits.Activity;
                index = i;  
                //If the activity indicator of this register is lower than the
                //minimum value found by now, this register is the new candidate
            }
        }
    }
    //Notice index could be still invalid if there are no erasable registers
    return index;   //Return the register index whose activity is the lowest.
}

/*******************************************************************************
 * Function:    BOOL RemoveRegisterFromTable(BYTE index)
 * Description: Delete the node info of a selected register using its index.
 * PreCondition:None
 * Parameters:  Index of the table record to be deleted
 * Returns:     A boolean which indicates if the operation was successful.
 ******************************************************************************/
BOOL RemoveRegisterFromTable(BYTE index){
    if(index < DEV_ADDR_TABLE_SIZE){
        AddrTable.DevAddrInfo[index].Info = UNREG_ADDR_MASK;
        //¿"Reset" all addresses in addition?
            AddrTable.MIWIConIndex[index] = 0;
            AddrTable.PermMIWIAddr[index].Address = 0;
            #ifndef PROTOCOL_P2P
                AddrTable.AltMIWIAddr[index].Address = 0;
            #endif
            #ifdef WIFI_2400_RI
                AddrTable.WIFIAddr[index].Address = 0;
            #endif
        return TRUE;
    }
    return FALSE;
}

/*******************************************************************************
 * Function:    BOOL StoreMIWIConIndex(BYTE index, BYTE thisMiwiConIndex)
 * Description: Store a MiWi connection index in a node record in devNAT table.
 * PreCondition:devNAT Table has been initialised.
 * Parameters:  index            - Index of the devNAT table (node record)
 *              thisMiwiConIndex - the MiWi connection index to be stored
 * Returns:     A boolean which indicates if the operation was successful.
 ******************************************************************************/
BOOL StoreMIWIConIndex(BYTE index, BYTE thisMiwiConIndex){
    if (index < DEV_ADDR_TABLE_SIZE){       //Also implies that index != 0xFF
        if (AddrTable.DevAddrInfo[index].Bits.Registered){
            AddrTable.MIWIConIndex[index] = thisMiwiConIndex;
            AddrTable.DevAddrInfo[index].Bits.ValidMIWIConIndex = 1;
            return TRUE;    //Success (MiWi connection index stored)
        }
        //Failure: The index corresponds to a unregistered node.
    }
    return FALSE;   //Failure: Index = 0xFF or exceeds table size.
}

#ifndef PROTOCOL_P2P
/*******************************************************************************
 * Function:    BOOL StoreAltMIWIAddr(BYTE index, UINT16 thisAltMiwiAddr)
 * Description: Store an alternative MiWi address in a node record in devNAT
 *              table.
 * PreCondition:devNAT Table has been initialised.
 * Parameters:  index           - Index of the devNAT table (node record)
 *              thisAltMiwiAddr - the alternative MiWi address to be stored
 * Returns:     A boolean which indicates if the operation was successful.
 ******************************************************************************/
BOOL StoreAltMIWIAddr(BYTE index, UINT16 thisAltMiwiAddr){
    if (index < DEV_ADDR_TABLE_SIZE){
        if (AddrTable.DevAddrInfo[index].Bits.Registered){
            AddrTable.AltMIWIAddr[index].Address = thisAltMiwiAddr;
            AddrTable.DevAddrInfo[index].Bits.ValidAltMIWIAddr = 1;
            return TRUE;    //Success (Alt. MiWi address stored)
        }
        //Failure: The index corresponds to a unregistered node.
    }
    return FALSE;   //Failure: Index = 0xFF or exceeds table size.
}
#endif

#ifdef WIFI_RI
/*******************************************************************************
 * Function:    BOOL StoreWIFIAddr(BYTE index, UINT32 thisWifiAddr)
 * Description: Store a WiFi address in a node record in devNAT table.
 * PreCondition:devNAT Table has been initialised.
 * Parameters:  index        - Index of the devNAT table (node record)
 *              thisWifiAddr - the WiFi address to be stored
 * Returns:     A boolean which indicates if the operation was successful.
 ******************************************************************************/
BOOL StoreWIFIAddr(BYTE index, UINT32 thisWifiAddr){
    if (index < DEV_ADDR_TABLE_SIZE){
        if (AddrTable.DevAddrInfo[index].Bits.Registered){
            AddrTable.WifiAddr[index].Address = thisWifiAddr;
            AddrTable.DevAddrInfo[index].Bits.ValidWifiAddr = 1;
            return TRUE;    //Success (WiFi address stored)
        }
        //Failure: The index corresponds to a unregistered node.
    }
    return FALSE;   //Failure: Index = 0xFF or exceeds table size.
}
#endif

UINT64 GetPermMIWIAddr(BYTE index){
    return 0;
}
UINT16 GetAltMIWIAddr(BYTE index){
    return 0;
}
BYTE GetMIWIConIndex(BYTE index){
    return 0;
}
UINT32 GetWIFIAddr(BYTE index){
    return 0;
}

//BOOL ResolveAddr (radioInterface ri, UINT64 devAddr, BYTE *storeItHere){
//    BYTE index;
//    BYTE length;
//    BYTE *p;
//    switch (ri) {
//        case TULIO:
//            #ifndef TULIO_RI
//                //Printf("\rError: Tulio is not available");
//                return FALSE;
//            #else
//                //Check if devAddr index is the Tulio last index.
//                if (devAddr == AddressTable.DevAddr[TULIOLastIndex]){   //Hit
//                    index = TULIOLastIndex;
//                    goto STORE_TULIO;
//                }
//                //Miss. Check if the node is registered in Device Address Table.
//                index = GetAddrTableIndex(devAddr);
//                if(index != 0xFF) {
//                    //Node registered. Check if the Tulio address stored is valid.
//                    if(AddressTable.DevAddrInfo[index].FlagBits.ValidTulioAddr){
//                        TULIOLastIndex = index;
//                        goto STORE_TULIO;
//                    }
//                }//Not registered or address is invalid. Unknown device address.
//                return FALSE;
//
//            STORE_TULIO:   //Now we have the index, store the requested address.
//                p = &AddressTable.TulioAddr[index].AddrBytes.B0;
//                //p points to Tulio Address. PIC32MX is little endian.
//                *storeItHere++ = *p++;  //LSB.
//                *storeItHere = *p;      //MSB.
//                return TRUE;
//            #endif
//        case MIWI:
//            #ifndef MIWI_RI
//                //Printf("\rError: MiWi is not available");
//                return FALSE;
//            #else
//                //Check if devAddr index is the MiWi last index.
//                if (devAddr == AddressTable.DevAddr[MIWILastIndex]){   //Hit
//                    index = MIWILastIndex;
//                    goto STORE_MIWI;
//                }
//                //Miss. Check if the node is registered in Device Address Table.
//                index = GetAddrTableIndex(devAddr);
//                if(index != 0xFF) {
//                    //Node registered. Check if the MiWi address stored is valid.
//                    if(AddressTable.DevAddrInfo[index].FlagBits.ValidMiwiAddr){
//                        MIWILastIndex = index;
//                        goto STORE_MIWI;
//                    }
//                }//Not registered or address is invalid. Unknown device address.
//                return FALSE;
//
//            STORE_MIWI:    //Now we have the index, store the requested address.
//                p = &AddressTable.MiwiAddr[index].AddrBytes.B0;
//                //p points to MiWi Address.
//
//                //Store MiWi Address Length first to indicate address type.
//                if(AddressTable.DevAddrInfo[index].FlagBits.MiwiPermAddr)
//                    length = 8;
//                else
//                    length = (*(p+7)) ? 2 : 1;  //Check MSB. See DevAddrInfo.
//                *storeItHere++ = length;        //Length stored in position [0]
//
//                //Store the Address. PIC32MX is little endian.
//                while (length > 0){
//                    *storeItHere++ = *p++;      //LSB first, in position [1]
//                    length--;
//                }
//                return TRUE;
//            #endif
//        case WIFI:
//            #ifndef WIFI_RI
//                //Printf("\rError: WiFi is not available");
//                return FALSE;
//            #else
//                //Check if devAddr index is the WiFi last index.
//                if (devAddr == AddressTable.DevAddr[WIFILastIndex]){   //Hit
//                    index = WIFILastIndex;
//                    goto STORE_WIFI;
//                }
//                //Miss. Check if the node is registered in Device Address Table.
//                index = GetAddrTableIndex(devAddr);
//                if(index != 0xFF) {
//                    //Node registered. Check if the WiFi address stored is valid.
//                    if(AddressTable.DevAddrInfo[index].FlagBits.ValidWifiAddr){
//                        WIFILastIndex = index;
//                        goto STORE_WIFI;
//                    }
//                }//Not registered or address is invalid. Unknown device address.
//                return FALSE;
//
//            STORE_WIFI: //Now we have the index, store the requested address.
//                p = &AddressTable.WifiAddr[index].AddrBytes.B0;  //p points to WiFi Address.
//                //Store the address. PIC32MX is little endian.
//                for (length=0; length<4; length++){
//                    *storeItHere++ = *p++;  //LSB first, in position [0]
//                }
//                return TRUE;
//            #endif
//        case ALL:
//            //Printf("\rError: You must choose a single radio interface.");
//            return FALSE;
//        case NONE:
//            //Printf("\rError: None of radio interfaces were selected.");
//            return FALSE;
//        default:
//            //Printf("\rError: Unknown radio interface.");
//            return FALSE;
//    }
//}
