/*
 * lpwan_network_layer.h
 *
 *  Created on: 14 jun 2024
 *      Author: Sergio
 */

#ifndef NET_NETWORK_LAYER_HEADERS_LPWAN_NETWORK_LAYER_H_
#define NET_NETWORK_LAYER_HEADERS_LPWAN_NETWORK_LAYER_H_

#include "TDMA.h"
#include "packetbuffer.h"





void init_network_layer();
void init_rout_table();
state put_rout_main(unsigned int fateID);
unsigned char get_next_jump_ID(unsigned int fateID);
void send_message(unsigned int ID);
void send_routMessage(routTable table[SIZE_ROUT]);



#endif /* NET_NETWORK_LAYER_HEADERS_LPWAN_NETWORK_LAYER_H_ */
