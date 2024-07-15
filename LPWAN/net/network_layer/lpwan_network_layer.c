/*
 * lpwan_network_layer.c
 *
 *  Created on: 14 jun 2024
 *      Author: Sergio
 */

#include "lpwan_network_layer.h"
#include "funcs.h"
#include "netstack.h"
#include "mac_layer.h"
#include "TDMA.h"
#include "LCD.h"
#include "message.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern netstack_t netstack;

Table table;

void init_network_layer(){

  init_rout_table();

}


void send_message(unsigned char ID){

    net_StandardMessage netmessage;

    unsigned char nodeID  = get_next_jump_ID(ID);

    netstack.mac_layer_funcs->mac_layer_send_packet(nodeID, ID, netmessage);

}

void send_routMessage(routTable table[SIZE_ROUT]){
  int i;


    routTable routTable[SIZE_ROUT];

    for(i = 0; i < SIZE_ROUT; ++i){

    routTable[i] = table[i];

    }

    netstack.mac_layer_funcs->mac_layer_send_broadcast(routTable);

}



void init_rout_table(){

  table.front = -1;
  table.rear = -1;

}

state put_rout_main(unsigned int fateID){

  int i;

  routTable routTable[SIZE_ROUT];

    if (table.rear == SIZE_ROUT - 1){
        init_rout_table();
       return ERROR;
    }

        if (table.front == - 1){
            table.front = 0;
        }

        table.rear = table.rear + 1;
        table.table[table.rear].sourceID = MY_ID;
        table.table[table.rear].fateID = fateID;
        table.table[table.rear].next_jump_ID = fateID;
        table.table[table.rear].steps = 1;


        for(i = 0; i < SIZE_ROUT; ++i){

        routTable[i] = table.table[i];

        }

        send_routMessage(routTable);

        return OK;

}

void update_rout_table(routTable new_table[SIZE_ROUT]){

  int i;

  table.table[0].fateID = new_table[0].sourceID;
  table.table[0].next_jump_ID = new_table[0].sourceID;
  table.table[0].sourceID = MY_ID;
  table.table[0].steps = 1;

  for(i = 1; i < SIZE_ROUT; ++i){

      if(new_table[i].fateID != MY_ID){

      table.table[i].next_jump_ID = new_table[i-1].sourceID;
      table.table[i].fateID =  new_table[i-1].fateID;
      table.table[i].sourceID = MY_ID;
      table.table[i].steps = 2;

      }else{

          table.table[i].next_jump_ID = 0;
          table.table[i].fateID =  0;
          table.table[i].sourceID = 0;
          table.table[i].steps = 0;

      }
  }

}

void resend_packet(net_StandardMessage netmessage){



}

unsigned char get_next_jump_ID(unsigned int fateID){
  int i = 0;

  for(i = 0; i < SIZE_ROUT; ++i){

      if (table.table[i].fateID == fateID){

          return table.table[i].next_jump_ID;

      }

  }

}


INTERRUPT(vIsr_INT1, INTERRUPT_INT1){

  mTmr_ClearINT1();
  Lcd_Write_Line(4, "Message buffer");

  //send_message(2);
  send_routMessage(table.table);

}
