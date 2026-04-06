#include "lcd_i2c.h"
#include <string.h>
#include <stdio.h>

//-------------------------------------------helper functions
/*static void LCD_WriteI2C(uint8_t data){
    HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS << 1), &data, 1, 100);
}*/
static void LCD_WriteI2C(uint8_t data){
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &data, 1, 100);

    if(status != HAL_OK) {
        printf("[I2C ERROR] Status: 0x%02X\r\n", status);
    }
}


static void LCD_PulseEnable(uint8_t data){
	LCD_WriteI2C(data | 0x04); //EN = high
	HAL_Delay(1);
	LCD_WriteI2C(data & ~0x04); //EN = low
	HAL_Delay(1);
}

static void LCD_Send(uint8_t data){
	//handle both data or command
    LCD_WriteI2C(data);      //send
    LCD_PulseEnable(data);   //pulse
}

static void LCD_SendNibble(uint8_t nibble){
	LCD_Send(nibble);
}

static void LCD_MakeNibble(uint8_t data, uint8_t rs){
	//split then send
    uint8_t upper = (data & 0xF0) | LCD_BACKLIGHT | rs;
    uint8_t lower = ((data << 4) & 0xF0) | LCD_BACKLIGHT | rs;
    LCD_SendNibble(upper);
    LCD_SendNibble(lower);
}
//-----------------------------------------------------------

void LCD_Init(void){
    HAL_Delay(50); //waiting for power

	//wake up sequence (in 8-bit)
	LCD_Send(0x30 | LCD_BACKLIGHT);
	HAL_Delay(5);
	LCD_Send(0x30 | LCD_BACKLIGHT);
	HAL_Delay(1);
	LCD_Send(0x30 | LCD_BACKLIGHT);
	HAL_Delay(1);
	LCD_Send(0x20 | LCD_BACKLIGHT); //switching to 4-bit mode
	HAL_Delay(1);

    LCD_MakeNibble(0x28, 0x00);  //4-bit communication, 2 lines, 5x8 font
    LCD_MakeNibble(0x0C, 0x00);  //cursor on/off
    LCD_MakeNibble(0x06, 0x00);  //entry mode
    LCD_MakeNibble(0x01, 0x00);  //clear display
    HAL_Delay(2);
}

/*
void LCD_Init(void){
    HAL_Delay(50);

    // Just one initialization attempt
    LCD_Send(0x30 | LCD_BACKLIGHT);
    HAL_Delay(100);

    printf("[LCD] Init attempt done\r\n");
}*/

void LCD_SetCursor(uint8_t row, uint8_t col){
    uint8_t address;

    if(row == 0){
        address = 0x00 + col;
    } else {
        address = 0x40 + col;
    }
    LCD_MakeNibble(0x80 | address, 0x00);
}

void LCD_Print(char* str){
    while(*str){
        LCD_MakeNibble(*str, 0x01);
        str++;
    }
}
//rs=1 → data

void LCD_Clear(){
    LCD_MakeNibble(0x01, 0x00);
    HAL_Delay(2);
}
