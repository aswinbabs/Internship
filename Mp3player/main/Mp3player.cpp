

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h" 
#include "driver/gpio.h"
#include "esp_log.h"
#include <stdio.h>
#include <iostream>
#include "Mp3player.hpp"



extern "C" void app_main()
{
    gpio_init();
    ESP_LOGI(TAG, "Initializing DFPlayer Mini...");
    init_uart();
       

    int prev_button1 = 1;
    int prev_button2 = 1;
    int prev_button3 = 1;
    int prev_button4 = 1;
    bool play = false;
    
    // Variables to track press durations
    TickType_t button1_press_start = 0;
    TickType_t button2_press_start = 0;
    TickType_t button3_press_start = 0;
    TickType_t button4_press_start = 0;

    while (1) 
    {
        int button1 = gpio_get_level(BUTTON_1);
        int button2 = gpio_get_level(BUTTON_2);
        int button3 = gpio_get_level(BUTTON_3);
        int button4 = gpio_get_level(BUTTON_4);

        // Button 1: Handle press and long press
        if (button1 == 0 && prev_button1 == 1)
        {   
            button1_press_start = xTaskGetTickCount();
            ESP_LOGD(TAG, "Button 1 : Next ");
        } 
        if (button1 == 1 && prev_button1 == 0) {
            
            TickType_t press_duration = xTaskGetTickCount() - button1_press_start;
            if (press_duration < pdMS_TO_TICKS(LONG_PRESS_THRESHOLD)) {
                
                next_mp3();  
                ESP_LOGD(TAG, "Playing Next track...");

            } 
            else 
            {
                while(!(button1 == 1 && prev_button1 == 0))
                {
                    volumePlus();
                }
            }
        }
        prev_button1 = button1;

        if (button2 == 0 && prev_button2 == 1) {
            play = !play;
            ESP_LOGD(TAG, "Play / Pause : %s", play ? "PLAY" : "PAUSE");
        } 
        if (button2 == 1 && prev_button2 == 0)
        {
            if (play)
            {
                playback();
            }
            else
            {
                pause_mp3();
            }
        }
        prev_button2 = button2;

        // Button 3: Handle press and long press
        if (button3 == 0 && prev_button3 == 1) {
            button3_press_start = xTaskGetTickCount();
            ESP_LOGI(TAG, "Button 3 pressed");
        } 
        if (button3 == 1 && prev_button3 == 0) {
            TickType_t press_duration = xTaskGetTickCount() - button3_press_start;
            if (press_duration < pdMS_TO_TICKS(LONG_PRESS_THRESHOLD)) {
                prev_mp3();
                ESP_LOGD(TAG, "Playing previous track....");
            } else {
                while(!(button3 == 1 && prev_button3 == 0))
                {
                    volumeMinus();
                }
            }
        }
        prev_button3 = button3;

        // Button 4: Handle press and long press
        if (button4 == 0 && prev_button4 == 1) 
        {
           specifyPlay_mp3(0x02, 0x03);   // folder / name
        } 
        prev_button4 = button4;

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


//////////////////////////////////////////



/*

// LED ON / OFF USING COMMAND FROM UART
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"

static const char *TAG = "uart_led_control";

// UART configuration
#define UART_NUM UART_NUM_0
#define BUF_SIZE 1024
#define TXD_PIN 1  // Default UART TX pin on ESP32
#define RXD_PIN 3  // Default UART RX pin on ESP32

// LED configuration (using onboard LED for Lolin D32)
#define LED_PIN GPIO_NUM_5  // Lolin D32 onboard LED

// Function to initialize UART
static void uart_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    
    // Install UART driver
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

// Function to initialize LED
static void led_init(void) {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0); // Start with LED off
}

// Function to send response over UART
static void uart_send_response(const char* response) {
    uart_write_bytes(UART_NUM, response, strlen(response));
    uart_write_bytes(UART_NUM, "\r\n", 2);
}

extern "C" void app_main(void) {
    // Initialize components
    uart_init();
    led_init();
    
    ESP_LOGI(TAG, "UART LED control initialized");
    uart_send_response("ESP32 UART LED Control Ready");
    uart_send_response("Commands: 'ON' to turn LED on, 'OFF' to turn LED off");
    
    // Buffer for UART data
    uint8_t data[BUF_SIZE];
    
    while (1) {
        // Read data from UART
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, pdMS_TO_TICKS(100));
        
        if (len > 0) {
            // Null-terminate the received data
            data[len] = '\0';
            ESP_LOGI(TAG, "Received command: %s", data);
            
            // Process the command
            if (strncmp((char*)data, "ON", 2) == 0) {
                gpio_set_level(LED_PIN, 1);
                ESP_LOGI(TAG, "LED turned ON");
                uart_send_response("LED is now ON");
            } 
            else if (strncmp((char*)data, "OFF", 3) == 0) {
                gpio_set_level(LED_PIN, 0);
                ESP_LOGI(TAG, "LED turned OFF");
                uart_send_response("LED is now OFF");
            } 
            else {
                ESP_LOGW(TAG, "Unknown command");
                uart_send_response("Unknown command. Use 'ON' or 'OFF'");
            }
        }
        
        // Short delay to prevent CPU hogging
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}




//////////////////////////////////////////////






#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "stdio.h"

// Define UART settings
#define UART_PORT_NUM UART_NUM_0
#define UART_BAUD_RATE 9600
#define UART_BUF_SIZE 1024

// Define SD card paths
#define FOLDER_01 "/01/"
#define FOLDER_02 "/02/"

// Declare the current track and folder
uint8_t current_folder = 1; // 1 for folder 01, 2 for folder 02
uint8_t current_track = 1;  // Track number (1-3)

// To initialize UART
void uart_init() {
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,  // Correct enum for 8 data bits
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,  // Correct stop bits enum
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    // Configure UART2
    uart_param_config(UART_PORT_NUM, &uart_config);

    // Set the TX and RX pins for UART2 (GPIO17 and GPIO16)
    uart_set_pin(UART_PORT_NUM, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);  // GPIO17 for TX, GPIO16 for RX

    // Install UART driver for UART2
    uart_driver_install(UART_PORT_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0);  // 2x buffer size for receiving data
}

// Function to send UART command
void send_uart_command(uint8_t *cmd, size_t length) {
    uart_write_bytes(UART_PORT_NUM, (const char *)cmd, length);
}

// // Function to read command
// void read_uart_command() {
//     uint8_t data[128];  // Buffer for reading data from UART
//     int length = uart_read_bytes(UART_PORT_NUM, data, sizeof(data), pdMS_TO_TICKS(1000));  // Read with 1-second timeout

//     if (length > 0) {
//         // Successfully read data, print or check the received response
//         printf("Received %d bytes: ", length);
//         for (int i = 0; i < length; i++) {
//             printf("0x%02X ", data[i]);
//         }
//         printf("\n");

//         // You can further process the response here if needed.
//     } else {
//         // Timeout or no response received
//         printf("No response from MP3 module\n");
//     }
// }

// // Function to play a specific track
// void play_track(uint8_t track_number) {
//     // Command format: {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, track_number, 0xEF}
//     uint8_t cmd[8] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, track_number, 0xEF};

//     // Send the play track command
//     send_uart_command(cmd, sizeof(cmd));
//     printf("Playing track: %03d.mp3 from folder %d\n", track_number, current_folder);
    
//     // Optionally, read the response from the MP3 module to confirm it played
//     read_uart_command();
// }

// // Function to control the volume
// void set_volume(uint8_t volume_level) {
//     if (volume_level > 30) volume_level = 30;  // Limit volume to max 30

//     uint8_t cmd[10] = {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, volume_level, 0x00, 0x00, 0xEF};

//     // Send the volume control command
//     send_uart_command(cmd, sizeof(cmd));
//     printf("Setting volume to %d\n", volume_level);
// }

// // Function to print the current folder and track details
// void print_current_track() {
//     printf("Currently playing folder %d, track %03d.mp3\n", current_folder, current_track);
// }

// Main function to initialize everything and run commands
extern "C" void app_main() {
    // Initialize UART
    uart_init();

    // // Play track 001.mp3 from folder 01
    // current_folder = 1;
    // current_track = 1;

    // set_volume(30);  // Set the volume first
    // play_track(current_track);  // Play the track
    //uint8_t cmd[10] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x01, 0xFF, 0xD5, 0xEF};
    //send_uart_command(cmd,sizeof(cmd));

    uint8_t cmd[10] = {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x01, 0x00, 0x00, 0xEF};
    send_uart_command(cmd,sizeof(cmd));

    vTaskDelay(5000 / portTICK_PERIOD_MS);  // Wait for 5 seconds to simulate playback

    
}



*/