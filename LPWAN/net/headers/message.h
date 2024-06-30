/*
 * message.h
 *
 *  Created on: 19 jun 2024
 *      Author: Sergio
 */

#ifndef NET_HEADERS_MESSAGE_H_
#define NET_HEADERS_MESSAGE_H_

#include "funcs.h"

#define SIZE_ROUT 5

typedef enum {
    BEACON_MSG,
    SYNC_MSG,
    ACK_MSG,
    STANDARD_MSG,
    BROADCAST_MSG,
    CLEAN,
} MessageType;


typedef struct {
  unsigned char sourceID;
  unsigned char fateID;
  unsigned char next_jump_ID;
  unsigned char steps;
}routTable;

typedef struct {
  routTable table[SIZE_ROUT];
  int rear;
  int front;
}Table;

typedef struct{

    unsigned char fateID;                 // ID del siguiente dispositivo al que saltar
    routTable table[SIZE_ROUT];

}network_layer_header_t;

typedef struct packet {
    MessageType type;       // Type of the message
    unsigned char deviceID;    // ID of the transmitting device
    unsigned char nodeID; // ID of the receiving device
    unsigned int timestamp; //Timestamp of the transmitting device
}mac_layer_header_t;


typedef struct {
  mac_layer_header_t mac_hdr;
  network_layer_header_t net_hdr;
}packet_header_t;

typedef struct {

  packet_header_t packet_hdr;
  unsigned char payload[8];  // Payload for Beacon message (8 bytes)    // Mensaje encapsulado de la capa MAC

}net_StandardMessage;


#endif /* NET_HEADERS_MESSAGE_H_ */
