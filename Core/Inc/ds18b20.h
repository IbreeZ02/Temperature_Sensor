#ifndef DS18B20_H
#define DS18B20_H

#include "stm32f4xx_hal.h"

#define DS18B20_PORT GPIOA
#define DS18B20_PIN GPIO_PIN_0

#define DS18B20_CMD_SKIP_ROM 0xCC   // talk to all sensors
#define DS18B20_CMD_CONVERT_T 0x44   // start temperature conversion
#define DS18B20_CMD_READ_SCRATCH 0xBE   // read temperature result

extern TIM_HandleTypeDef htim1;

uint8_t DS18B20_Reset(void);
void DS18B20_WriteBit(uint8_t bit);
void DS18B20_WriteByte(uint8_t byte);
uint8_t DS18B20_ReadByte(void);
float DS18B20_GetTemperature(void);

#endif
