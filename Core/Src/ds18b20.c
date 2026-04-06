#include "ds18b20.h"
#include <stdio.h>

//-------------------------------helper functions---------------------------
static void pin_low(void){
    // Step 1: configure pin as OUTPUT
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin   = DS18B20_PIN;      // PA0
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; // push-pull output
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

    // Step 2: write LOW (0V) to pin
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
}

static void pin_release(void){
    // configure pin as INPUT
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // input mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;      // no internal pull
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

/* Read current state of DATA pin */
static uint8_t pin_read(void){
    return HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN);
}

void delay(uint16_t us){
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while(__HAL_TIM_GET_COUNTER(&htim1) < us);
}
//--------------------------------------------------------------------------


uint8_t DS18B20_Reset(){
    pin_low();
    delay(480);
    pin_release();
    delay(70); //waiting for sensor response

    uint8_t presence = !pin_read();
    delay(410); //wait for timeslot end
    printf("Presence: %d\r\n", presence);
    return presence; //1 = sensor found, 0 = not found
}


void DS18B20_WriteBit(uint8_t bit){
    pin_low();
    if(bit){
        delay(10);
        pin_release(); //high
        delay(55);
    } else {
        delay(65);
        pin_release();
        delay(5);
    }
}

void DS18B20_WriteByte(uint8_t byte){
    for(int i = 0; i < 8; i++){
        DS18B20_WriteBit(byte & (1 << i));
    }
}

uint8_t DS18B20_ReadBit(){
    pin_low();
    delay(2);
    pin_release();
    delay(10);
    uint8_t bit = pin_read();
    delay(50);
    return bit;
}

uint8_t DS18B20_ReadByte(){
    uint8_t byte = 0;
    for(int i = 0; i < 8; i++){
        byte |= (DS18B20_ReadBit() << i);
    }
    return byte;
}

float DS18B20_GetTemperature(){
    DS18B20_Reset();
    DS18B20_WriteByte(0xCC); //skip ROM
    DS18B20_WriteByte(0x44); //convert T

    HAL_Delay(750); //wait for conversion

    DS18B20_Reset();
    DS18B20_WriteByte(0xCC); // skip ROM
    DS18B20_WriteByte(0xBE); // read scratchpad

    uint8_t lsb = DS18B20_ReadByte();
    uint8_t msb = DS18B20_ReadByte();

    int16_t raw = (msb << 8) | lsb;
    return raw / 16.0;
}
