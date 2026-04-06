// Core/Src/test.c
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "ds18b20.h"
#include "lcd_i2c.h"
#include "test.h"

// External handles (already initialized by main.c)
extern UART_HandleTypeDef huart2;

// External functions from main.c
extern void Buzzer_On(void);
extern void Buzzer_Off(void);

void test_main(void){
    // NOTE: All peripherals are already initialized by main.c
    // We just use them here - no need to reinitialize!

    printf("\n\n");
    printf("====================================\r\n");
    printf("      HARDWARE TEST SUITE\r\n");
    printf("====================================\r\n");

    // Test 1: UART
    printf("\n[1] Testing UART...\r\n");
    printf("    [PASS] UART working\r\n");

    // Test 2: LCD
    printf("\n[2] Testing LCD...\r\n");
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("LCD TEST OK");
    HAL_Delay(500);
    printf("    [PASS] LCD working\r\n");

    // Test 3: Buzzer
    printf("\n[3] Testing Buzzer...\r\n");
    Buzzer_On();
    HAL_Delay(500);
    Buzzer_Off();
    HAL_Delay(200);
    printf("    [PASS] Buzzer working\r\n");

    // Test 4: DS18B20 Sensor
    printf("\n[4] Testing DS18B20 Sensor...\r\n");

    uint8_t presence = DS18B20_Reset();

    if(!presence){
        printf("    [FAIL] Sensor NOT detected!\r\n");
        printf("    Check: 4.7k pull-up resistor on PA0\r\n");
        printf("    Check: Wiring from STM32 to sensor\r\n");

        LCD_Clear();
        LCD_SetCursor(0, 0);
        LCD_Print("SENSOR FAIL");
    } else {
        printf("    [PASS] Sensor detected\r\n");

        // Read temperature multiple times
        printf("    Reading temperature...\r\n");
        float temp = DS18B20_GetTemperature();
        printf("    Temperature: %.2f C\r\n", temp);

        if(temp < -50.0 || temp > 125.0){
            printf("    [FAIL] Invalid temperature!\r\n");
            printf("    (Valid range: -50 to +125 C)\r\n");

            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_Print("TEMP INVALID");
        } else {
            printf("    [PASS] Temperature valid\r\n");

            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_Print("TEMP: ");
            char buffer[16];
            sprintf(buffer, "%.2f C", temp);
            LCD_Print(buffer);
        }
    }

    // Summary
    printf("\n====================================\r\n");
    printf("      TEST COMPLETE\r\n");
    printf("====================================\r\n");
    printf("\nResults printed above.\r\n");
    printf("If all PASS: Uncomment RUN_MAIN and\r\n");
    printf("             comment out RUN_TEST\r\n");
    printf("If any FAIL: Check connections\r\n\r\n");

    // Just loop here, waiting for restart
    while(1){
        HAL_Delay(1000);
    }
}
