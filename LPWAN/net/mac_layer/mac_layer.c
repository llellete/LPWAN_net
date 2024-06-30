/*
 * mac_layer.c
 *
 *  Created on: 29 abr 2024
 *      Author: Sergio
 */

#include "mac_layer.h"
#include "netstack.h"




void vInit_mac_layer(mac_layer_funcs_t* mac_layer){

  vSet_mac_layer(mac_layer);

  mac_layer->mac_layer_init();

}


