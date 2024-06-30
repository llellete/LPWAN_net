/*! @file radio.h
 * @brief This file is contains the public radio interface functions.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef SI446X_PHY_H_
#define SI446X_PHY_H_

/*****************************************************************************
 *  Global Macros & Definitions
 *****************************************************************************/
/*! Maximal packet length definition (FIFO size) */
#define RADIO_MAX_PACKET_LENGTH     64u


#include "bsp.h"

/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
  U8   *Radio_ConfigurationArray;

  U8   Radio_ChannelNumber;
  U8   Radio_PacketLength;
  U8   Radio_State_After_Power_Up;

    U16  Radio_Delay_Cnt_After_Reset;

    U8   Radio_CustomPayload[RADIO_MAX_PACKET_LENGTH];
} tRadioConfiguration;

/*****************************************************************************
 *  Global Variable Declarations
 *****************************************************************************/
extern const SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SEG_CODE, SEG_CODE);
extern SEGMENT_VARIABLE(customRadioPacket[RADIO_MAX_PACKET_LENGTH], U8, SEG_XDATA);

/*! Si446x configuration array */
extern const SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8, SEG_CODE);

/*****************************************************************************
 *  Global Function Declarations
 *****************************************************************************/
  void  vRadio_Init(void);
  void vRadio_PowerUp(void);
  state    bRadio_Check_Tx_RX();
  void  vRadio_StartRX(U8 packetLenght);
  state    bRadio_Check_Ezconfig(U16);
  void  vRadio_StartTx_Variable_Packet(U8, U8*, U8);
  state  vSendRadioMessage(U8 *pioRadioPacket);
  state  vStartReception();
  state   vPendingMessage();
  U8    vReadRSSI();
  void  vRadioSleep();
  state vMessageSent();


#endif /* RADIO_H_ */


