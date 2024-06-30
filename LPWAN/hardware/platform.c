/*
 * main_hw.c
 *
 *  Created on: 27 feb 2024
 *      Author: root
 */
#include <headers/bsp.h>

#include "platform.h"



void vInitializeHW()
{

  // Initialize the MCU peripherals
  vPlf_McuInit();

  // Initialize IO ports
  vCio_InitIO();

  // Initialize Tmr0 and PCA0 1F47

  vPca_InitPcaTmr(bPca_PcaCps_100_c, 0, 0x1F47, TRUE);

  //vTmr_StartTmr2(eTmr_SysClkDiv12_c, wwTmr_Tmr2Periode.U16, FALSE, bTmr_TxXCLK_00_c);

  // Start INT0

  mTmr_EnableINT0();
  mTmr_EnableINT1();
  mPca_EnablePcaTmr();
  // Start the push button handler
  vHmi_InitPbHandler();

  // Start the Led handler
  vHmi_InitLedHandler();

  // Start the Buzzer Handler
  //vHmi_InitBuzzer();

  mTmr_StartTmr0();

  // Enable configured interrupts
  mIsr_EnableAllIt();

  /* Initialize graphic LCD */
  vLcd_InitLcd();


  vLcd_LcdWriteLine(2, "TDMA DEMO");


}

void vPlf_McuInit(void)
{
  U16 wDelay = 0xFFFF;

  /* disable watchdog */
  PCA0MD &= (~M_WDTE);

  /* Init Internal Precision Oscillator (24.5MHz) */
  SFRPAGE = LEGACY_PAGE;
  FLSCL   = M_BYPASS;

  OSCICN |= M_IOSCEN; // p7: Internal Prec. Osc. enabled
  CLKSEL  = 0x00;     // Int. Prec. Osc. selected (24.5MHz)



#if (defined SILABS_PLATFORM_WMB930)
  /* Port IN/OUT init */
  P0MDOUT = 0x80;
  P1MDOUT = 0xF5;
  P2MDOUT = 0x49;

  P0SKIP  = 0xCF;
  P1SKIP  = 0x18;
  P2SKIP  = 0xB9;


  P0MDIN  = 0xFF; // All pin configured as digital port
  P1MDIN  = 0xFF; // All pin configured as digital port
#if !(defined SILABS_PLATFORM_WMB912)
  P2MDIN  = 0xFF; // All pin configured as digital port
#endif

  /* Set Drive Strenght */
  SFRPAGE = CONFIG_PAGE;
  P0DRV   = 0x00;
  P1DRV   = 0x00;
#if !(defined SILABS_PLATFORM_WMB912)
  P2DRV   = 0x00;
#endif

  SFRPAGE = LEGACY_PAGE;

  /* Crossbar configuration */
  XBR0    = M_URT0E | M_SMB0E; //p0: UART enabled on XBAR
  XBR1    = M_SPI1E ; //p6: SPI1 enabled on XBAR

#if (defined SILABS_PLATFORM_WMB930)
  XBR1    |= (1 << BF_PCA0ME_0); //p0: PCA CEX0 enabled on XBAR
  XBR0    |= M_SPI0E;

  XBR2    = M_XBARE; //p6: XBAR enable

  /* latch all inputs to '1' */
  P0      = ~P0MDOUT;
  P1      = ~P1MDOUT;
#if !(defined SILABS_PLATFORM_WMB912)
  P2      = ~P2MDOUT;
#endif

  /* set all output to its default state */
  LED1      = EXTINGUISH;
#if !(defined SILABS_PLATFORM_WMB912)
  LED2      = EXTINGUISH;
  LED3      = EXTINGUISH;
  LED4      = EXTINGUISH;
#endif
  RF_NSEL   = TRUE;
  RF_PWRDN  = FALSE;

  /* SPI1 & SPI0 Config & Enable */
  SPI0CFG   = 0x40;
  SPI1CFG   = 0x40;
#if !(defined SILABS_PLATFORM_WMB912)
  SPI0CN    = 0x01;
  SPI1CN    = 0x01;
  SPI0CKR   = 0x0B;
  SPI1CKR   = 0x0B;

  /* De-select radio SPI */
  vSpi_SetNsel(eSpi_Nsel_RF);

#if ((defined SILABS_LCD_DOG_GLCD) || (defined SILABS_MCU_DC_EMIF_F930) || (defined SILABS_PLATFORM_WMB))
  /* De-select LCD SPI */
  vSpi_SetNsel(eSpi_Nsel_LCD);

  LCD_A0    = FALSE;
#endif

  /* Startup delay */
  for (; wDelay; wDelay--)  ;

}


