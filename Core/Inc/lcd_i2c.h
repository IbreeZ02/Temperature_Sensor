#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "stm32f4xx_hal.h"
#define LCD_ADDRESS (0x27 << 1)
#define LCD_BACKLIGHT 0x08

extern I2C_HandleTypeDef hi2c1;


void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(char* str);

#endif
