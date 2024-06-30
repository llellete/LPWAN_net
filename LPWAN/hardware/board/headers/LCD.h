/*
 * LCD.h
 *
 *  Created on: 7 ago. 2023
 *      Author: User
 */

#ifndef APPLICATION_LCD_H_
#define APPLICATION_LCD_H_

#include "../../../net/headers/bsp.h"

void Lcd_Write_Line(U8 biLine, U8 * pbiText);
void Lcd_Init();

#endif /* APPLICATION_LCD_H_ */
