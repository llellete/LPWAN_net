/*
 * main.c
 *
 *  Created on: 29 feb 2024
 *      Author: root
 */

#include "netstack.h"
//#include "bsp.h"
#include "platform.h"

void main(void)
{
  // Initialize the Hardware and Radio
  vInitializeHW();

  vInit_net();

  while (TRUE)
  {

      vHmi_LedHandler();
      vHmi_BuzzHandler();
     //vHmi_PbHandler();
    //Pollhandler();

  }


}
