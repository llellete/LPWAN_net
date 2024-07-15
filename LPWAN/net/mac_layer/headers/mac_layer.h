/*
 * mac_layer.h
 *
 *  Created on: 29 abr 2024
 *      Author: Sergio
 */

#ifndef NET_MAC_LAYER_HEADERS_MAC_LAYER_H_
#define NET_MAC_LAYER_HEADERS_MAC_LAYER_H_

#include "funcs.h"
#include "netstack.h"
#include "message.h"

typedef struct mac_layer_funcs_{
  state   (*mac_layer_init)();
  state   (*mac_layer_send_packet)(unsigned char ,unsigned char, net_StandardMessage) reentrant;
  net_StandardMessage (*mac_layer_receive_packet)();
  void    (*mac_layer_set_id)(unsigned char);
  state   (*mac_layer_send_broadcast)(routTable table[SIZE_ROUT]) reentrant;
}mac_layer_funcs_t;

void vInit_mac_layer     (mac_layer_funcs_t* mac_layer);




#endif /* NET_MAC_LAYER_HEADERS_MAC_LAYER_H_ */
