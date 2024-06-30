/*
 * main.h
 *
 *  Created on: 23 feb 2024
 *      Author: Sergio
 */

#ifndef NET_HEADERS_NETSTACK_H_
#define NET_HEADERS_NETSTACK_H_

#include "bsp.h"

void vInit_net();
void vSet_phy_layer(struct phy_layer_funcs_* phy_layer);
void vSet_mac_layer(struct mac_layer_funcs_* mac_layer);
void vSet_network_layer(struct network_layer_funcs_* network_layer);


// Generamos la estructurta física y la estructura de red para luego asignarle la radio específica

struct phy_layer_funcs_;
struct mac_layer_funcs_;
struct network_layer_funcs_;

typedef struct netstack_{

  struct mac_layer_funcs_* mac_layer_funcs;
  struct phy_layer_funcs_* phy_layer_funcs;
  struct network_layer_funcs_* network_layer_funcs;

}netstack_t;




#endif /* NET_HEADERS_NETSTACK_H_ */
