/*
 * phy_layer.h
 *
 *  Created on: 13 feb 2024
 *      Author: root
 */

#ifndef NET_PHY_LAYER_HEADERS_PHY_LAYER_H_
#define NET_PHY_LAYER_HEADERS_PHY_LAYER_H_

#include "funcs.h"
#include "netstack.h"

typedef struct phy_layer_funcs_{
  void  (*phy_layer_init)();
  void  (*phy_layer_sleep)();
  state (*phy_layer_startRX)();
  state (*phy_layer_sendPacket)(U8 *pioRadioPacket);
  state (*phy_layer_pendingMessage)();
  U8    (*phy_layer_rssi)();
}phy_layer_funcs_t;

void vInit_phy_layer     (phy_layer_funcs_t* phy_layer);

#endif /* NET_PHY_LAYER_HEADERS_PHY_LAYER_H_ */
