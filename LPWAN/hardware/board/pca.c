/*! @file pca.c
 * @brief This file contains the interface functions of the programmable counter array (PCA).
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include <headers/bsp.h>

#if !( (defined SILABS_MCU_F930) || (defined SILABS_MCU_F912) )
  #error Check implementation for given MCU!
#endif

/*------------------------------------------------------------------------*/
/*                          Global variables                              */
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
/*                          Function implementations                      */
/*------------------------------------------------------------------------*/

/*!
 * This function is used to start Timer 2 in the specified mode.
 *
 * @param[in] biPulseSelect       Selects time-base source of PCA counter (use predefined constants: bPca_PcaCps_000_c etc.)
 * @param[in] biPcaTmrItEnable    Enables PCA timer IT if TRUE, disables it if FALSE
 * @param[in] biCycleLengthSelect PWM-mode cycle length select (use predefined constants: bPca_PwmClsel_00_c etc.)
 *
 * @return  None.
 */
void vPca_InitPcaTmr(U8 biPulseSelect, U8 biCycleLengthSelect, U16 period, U8 biSelectComp)
{
  if(biSelectComp){
     PCA0MD = M_CIDL | (biPulseSelect & M_CPS);
     PCA0CPM1 = 0x49;

     PCA0L = 0x00;
     PCA0H = 0x00;

     PCA0CPL1 = period & 0x00FF;
     PCA0CPH1 = (period >> 8) & 0x00FF;

     mPca_EnablePcaTmrIt();

  }else{

      //PCA0MD |= M_CIDL | (biPulseSelect & M_CPS);
      PCA0PWM = M_ARSEL | (biCycleLengthSelect & M_CLSEL);
  }

}
