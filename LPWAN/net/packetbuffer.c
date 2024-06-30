/*
 * packetbuffer.c
 *
 *  Created on: 19 mar 2024
 *      Author: Sergio
 */


#include "packetbuffer.h"
#include "bsp.h"
#include "funcs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// INICIALIZACIÓN DE LOS BUFFER

packetbuffer tx_buffer;
packetbuffer rx_buffer;

state init_tx_buffer(){

  tx_buffer.front = -1;
  tx_buffer.rear  = -1;
  return OK;
}

state init_rx_buffer(){

  tx_buffer.front = -1;
  tx_buffer.rear = -1;
  return OK;
}



// MANEJADORES DEL BUFFER TX


state put_tx_packet(net_StandardMessage message)
{
  if(message.packet_hdr.mac_hdr.type == STANDARD_MSG){
      char customData[MAX_PAYLOAD_SIZE] = "ST";
      memcpy(message.payload, customData, MAX_PAYLOAD_SIZE);
  } else if(message.packet_hdr.mac_hdr.type == BROADCAST_MSG){
      char customData[MAX_PAYLOAD_SIZE] = "BR";
      memcpy(message.payload, customData, MAX_PAYLOAD_SIZE);
  }
    if (tx_buffer.rear == SIZE - 1){
       init_tx_buffer();
       return ERROR;
    }

        if (tx_buffer.front == - 1){
            tx_buffer.front = 0;
        }

        tx_buffer.rear = tx_buffer.rear + 1;
        tx_buffer.buffer[tx_buffer.rear] = message;
        return OK;

}

net_StandardMessage get_tx_packet()
{
  net_StandardMessage message;
  if (tx_buffer.front == - 1 || tx_buffer.front > tx_buffer.rear)
      {
        message.packet_hdr.mac_hdr.type = CLEAN;
        message.packet_hdr.mac_hdr.deviceID = 0;
        message.packet_hdr.mac_hdr.nodeID = 0;
        message.packet_hdr.mac_hdr.timestamp = 0;

        return message;
      }else{

        message = tx_buffer.buffer[tx_buffer.front];

        tx_buffer.front = tx_buffer.front + 1;

        return message;
      }
}


// MANEJADORES DEL BUFFER RX


state put_rx_packet(net_StandardMessage message)
{
    if (rx_buffer.rear == SIZE - 1){
        init_rx_buffer();
       return ERROR;
    }

        if (rx_buffer.front == - 1){
            rx_buffer.front = 0;
        }

        rx_buffer.rear = rx_buffer.rear + 1;
        rx_buffer.buffer[rx_buffer.rear] = message;
        return OK;

}

net_StandardMessage get_rx_packet()
{
  net_StandardMessage message;
  if (rx_buffer.front == - 1 || rx_buffer.front > rx_buffer.rear)
      {
        message.packet_hdr.mac_hdr.type = CLEAN;
        message.packet_hdr.mac_hdr.deviceID = 0;
        message.packet_hdr.mac_hdr.nodeID = 0;
        message.packet_hdr.mac_hdr.timestamp = 0;

        return message;
      }else{

        message = rx_buffer.buffer[rx_buffer.front];

        rx_buffer.front = rx_buffer.front + 1;

        return message;
      }
}




