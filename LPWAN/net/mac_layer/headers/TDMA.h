/*
 * TDMA.h
 *
 /*
 * TDMA.h
 *
 *  Created on: 21 jul. 2023
 *      Author: User
 */

#ifndef APPLICATION_INC_TDMA_H_
#define APPLICATION_INC_TDMA_H_

#include "../../headers/bsp.h"
#include "message.h"


#define MAX_DEVICES 5 // Number of devices for the net
#define SLOT_TIME_MS 31 // Time per slot

#define CYCLES_TO_CHANGE_SLOT 4 // Number of cycles to change the slot
#define SYNC_REPEAT 4 //  Number of cycles before requesting a new sync

static new_node_flag = FALSE;

typedef unsigned char uint8_t;
typedef enum {
    IDLE,
    TX,
    RX
} DeviceState;


// This structure represents the role of the device
typedef enum {
    NON_ASSIGNED,
    MAIN,
    NODE,
    BEACON,
} DeviceRole;

// This structure represents the transciever steps
typedef enum {
    SEND_ACK,
    SYNC_RECEIVED,
    PENDING_ACK,
    BEACON_ACTIVE,
    CONTINUOUS_MODE,
} DeviceFlow;

// This structure represents if the device is a master or a node
typedef enum {
    PENDING_SLOT,
    SLOT_ASSIGNED,
} SlotState;



// This structure represents a device
typedef struct {
    unsigned int deviceID; // ID
    unsigned int slot;     // Slot
    DeviceState state; //State
    DeviceRole role; // Role
    int rssi; // RSSI
} DeviceInfo;



typedef struct {
    MessageType type;       // Type of the message
    uint8_t deviceID;    // ID of the transmitting device
    uint8_t deviceSLOT;  // SLOT of the transmitting device
    uint8_t currentSLOT; // SLOT en el que se encuentra cuando envía el beacon
    U16 timestamp; //Timestamp of the transmitting device
    uint8_t payload[8];  // Payload for Beacon message (8 bytes)
} BeaconMessage;

typedef struct {
    MessageType type;       // Type of the message
    uint8_t deviceID;    // ID of the transmitting device
    uint8_t payload[16]; // Payload for Sync message (16 bytes)
} SyncMessage;

typedef struct {
    MessageType type;       // Type of the message
    uint8_t deviceID;    // ID of the transmitting device
    uint8_t nodeSLOT;
    uint8_t payload[4];  // Payload for ACK message (4 bytes)
} ACKMessage;


// Initialization of communication
state init_mac_layer();


//TDMA Functions
void addDevice(int deviceID, int slot, DeviceState state, DeviceRole role, int rssi);
unsigned int getDeviceIDBySlot(unsigned int slot);
int isDeviceIDInArray(unsigned int deviceID);
void initializeDeviceArray();
uint8_t getCurrentDeviceID();
void updateCount(U16 time);
unsigned int assignRandomSlot( unsigned int num_slots);
void tdma_handler();
void set_id(unsigned char ID);
void sendBeacon();
BIT  StringCompare(U8* pbiPacketContent, U8* pbiString, U8 lenght);
void sendStandard();
void process_beacon_message();
void process_sync_message(U8 rout);
void process_ack_message();
void process_standard_message();
void sendSync();
void sendACK(U8 rout);
void removeDevice(unsigned int deviceID, DeviceRole role);
state send_packet(unsigned char ID, unsigned char fateID, net_StandardMessage message);
state send_broadcast(routTable table[SIZE_ROUT]);
net_StandardMessage recieve_packet();
BIT gSampleCode_StringCompare(U8* pbiPacketContent, U8* pbiString, U8 lenght);
#endif /* APPLICATION_INC_TDMA_H_ */
