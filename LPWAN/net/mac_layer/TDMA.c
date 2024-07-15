/*
 * TDMA.c
 *
 *  Created on: 21 jul. 2023
 *      Author: User
 */

#include <netstack.h>
#include "TDMA.h"
#include "message.h"
#include "phy_layer.h"
#include "si446x_phy.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LCD.h"
#include "mac_layer.h"
#include "lpwan_network_layer.h"

//Global variables


volatile unsigned int currentTime = 0; /**< ms counter */



mac_layer_funcs_t TDMA_mac_funcs = {
  init_mac_layer,
  send_packet,
  recieve_packet,
  set_id,
  send_broadcast,
};

// Static variables
static unsigned int MY_SLOT;                 // Slot de TX del nodo MAIN
static int ARRAY_INDEX = 0;                  // Índice de la tabla de slots
static int slotState = PENDING_SLOT;         // Rol del nodo en el SLOT actual
static unsigned int resetCycle = 0;          // Variable asignada para resetear en caso de desconexión
static DeviceFlow FSM = BEACON_ACTIVE;       // Estado de la sincronización inicial
static DeviceRole Role = NON_ASSIGNED;       // Rol del nodo
static unsigned int cyclesWithoutRole = 0;   // Número de ciclos que lleva el nodo sin rol asignado
static unsigned int deviceCount = 0;         //
static int RSSI = 0;                         // RSSI de la recepción
static unsigned int new_sync_counter = 0;    // Número de slots sin recibir paquetes
static unsigned int currentSlotTime = 0;     // Valor del slot actual
static set_pca = FALSE;                      // Flag para cambiar el pca en la temporización
static U16 PCA_Count = 0x1F47;               // Valor de comparación en el PCA para saltar interrupción
DeviceInfo deviceArray[MAX_DEVICES];         // Array de los nodos de la red y los slots, roles, etc
//static U8  lcd_data[RADIO_MAX_PACKET_LENGTH];
extern netstack_t netstack;


/** \fn sendStandard(unsigned int currentTimeMs)
 * \brief FUNCIÓN DE ENVIO DE PAQUETES EN ESTADO ESTACIONARIO
 */




void sendStandard(){
  // Creación del paquete a enviar asignándole el paquete de la primera posición del buffer de transmisión
  unsigned int currentTimeMs;
  net_StandardMessage message;

  message = get_tx_packet();
  currentTimeMs = PCA0;
  if(message.packet_hdr.mac_hdr.nodeID == 0){

      Lcd_Write_Line(4, "Broadcast sent");

  }else{

      Lcd_Write_Line(4, "Message sent");

  }


   if (message.packet_hdr.mac_hdr.type == CLEAN){

       message.packet_hdr.mac_hdr.timestamp = currentTimeMs;
       Lcd_Write_Line(4, "Not message");

   }
      memset(&customRadioPacket, 0, sizeof(customRadioPacket));


      // Copia en memoria del paquete a enviar en el buffer de la radio y su envío

      memcpy(&customRadioPacket[0], &message, sizeof(net_StandardMessage));
      netstack.phy_layer_funcs->phy_layer_sendPacket(&customRadioPacket[0]);

  }









/** \fn
 * \brief FUNCIONES DE SINCRONIZACIÓN INICIAL ENTRE NODOS
 */


void sendACK(U8 rout){
  // Creación de un ACK para su transmisión

  ACKMessage message;
  char customData[MAX_PAYLOAD_SIZE] = "ACK";
  message.type = ACK_MSG;
  message.deviceID = MY_ID;
  message.nodeSLOT = assignRandomSlot(MAX_DEVICES);
  Lcd_Write_Line(4, "Waiting to send");

  // Se asigna el slot de RX al nodo secundario

  addDevice(NODE_ID, message.nodeSLOT,RX, NODE, RSSI);

  // Set del payload para el ACK

  memcpy(message.payload, customData, MAX_PAYLOAD_SIZE);

  // Copia en memoria del ACK en el customRadioPacket buffer

  memcpy(&customRadioPacket[0], &message, sizeof(ACKMessage));

  // Se establece el nodo MAIN

  Role = MAIN;
  removeDevice(MY_ID, NON_ASSIGNED);
  addDevice(MY_ID,MY_SLOT,TX, MAIN, 0);

  netstack.phy_layer_funcs->phy_layer_sendPacket(&customRadioPacket[0]);
  Lcd_Write_Line(4, "Ack sent");

  FSM = BEACON_ACTIVE;

  put_rout_main(rout);
}


void sendSync() {
  // Creación de un SYNC para su transmisión

  SyncMessage message;
  char customData[MAX_PAYLOAD_SIZE] = "SYNC";
  message.type = SYNC_MSG;
  message.deviceID = MY_ID;
  Lcd_Write_Line(4, "Sending Sync");

  // Se limpia el customRadioPacket

  memset(&customRadioPacket, 0, sizeof(customRadioPacket));

  // Set del payload para el SYNC message

  memcpy(message.payload, customData, MAX_PAYLOAD_SIZE);

  // Copia en memoria del SYNC en el customRadioPacket buffer y envío del mismo

  memcpy(&customRadioPacket[0], &message, sizeof(SyncMessage));

  netstack.phy_layer_funcs->phy_layer_sendPacket(&customRadioPacket[0]);

}


void sendBeacon(){
  // Creación de un Beacon para su transmisión

  BeaconMessage message;
  char customData[MAX_PAYLOAD_SIZE] = "BCON";
  message.type = BEACON_MSG;
  message.deviceID = MY_ID; // Device ID
  message.deviceSLOT = MY_SLOT; // Device slot
  message.currentSLOT = deviceArray[ARRAY_INDEX].slot;
  currentTimeMs = PCA0;
  message.timestamp = currentTimeMs; // Device time
  sprintf(customRadioPacket, "Time: %u", currentTimeMs);

  // Se limpia el customRadioPacket

  memset(customRadioPacket, 0, sizeof(customRadioPacket));

  // Set the payload for the Beacon message

  memcpy (message.payload, customData, MAX_PAYLOAD_SIZE);

  // Copia en memoria del Beacon en el customRadioPacket buffer y envío del mismo

  memcpy(&customRadioPacket[0], &message, sizeof(BeaconMessage));

  netstack.phy_layer_funcs->phy_layer_sendPacket(&customRadioPacket[0]);

  Lcd_Write_Line(4, "Sending Beacon");


}





/** \fn
 * \brief FUNCIONES DE CONTROL DEL ARRAY DE DISPOSITIVOS Y SINCRONIZACIÓN
 */



void addDevice(int deviceID, int slot, DeviceState state, DeviceRole role, int rssi) {

int i = 0;
// Check if the device with the given ID already exists
for (i = 0; i < deviceCount; i++) {
   if (deviceArray[i].deviceID == deviceID) {
       // Remove the existing entry
       removeDevice(deviceID, role);
       break;
   }
 }

// Add the device
if (deviceCount < MAX_DEVICES) {
        deviceArray[slot-1].deviceID = deviceID;
        deviceArray[slot-1].slot = slot;
        deviceArray[slot-1].state = state;
        deviceArray[slot-1].role = role;
        deviceArray[slot-1].rssi = rssi;
        deviceCount++;
  }
}

void initializeDeviceArray() {
  int i = 0;
    for (i = 0; i < MAX_DEVICES; i++) {
        deviceArray[i].deviceID = -1;
        deviceArray[i].slot = i+1;
        deviceArray[i].state = IDLE;
        deviceArray[i].role = NON_ASSIGNED;
        deviceArray[i].rssi = 0;
    }
}


// Función de actualización del valor del PCA para sincronizar los nodos


void updateCount(U16 time) {
  U16 diff;
  mTmr_StopTmr0();
  mPca_StopPcaTmr();
  set_pca = TRUE;
  diff = (PCA_Count - time) + 0x0050;
  vPca_InitPcaTmr(bPca_PcaCps_100_c, 0, diff, TRUE);
  mPca_EnablePcaTmr();
  mTmr_StartTmr0();
}


// Función para asignar SLOT aleatorio


unsigned int assignRandomSlot(unsigned int num_slots) {
    unsigned int randomSlot = 0;
    unsigned int attempts = 0;

    randomSlot = (rand() % (num_slots - 1)) + 2; // Generate slot between 2 and num_slots

    while (getDeviceIDBySlot(randomSlot) != 65535 && attempts < num_slots) {

        randomSlot = (rand() % (num_slots - 1)) + 2; // Generate slot between 2 and num_slots
        attempts++;

    }

    return randomSlot;

}









/** \fn state init_mac_layer()
 * \brief FUNCIÓN DE INICIALIZACIÓN DE LA CAPA MAC
 */



state init_mac_layer(){

   // Inicialización del array de nodos


   initializeDeviceArray();

   // Asignación aleatoria de SLOT

   srand(MY_ID);
   MY_SLOT = assignRandomSlot(MAX_DEVICES);

   // Clean the customRadioPacket

   memset(&customRadioPacket, 0, sizeof(customRadioPacket));
   sprintf(customRadioPacket, "Slot:%u ID:%d", MY_SLOT, MY_ID);
   Lcd_Write_Line(1, customRadioPacket);

   // Asignación del SLOT de Beacon

   addDevice(MY_ID, 1, TX, BEACON, 0);

   // Relleno la tabla de nodos

   addDevice(MY_ID, MY_SLOT,TX, NON_ASSIGNED, 0);

   tdma_handler();


   return OK;

}



/** \fn void process_received_message()
 * \brief FUNCIONES DE PORCESADO DE PAQUETES RECIBIDOS
 */

void process_beacon_message() {

  // Copiamos el mensaje recibido para su processación

  BeaconMessage receivedMessage;
  memcpy(&receivedMessage, &customRadioPacket[0], sizeof(BeaconMessage));

  // Se procesa el mensaje Beacon


      if(FSM != PENDING_ACK && isDeviceIDInArray(receivedMessage.deviceID) == 0){

          // Sesincronizan los nodos

          ARRAY_INDEX = receivedMessage.currentSLOT;
          currentSlotTime = receivedMessage.currentSLOT + 1;
          currentTime = currentSlotTime;
          updateCount(receivedMessage.timestamp);
          NODE_ID = receivedMessage.deviceID;

          // Se añade el nodo de RX del Main

          addDevice(receivedMessage.deviceID, receivedMessage.deviceSLOT, RX, MAIN, RSSI);

          Lcd_Write_Line(4, "Beacon received");

          FSM = PENDING_ACK;

          sendSync();



          Lcd_Write_Line(4, "Waiting ACK");
        }
}


void process_sync_message(U8 rout) {

  // Copiamos el mensaje recibido para su processación

  SyncMessage receivedMessage;
  memcpy(&receivedMessage, &customRadioPacket[0], sizeof(SyncMessage));

  // Se procesa el mensaje SYNC

  NODE_ID = receivedMessage.deviceID;

  Lcd_Write_Line(4, "Sync received");

  vHmi_ChangeLedState(eHmi_Led2_c, eHmi_LedBlinkOnce_c);

  sendACK(rout);

}


void process_ack_message() {

  // Copiamos el mensaje recibido para su processación

  ACKMessage receivedMessage;
  memcpy(&receivedMessage, &customRadioPacket[0], sizeof(ACKMessage));


  // Se procesa el mensaje


  if((FSM == PENDING_ACK) && receivedMessage.deviceID == NODE_ID){

      removeDevice(MY_ID, NON_ASSIGNED);
      addDevice(MY_ID, receivedMessage.nodeSLOT,TX, NODE, RSSI);
      MY_SLOT = receivedMessage.nodeSLOT;
      Lcd_Write_Line(4, "ACK received");
      FSM = BEACON_ACTIVE;
      slotState = SLOT_ASSIGNED;
      Role =  NODE;
      vHmi_ChangeLedState(eHmi_Led3_c, eHmi_LedBlinkOnce_c);
   }
}



void process_standard_message() {

  // Copiamos el mensaje recibido para su processación

  net_StandardMessage receivedMessage;
  memcpy(&receivedMessage, &customRadioPacket[0], sizeof(net_StandardMessage));

  // Se procesa el mensaje Standard

   // Para la recepción en estado estacionario, se recibe el mensaje y se copia en el buffer de RX para ser procesado por las capas superiores


   new_sync_counter = 0;

   //updateCount(receivedMessage.timestamp);

   if(receivedMessage.packet_hdr.mac_hdr.nodeID == MY_ID){

       if(receivedMessage.packet_hdr.net_hdr.fateID != MY_ID){

           RSSI = (netstack.phy_layer_funcs->phy_layer_rssi()/2)-130;
           sprintf(customRadioPacket, "RSSI:%d dBm", RSSI);
           Lcd_Write_Line(3, customRadioPacket);

           Lcd_Write_Line(4, "Redirecting packet");
           send_message(receivedMessage.packet_hdr.net_hdr.fateID);

       } else {

           RSSI = (netstack.phy_layer_funcs->phy_layer_rssi()/2)-130;
           sprintf(customRadioPacket, "RSSI:%d dBm", RSSI);
           Lcd_Write_Line(3, customRadioPacket);

           Lcd_Write_Line(4, "Message received");
           put_rx_packet(receivedMessage);

       }

   }else if(receivedMessage.packet_hdr.mac_hdr.nodeID == 0){


           // Leemos y sacamos por pantalla el RSSI

           RSSI = (netstack.phy_layer_funcs->phy_layer_rssi()/2)-130;
           sprintf(customRadioPacket, "RSSI:%d dBm", RSSI);
           Lcd_Write_Line(3, customRadioPacket);

           Lcd_Write_Line(4, "Rout Table received");
           update_rout_table(receivedMessage.packet_hdr.net_hdr.table);

   }else{

       if (Role == MAIN){
           // Leemos y sacamos por pantalla el RSSI

           RSSI = (netstack.phy_layer_funcs->phy_layer_rssi()/2)-130;
           sprintf(customRadioPacket, "RSSI:%d dBm", RSSI);
           Lcd_Write_Line(3, customRadioPacket);

           Lcd_Write_Line(4, "Redirecting packet");
           send_message(receivedMessage.packet_hdr.net_hdr.fateID);

       }
   }



}






/** \fn void tdma_handler()
 * \brief FUNCIÓN DE CONTROLADOR DEL PROTOCOLO TDMA
 */



void tdma_handler() {

  // Se saca por pantalla la situación actual

  sprintf(customRadioPacket, "S:%d ID:%d N:%d R:%d", MY_SLOT, MY_ID, NODE_ID, (int)Role);
  Lcd_Write_Line(1, customRadioPacket);
  sprintf(customRadioPacket, "CS:%d S:%d", (int)deviceArray[ARRAY_INDEX].slot, (int)deviceArray[ARRAY_INDEX].state);
  Lcd_Write_Line(2, customRadioPacket);
  vLcd_LcdClearLine(4);
  vLcd_LcdClearLine(3);

  memset(&customRadioPacket, 0, sizeof(customRadioPacket));

  // SLOT en TX

  if (deviceArray[ARRAY_INDEX].state == TX) {

      // Rol BEACON

      if((deviceArray[ARRAY_INDEX].role == BEACON) && (Role != NODE)){

          if(FSM == BEACON_ACTIVE){

              sendBeacon();
              resetCycle = 0;

          }else if(FSM == PENDING_ACK){

            resetCycle++;

          }

      }

      if((Role != NON_ASSIGNED) && (deviceArray[ARRAY_INDEX].role != BEACON)){
        sendStandard();
        sprintf(customRadioPacket, "CS:%d S:%d T:%u", (int)deviceArray[ARRAY_INDEX].slot, (int)deviceArray[ARRAY_INDEX].state, currentTimeMs);
        Lcd_Write_Line(2, customRadioPacket);
      }

      // Rol NON_ASSIGNED

      if (Role == NON_ASSIGNED) {

          cyclesWithoutRole++;


      }else{

          cyclesWithoutRole = 0;


      }

      // En caso de que se hayan pasado un número de ciclos sin rol, se reinicia

      if (cyclesWithoutRole >= CYCLES_TO_CHANGE_SLOT) {

          cyclesWithoutRole = 0;
          removeDevice(MY_ID, BEACON);
          addDevice(MY_ID, assignRandomSlot(MAX_DEVICES), TX, BEACON,0);

      }

      if(Role == NON_ASSIGNED || deviceArray[ARRAY_INDEX].role == BEACON){
        // Start reception
          netstack.phy_layer_funcs->phy_layer_startRX();
      }else{
          netstack.phy_layer_funcs->phy_layer_sleep();
      }



  // SLOT en RX

  }else if (deviceArray[ARRAY_INDEX].state == RX) {


      netstack.phy_layer_funcs->phy_layer_startRX();


      // Se comprueba el número de slots sin recibir un paquete

      /*if(Role == NODE) {

          new_sync_counter++;

      }

      if(new_sync_counter > SYNC_REPEAT){

          Role = NON_ASSIGNED;
          new_sync_counter = 0;

      }
 */
  }

}

// Función para envío de paquetes

state send_packet(unsigned char ID, unsigned char fateID, net_StandardMessage message){

  message.packet_hdr.mac_hdr.type = STANDARD_MSG;
  message.packet_hdr.mac_hdr.deviceID = MY_ID;
  message.packet_hdr.mac_hdr.nodeID = ID;
  currentTimeMs = PCA0;
  message.packet_hdr.mac_hdr.timestamp = currentTimeMs;
  message.packet_hdr.net_hdr.fateID = fateID;

  return put_tx_packet(message);

}

state send_broadcast(routTable table[SIZE_ROUT]){
  int i;
  net_StandardMessage message;

  routTable routTable[SIZE_ROUT];

  for(i = 0; i < SIZE_ROUT; ++i){

  routTable[i] = table[i];

  }


  for(i = 0; i < SIZE_ROUT; ++i){

      message.packet_hdr.net_hdr.table[i] = routTable[i];

  }


  message.packet_hdr.mac_hdr.type = BROADCAST_MSG;
  message.packet_hdr.mac_hdr.deviceID = MY_ID;
  message.packet_hdr.mac_hdr.nodeID = 0;
  currentTimeMs = PCA0;
  message.packet_hdr.mac_hdr.timestamp = currentTimeMs;
  message.packet_hdr.net_hdr.fateID = 0;


  return put_tx_packet(message);

}


// Función para recepción de paquetes

net_StandardMessage recieve_packet(){

  net_StandardMessage message;

  message = get_rx_packet();

  return message;

}



// Función de cambio de ID por parte de la capa superior

void set_id(unsigned char ID){

  MY_ID = ID;

}





/** \fn INTERRUPT(vIsr_PCA0Isr, INTERRUPT_PCA0)
 * \brief FUNCIÓN DE CONTROL DE LA TEMPORIZACIÓN DEL PCA
 */



INTERRUPT(vIsr_PCA0Isr, INTERRUPT_PCA0) {

  mPca_ClearPcaTmrIt();


  // Se actualiza el valor de comparación del PCA en caso de que se haya modificado para la sincronización de nodos

  if(set_pca){

      mTmr_StopTmr0();
      mPca_StopPcaTmr();
      vPca_InitPcaTmr(bPca_PcaCps_100_c, 0, PCA_Count, TRUE);
      set_pca = FALSE;
      mPca_EnablePcaTmr();
      mTmr_StartTmr0();

  }

  // Se cambia de slot y se ejecuta el controlador para el siguiente slot

  currentTime = currentTime + 1;

  tdma_handler();

  if (currentTime >= currentSlotTime && currentTime <= currentSlotTime + 1) {

      ARRAY_INDEX++;
      //vHmi_ChangeLedState(eHmi_Led1_c, eHmi_LedToggle_c);

  }

  // Se comprueba que no se haya llegado al final del superciclo

  if (ARRAY_INDEX >= (MAX_DEVICES)) {

      ARRAY_INDEX = 0;
      currentTime = 0;

      if (resetCycle > 3) {

          resetCycle = 0;
          FSM = BEACON_ACTIVE;

      }
  }

  // Se actualiza el SLOT actual

  currentSlotTime = (deviceArray[ARRAY_INDEX].slot);

}


// Función de ejecución del procesado del paquetes cuando se detecta la recepción de un paquete mediante la interrupción de la radio

INTERRUPT(vIsr_INT0, INTERRUPT_INT0){
 // unsigned char type;

  mTmr_ClearINT0();


 // type = customRadioPacket[0];
  if(netstack.phy_layer_funcs->phy_layer_pendingMessage() == OK){

  if(customRadioPacket[0] == 0){

      if(Role == NON_ASSIGNED){

          process_beacon_message();

      }

  }else if(customRadioPacket[0] == 1){

      process_sync_message(customRadioPacket[1]);

  }else if(customRadioPacket[0] == 2){

      process_ack_message();

  }else if(customRadioPacket[0] == 3 || customRadioPacket[0] == 4){


          process_standard_message();


  }

  }
}



BIT gSampleCode_StringCompare(U8* pbiPacketContent, U8* pbiString, U8 lenght)
{
  while ((*pbiPacketContent++ == *pbiString++) && (lenght > 0u))
  {
    if( (--lenght) == 0u )
    {
      return TRUE;
    }
  }

  return FALSE;
}

/** \fn
 * \brief FUNCIONES CON POSIBLE FUNCIONES MÁS ADELANTE
 */



unsigned int getDeviceIDBySlot(unsigned int slot) {
  int i = 0;
    for (i = 0; i < MAX_DEVICES; i++) {
        if (deviceArray[i].slot == slot) {
            return deviceArray[i].deviceID;
        }
    }
    return 0;
}



uint8_t getCurrentDeviceID() {
  int i = 0;
    for (i = 0; i < MAX_DEVICES; i++) {
        if (deviceArray[i].slot <= currentTimeMs && currentTimeMs < deviceArray[i].slot + 1) {
            return deviceArray[i].deviceID;
        }
    }
    return 0;
}

void removeDevice(unsigned int deviceID, DeviceRole Role) {
    int i;
    int foundIndex = -1;

    // Find the index of the device with the given ID

    for (i = 0; i < MAX_DEVICES; i++) {
        if (deviceArray[i].deviceID == deviceID && deviceArray[i].role == Role ) {
            foundIndex = i;
            break;
        }
    }

    // If the device is found, mark it as unused
    if (foundIndex != -1) {
        deviceArray[foundIndex].deviceID = -1;
        deviceArray[foundIndex].state = IDLE;
        deviceArray[foundIndex].role = NON_ASSIGNED;
        deviceArray[foundIndex].rssi = 0;
        deviceCount--;
    }
}


int isDeviceIDInArray(unsigned int deviceID) {
    int i;

    // Iterate through the deviceArray to check if the device ID is present
    for (i = 0; i < MAX_DEVICES; i++) {
        if (deviceArray[i].deviceID == deviceID) {
            return 1; // Device ID found in the array
        }
    }

    return 0; // Device ID not found in the array
}

