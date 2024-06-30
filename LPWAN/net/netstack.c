/*! @file main.c
 * @brief The main.c file of the Fixed packet length Custom RX demo
 * for Si446X devices.
 *
 * Contains the initialization of the MCU & the radio.
 * @n The main loop controls the program flow & the radio.
 *
 * @b CREATED
 * @n Silicon Laboratories Ltd.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 *
 */

#include <headers/bsp.h>
#include "netstack.h"
#include "STDLIB.H"
#include "STDIO.H"
#include "phy_layer.h"
#include "si446x_phy.h"
#include "TDMA.h"
#include "mac_layer.h"
#include "lpwan_network_layer.h"

/*------------------------------------------------------------------------*/
/*                      Local function prototypes                         */
/*------------------------------------------------------------------------*/



/*------------------------------------------------------------------------*/
/*                          Global variables                              */
/*------------------------------------------------------------------------*/
SEGMENT_VARIABLE(bMain_IT_Status, U8, SEG_XDATA);
SEGMENT_VARIABLE(lPer_MsCnt, U16, SEG_DATA);


/*------------------------------------------------------------------------*/
/*                              Defines                                   */
/*------------------------------------------------------------------------*/
#define PACKET_SEND_INTERVAL 2000u

// Definimos la radio específica que vamos a usar
#define PHY_LAYER si446x_phy_funcs
#define MAC_LAYER TDMA_mac_funcs
//#define NET_LAYER lpwan_net_funcs



extern phy_layer_funcs_t PHY_LAYER;
extern mac_layer_funcs_t MAC_LAYER;
//extern network_layer_funcs_t NET_LAYER;


netstack_t netstack;

/*------------------------------------------------------------------------*/
/*                             Enumeration                                */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/*                             Typedefs                                   */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/*                          Local variables                               */
/*------------------------------------------------------------------------*/
SEGMENT_VARIABLE(rollingCounter, U16, SEG_XDATA);

/*------------------------------------------------------------------------*/
/*                          Function implementations                      */
/*------------------------------------------------------------------------*/

/** \fn void main(void)
 * \brief The main function of the demo.
 *
 * \todo Create description
 */

void vInit_net(){

  init_rx_buffer();
  init_tx_buffer();
  init_network_layer();

  vInit_phy_layer(&PHY_LAYER);
  vInit_mac_layer(&MAC_LAYER);

 // vInit_network_layer(&NET_LAYER);

}




/*!
 * This function is used to wait for a little time.
 *
 * @return  None.
 */
void vSample_Code_Wait(U16 wWaitTime)
{
  SEGMENT_VARIABLE(wDelay, U16 , SEG_DATA) = wWaitTime;

  for (; wDelay--; )
  {
    NOP();
  }
}



/** \fn void MCU_Init(void)
 *  \brief Initializes ports of the MCU.
 *
 *  \return None
 *
 *  \note It has to be called from the Initialization section.
 *  \todo Create description
 */


void vSet_phy_layer(struct phy_layer_funcs_* phy_layer){

  netstack.phy_layer_funcs = phy_layer;

}

void vSet_mac_layer(struct mac_layer_funcs_* mac_layer){

  netstack.mac_layer_funcs = mac_layer;

}

void vSet_network_layer(struct network_layer_funcs_* network_layer){

  netstack.network_layer_funcs = network_layer;

}





