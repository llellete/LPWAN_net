/*! @file isr.h
 * @brief Header file of interrupt service routines.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef ISR_H_
#define ISR_H_
#include "../../../net/headers/bsp.h"
/*------------------------------------------------------------------------*/
/*                            Global macros                               */
/*------------------------------------------------------------------------*/

/*!
 * @brief Definition of enable all ITs.
 */
#define mIsr_EnableAllIt() EA = 1

#define mTmr_EnableINT0()  IT0 = 0; IE0 = 0; EX0 = 1; IT01CF = 0x01;

#define mTmr_ClearINT0()  IE0 = 0

#define mTmr_EnableINT1()  IT1 = 0; IE1 = 0; EX1 = 1; IT01CF = 0x01;

#define mTmr_ClearINT1()  IE1 = 0



/*!
 * @brief Definition of disable all ITs.
 */
#define mIsr_DisableAllIt() EA = 0

/*------------------------------------------------------------------------*/
/*                          Function prototypes                           */
/*------------------------------------------------------------------------*/
#ifdef SDCC
/*! Interrupt Service Handler function prototype declarations required
 * by SDDC */
INTERRUPT_PROTO(vIsr_Timer2Isr, INTERRUPT_TIMER2);
INTERRUPT_PROTO(vIsr_PcaTimerIsr, INTERRUPT_PCA0);
#endif

/*------------------------------------------------------------------------*/
/*                          External variables                            */
/*------------------------------------------------------------------------*/

extern volatile unsigned int currentTime; /**< ms counter */



#endif /* ISR_H_ */
