/*
 * network_layer.c
 *
 *  Created on: 14 jun 2024
 *      Author: Sergio
 */


#include "network_layer.h"
#include <netstack.h>



extern netstack_t netstack;

void vInit_network_layer(network_layer_funcs_t* network_layer){

  vSet_network_layer(network_layer);

  network_layer->network_layer_init();

}




