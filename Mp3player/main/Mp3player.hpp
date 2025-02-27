//Buttons & LED definitions
static const gpio_num_t BUTTON_1 = GPIO_NUM_21;
static const gpio_num_t BUTTON_2 = GPIO_NUM_19;
static const gpio_num_t BUTTON_3 = GPIO_NUM_34;
static const gpio_num_t BUTTON_4 = GPIO_NUM_39;
static const gpio_num_t LED_BUILTIN = GPIO_NUM_5;

#define LONG_PRESS_THRESHOLD 1000  // Time threshold in milliseconds for long press



static const char *TAG = "DFPLAYER";

#define UART_NUM UART_NUM_0  // Using UART0
#define TX_PIN 1             // ESP32 TX (Connect to DFPlayer RX)
#define RX_PIN 3             // ESP32 RX (Connect to DFPlayer TX)
#define BUF_SIZE 1024

// Function to calculate checksum
uint16_t calculate_checksum(uint8_t cmd, uint8_t param1, uint8_t param2) {
    uint16_t sum = 0xFF + 0x06 + cmd + 0x00 + param1 + param2;
    return 0xFFFF - sum + 1;  // Two’s complement
}

// Function to send commands to DFPlayer
void send_command(uint8_t cmd, uint8_t param1, uint8_t param2) {
    uint16_t checksum = calculate_checksum(cmd, param1, param2);
    uint8_t command[] = {
        0x7E, 0xFF, 0x06, cmd, 0x00, param1, param2,
        (uint8_t)((checksum >> 8) & 0xFF),  // High byte (cast to uint8_t)
        (uint8_t)(checksum & 0xFF),         // Low byte (cast to uint8_t)
        0xEF
    };

    uart_write_bytes(UART_NUM, (const char *)command, sizeof(command));
    ESP_LOGI(TAG, "Sent command: 0x%02X", cmd);
}


// Initialize UART
void init_uart() {
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
}

// Function to play the first MP3 file
void specifyPlay_mp3(uint8_t folder, uint8_t number) {
    send_command(0x03, folder, number);  // Play
}

void next_mp3()
{
    send_command(0x01, 0x00, 0x00);         // Play next track
}

// Function to set volume (0-30)
void set_volume(uint8_t volume) {
    send_command(0x06, 0x00, volume);
}

void playback()
{
    send_command(0x0D, 0x00, 0x00);
}

void pause_mp3()
{
    send_command(0x0E, 0x00, 0x00);
}

void prev_mp3()
{
    send_command(0x02, 0x00, 0x00);
}

void volumePlus()
{
    send_command(0x04, 0x00, 0x00);
}

void volumeMinus()
{
    send_command(0x05, 0x00, 0x00);
}

void gpio_init() {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pin_bit_mask = (1ULL << BUTTON_1) | (1ULL << BUTTON_2) | (1ULL << BUTTON_3) | (1ULL << BUTTON_4);
    gpio_config(&io_conf);

    gpio_config_t led_conf = {};
    led_conf.intr_type = GPIO_INTR_DISABLE;
    led_conf.mode = GPIO_MODE_OUTPUT;
    led_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    led_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    led_conf.pin_bit_mask = (1ULL << LED_BUILTIN);
    gpio_config(&led_conf);

    gpio_set_level(LED_BUILTIN, 1);
}
