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
