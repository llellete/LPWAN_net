/*! @file isr.c
 * @brief This file contains the interrupt functions of the application.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */


#include <headers/bsp.h>
#include "../../../TMDA/net/mac_layer/headers/TDMA.h"


/*------------------------------------------------------------------------*/
/*                          Global variables                              */
/*------------------------------------------------------------------------*/




/*------------------------------------------------------------------------*/
/*                       Interrupt implementations                        */
/*------------------------------------------------------------------------*/

/*!
 * This function is the interrupt service routine of Timer 2.
 * Clears IT flag and sets ms flag.
 *
 * @return  None.

INTERRUPT(vIsr_Timer2Isr, INTERRUPT_TIMER2)
{
  mTmr_ClearTmr2It();
  wIsr_Timer2Tick = TRUE;

  currentTimeMs = currentTimeMs + 1;
}
 */

/*!
 * This function is the interrupt service routine of PCA Timer.
 * Clears IT flag and reloads PWM duty cycle of the buzzer.
 *
 * @return  None.
 */

