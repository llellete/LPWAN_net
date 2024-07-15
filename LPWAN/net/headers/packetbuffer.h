/*
 * packetbuffer.h
 *
 *  Created on: 19 mar 2024
 *      Author: Sergio
 */

#ifndef NET_HEADERS_PACKETBUFFER_H_
#define NET_HEADERS_PACKETBUFFER_H_
#include "funcs.h"
#include "message.h"


# define SIZE 5
# define packet_buffer[SIZE]
#define MAX_PAYLOAD_SIZE 16 // Payload size



static unsigned int MY_ID = 1;
static unsigned int NODE_ID;

static U16 currentTimeMs = 0x0000;

typedef struct {
  net_StandardMessage buffer[SIZE];
  int rear;
  int front;
}packetbuffer;

state init_tx_buffer();
state init_rx_buffer();
state put_tx_packet(net_StandardMessage message);
net_StandardMessage get_tx_packet();
state put_rx_packet(net_StandardMessage message);
net_StandardMessage get_rx_packet();







#endif /* NET_HEADERS_PACKETBUFFER_H_ */
