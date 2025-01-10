

#include "u8g2.h"
#include "u8g2_esp32_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c.h"
//#include "driver/gpio.h"

// I2C pins and OLED address
// #define I2C_SDA GPIO_NUM_21
// #define I2C_SCL GPIO_NUM_22
// #define OLED_ADDR 0x3C

// U8g2 object
u8g2_t u8g2;


extern "C" void app_main(void)
{

// Initialize U8g2 HAL
    u8g2_esp32_hal_t u8g2_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_hal.sda = U8G2_ESP32_HAL_UNDEFINED;
    u8g2_hal.scl = U8G2_ESP32_HAL_UNDEFINED;
    u8g2_esp32_hal_init(u8g2_hal);

    // Initialize U8g2 with HAL
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2,U8G2_R0, // Screen rotation
    u8g2_esp32_i2c_byte_cb, // ESP32 I2C callback
    u8g2_esp32_gpio_and_delay_cb // Delay callback
    );

    // Set I2C address
    // u8x8_SetI2CAddress(&u8g2.u8x8, OLED_ADDR << 1);

    // Power up and initialize the display
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

   
    // Draw "Hello World"
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 0, 24, "Hello World!");
    u8g2_SendBuffer(&u8g2);
  
   // Keep running
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  
   

}
