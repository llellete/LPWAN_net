/*
 * network_layer.h
 *
 *  Created on: 14 jun 2024
 *      Author: Sergio
 */

#ifndef NET_NETWORK_LAYER_HEADERS_NETWORK_LAYER_H_
#define NET_NETWORK_LAYER_HEADERS_NETWORK_LAYER_H_

#include "funcs.h"
#include "netstack.h"
#include "packetbuffer.h"

typedef struct network_layer_funcs_{
  state   (*network_layer_init)();
  state   (*network_layer_send_packet)();
  net_StandardMessage (*network_layer_recieve_packet)();
}network_layer_funcs_t;

void vInit_network_layer     (network_layer_funcs_t* network_layer);


#endif /* NET_NETWORK_LAYER_HEADERS_NETWORK_LAYER_H_ */
