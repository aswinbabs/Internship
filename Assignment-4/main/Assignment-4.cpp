//program to know whether i/ps ON/OFF



#include <iostream>
#include "u8g2.h"
#include "u8g2_esp32_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include <cstring>

// I2C pins
const gpio_num_t i2c_sda_pin = GPIO_NUM_22;
const gpio_num_t i2c_scl_pin = GPIO_NUM_21;

// Button GPIO pins
const gpio_num_t button_inc = GPIO_NUM_18; // Increment button -power
const gpio_num_t button_dec = GPIO_NUM_19; // Decrement button -bell

// U8g2 object
u8g2_t u8g2;

// States
volatile bool bell_state = false;
volatile bool power_state = false;


// Function prototypes
void IRAM_ATTR button_isr_handler(void* arg);
void update_display();

void init_i2c() {
    i2c_config_t conf;
    memset(&conf, 0, sizeof(i2c_config_t));
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = i2c_sda_pin;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = i2c_scl_pin;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_1, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_1, conf.mode, 0, 0, 0));
}

void i2c_displayConfig() {
    u8g2_esp32_hal_t u8g2_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_hal.sda = U8G2_ESP32_HAL_UNDEFINED;
    u8g2_hal.scl = U8G2_ESP32_HAL_UNDEFINED;
    u8g2_esp32_hal_init(u8g2_hal);

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, 
        u8g2_esp32_i2c_byte_cb, 
        u8g2_esp32_gpio_and_delay_cb);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
}

void gpio_init() {
    gpio_config_t io_conf;

    // Configure increment button
    io_conf.intr_type = GPIO_INTR_NEGEDGE;       // Interrupt on falling edge
    io_conf.mode = GPIO_MODE_INPUT;             // Input mode
    io_conf.pin_bit_mask = (1ULL << button_inc); // Increment button pin
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;   // No internal pull-up
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // No internal pull-down
    gpio_config(&io_conf);

    // Configure decrement button
    io_conf.pin_bit_mask = (1ULL << button_dec); // Decrement button pin
    gpio_config(&io_conf);

    // Install ISR service
    gpio_install_isr_service(0);

    // Attach interrupt handlers
    gpio_isr_handler_add(button_inc, button_isr_handler, (void*)button_inc);
    gpio_isr_handler_add(button_dec, button_isr_handler, (void*)button_dec);
}


void IRAM_ATTR button_isr_handler(void* arg) {
    
    int button = reinterpret_cast<int>(arg);

      if (button == button_inc) {
        power_state = !power_state; //toggle the power state
        //while(button!=button_inc);

      } else if (button == button_dec) {
        bell_state = !bell_state;   //toggle the bell state
        //while(button!=button_dec);
      }
      else{
        bell_state = false;
        power_state = false;
      }
        
}

void update_display() {

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 10, 20, bell_state ? "BELL:ON" : "BELL:OFF");
    u8g2_DrawStr(&u8g2, 10, 60, power_state ? "POWER:ON" : "POWER:OFF");
    u8g2_SendBuffer(&u8g2);
}

extern "C" void app_main(void) {
    init_i2c();
    i2c_displayConfig();
    gpio_init();

    // Main loop
    while (1) {
        update_display();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}





//Program to display count
//--------------------------------------------------------------

// #include <iostream>
// #include "u8g2.h"
// #include "u8g2_esp32_hal.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/i2c.h"
// #include "driver/gpio.h"
// #include <cstring>

// // I2C pins
// const gpio_num_t i2c_sda_pin = GPIO_NUM_22;
// const gpio_num_t i2c_scl_pin = GPIO_NUM_21;

// // Button GPIO pins
// const gpio_num_t button_inc = GPIO_NUM_18; // Increment button
// const gpio_num_t button_dec = GPIO_NUM_19; // Decrement button

// // U8g2 object
// u8g2_t u8g2;

// // Counter
// volatile int counter = 0;

// // Function prototypes
// void IRAM_ATTR button_isr_handler(void* arg);
// void update_display();

// void init_i2c() {
//     i2c_config_t conf;
//     memset(&conf, 0, sizeof(i2c_config_t));
//     conf.mode = I2C_MODE_MASTER;
//     conf.sda_io_num = i2c_sda_pin;
//     conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
//     conf.scl_io_num = i2c_scl_pin;
//     conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
//     conf.master.clk_speed = 400000;
//     ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_1, &conf));
//     ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_1, conf.mode, 0, 0, 0));
// }

// void i2c_displayConfig() {
//     u8g2_esp32_hal_t u8g2_hal = U8G2_ESP32_HAL_DEFAULT;
//     u8g2_hal.sda = U8G2_ESP32_HAL_UNDEFINED;
//     u8g2_hal.scl = U8G2_ESP32_HAL_UNDEFINED;
//     u8g2_esp32_hal_init(u8g2_hal);

//     u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, 
//         u8g2_esp32_i2c_byte_cb, 
//         u8g2_esp32_gpio_and_delay_cb);

//     u8g2_InitDisplay(&u8g2);
//     u8g2_SetPowerSave(&u8g2, 0);
// }

// void gpio_init() {
//     gpio_config_t io_conf;

//     // Configure increment button
//     io_conf.intr_type = GPIO_INTR_NEGEDGE;       // Interrupt on falling edge
//     io_conf.mode = GPIO_MODE_INPUT;             // Input mode
//     io_conf.pin_bit_mask = (1ULL << button_inc); // Increment button pin
//     io_conf.pull_up_en = GPIO_PULLUP_DISABLE;   // No internal pull-up
//     io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // No internal pull-down
//     gpio_config(&io_conf);

//     // Configure decrement button
//     io_conf.pin_bit_mask = (1ULL << button_dec); // Decrement button pin
//     gpio_config(&io_conf);

//     // Install ISR service
//     gpio_install_isr_service(0);

//     // Attach interrupt handlers
//     gpio_isr_handler_add(button_inc, button_isr_handler, (void*)button_inc);
//     gpio_isr_handler_add(button_dec, button_isr_handler, (void*)button_dec);
// }


// void IRAM_ATTR button_isr_handler(void* arg) {
    
//     int button = reinterpret_cast<int>(arg);

//       if (button == button_inc) {
//         counter = counter+1; // Increment counter
//       } else if (button == button_dec) {
//         counter=counter-1; // Decrement counter
//       }
      
   
// }



// void update_display() {
//     char buffer[16];
//     sprintf(buffer, "Counter: %d", counter);

//     u8g2_ClearBuffer(&u8g2);
//     u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
//     u8g2_DrawStr(&u8g2, 10, 30, buffer);
//     u8g2_SendBuffer(&u8g2);
// }

// extern "C" void app_main(void) {
//     init_i2c();
//     i2c_displayConfig();
//     gpio_init();

//     // Initial display
//     update_display();

//     // Main loop
//     while (1) {
//         update_display();
//         vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }



//--------------------------------------------
//Print a text on oled
/*
//initial i2c communication with oled & lolin d32

#include <iostream>
#include "u8g2.h"
#include "u8g2_esp32_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c.h"
#include "driver/gpio.h"
#include <cstring>

// I2C pins
const gpio_num_t i2c_sda_pin = GPIO_NUM_22;
const gpio_num_t i2c_scl_pin = GPIO_NUM_21;

// U8g2 object
u8g2_t u8g2;

void init_i2c() 
{
    i2c_config_t conf;
    memset(&conf, 0, sizeof(i2c_config_t));
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = i2c_sda_pin;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = i2c_scl_pin;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_1, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_1, conf.mode, 0, 0, 0));
}

void i2c_displayConfig()
{
    // Initialize U8g2 HAL
    u8g2_esp32_hal_t u8g2_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_hal.sda = U8G2_ESP32_HAL_UNDEFINED;
    u8g2_hal.scl = U8G2_ESP32_HAL_UNDEFINED;
    u8g2_esp32_hal_init(u8g2_hal);

    // Initialize U8g2 with HAL
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2,U8G2_R0, // Screen rotation
    u8g2_esp32_i2c_byte_cb, // ESP32 I2C callback
    u8g2_esp32_gpio_and_delay_cb // Delay callback
    );

    // Power up and initialize the display
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

   
}
extern "C" void app_main(void)
{
    init_i2c();
    i2c_displayConfig();
    
    
    // // Draw "Hello Love!"
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 35, 35, "Hello Love!");
    u8g2_SendBuffer(&u8g2);


   
   // Keep running
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  
   

}*/