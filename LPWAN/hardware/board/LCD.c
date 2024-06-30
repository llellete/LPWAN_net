/*
 * LCD.c
 *
 *  Created on: 7 ago. 2023
 *      Author: User
 */

#include "LCD.h"

//Lcd write function provide the line and the text to be written
void Lcd_Write_Line(U8 biLine, U8 * pbiText){

  vLcd_LcdWriteLine(biLine, pbiText);

}

//LCD initialization
void Lcd_Init(){

  vLcd_InitLcd();

}
