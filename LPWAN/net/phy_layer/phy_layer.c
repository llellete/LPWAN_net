/*
 * phy_layer.c
 *
 *  Created on: 12 mar 2024
 *      Author: Sergio
 */
#include "netstack.h"
#include "phy_layer.h"




void vInit_phy_layer(phy_layer_funcs_t* phy_layer){

  vSet_phy_layer(phy_layer);

  phy_layer->phy_layer_init();

}



