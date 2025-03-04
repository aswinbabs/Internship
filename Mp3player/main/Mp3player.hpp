//Mp3Player.hpp

//Buttons & LED definitions
static const gpio_num_t BUTTON_1 = GPIO_NUM_21;
static const gpio_num_t BUTTON_2 = GPIO_NUM_19;
static const gpio_num_t BUTTON_3 = GPIO_NUM_34;
static const gpio_num_t BUTTON_4 = GPIO_NUM_39;
static const gpio_num_t LED_BUILTIN = GPIO_NUM_5;

#define LONG_PRESS_THRESHOLD 1000  // Time threshold in milliseconds for long press

static const char *TAG = "DFPLAYER";

#define UART_NUM UART_NUM_2  // Using UART0
#define TX_PIN 32             // ESP32 TX (Connect to DFPlayer RX)
#define RX_PIN 33             // ESP32 RX (Connect to DFPlayer TX)
#define BUF_SIZE 1024

// DFPlayer command definitions
#define CMD_NEXT             0x01
#define CMD_PREV             0x02
#define CMD_PLAY_TRACK       0x03
#define CMD_VOLUME_UP        0x04
#define CMD_VOLUME_DOWN      0x05
#define CMD_SET_VOLUME       0x06
#define CMD_PLAY             0x0D
#define CMD_PAUSE            0x0E
#define CMD_PLAY_FOLDER      0x0F
#define CMD_GET_STATUS       0x42
#define CMD_GET_VOLUME       0x43
#define CMD_GET_TOTAL_FILES  0x48
#define CMD_GET_CURRENT_TRACK 0x4C
#define CMD_GET_FOLDERS_COUNT 0x4E
#define CMD_GET_TRACKS_IN_FOLDER 0x4F

// Function to calculate checksum
uint16_t calculate_checksum(uint8_t cmd, uint8_t param1, uint8_t param2, uint8_t feedback = 0x00) {
    uint16_t sum = 0xFF + 0x06 + cmd + feedback + param1 + param2;
    return 0xFFFF - sum + 1;  // Two's complement
}

// Function to process responses from DFPlayer
void process_response(uint8_t cmd_sent) {
    uint8_t buffer[10];
    int bytes_read = uart_read_bytes(UART_NUM, buffer, 10, pdMS_TO_TICKS(200));
    
    if (bytes_read == 10 && buffer[0] == 0x7E && buffer[9] == 0xEF) {
        uint16_t param_value = (buffer[5] << 8) | buffer[6];
        
        // Handle the response based on the original command
        switch (cmd_sent) {
            case CMD_GET_STATUS:
                ESP_LOGI(TAG, "Player status: %d", param_value);
                break;
            case CMD_GET_VOLUME:
                ESP_LOGI(TAG, "Current volume: %d", param_value);
                break;
            case CMD_GET_TOTAL_FILES:
                ESP_LOGI(TAG, "Total files: %d", param_value);
                break;
            case CMD_GET_CURRENT_TRACK:
                ESP_LOGI(TAG, "Current track: %d", param_value);
                break;
            case CMD_GET_FOLDERS_COUNT:
                ESP_LOGI(TAG, "Number of folders: %d", param_value);
                break;
            case CMD_GET_TRACKS_IN_FOLDER:
                ESP_LOGI(TAG, "Tracks in folder: %d", param_value);
                break;
            default:
                ESP_LOGI(TAG, "Response for command 0x%02X: %d", cmd_sent, param_value);
                break;
        }
    } else if (bytes_read > 0) {
        ESP_LOGW(TAG, "Received invalid response (%d bytes)", bytes_read);
    } else {
        ESP_LOGW(TAG, "No response received within timeout");
    }
}

// Function to send commands to DFPlayer with optional feedback
void send_command(uint8_t cmd, uint8_t param1, uint8_t param2, bool request_feedback = false) {
    // Clear any pending data in receive buffer
    uint8_t dummy[BUF_SIZE];
    uart_read_bytes(UART_NUM, dummy, BUF_SIZE, 0);
    
    uint8_t feedback = request_feedback ? 0x01 : 0x00;
    uint16_t checksum = calculate_checksum(cmd, param1, param2, feedback);
    uint8_t command[] = {
        0x7E, 0xFF, 0x06, cmd, feedback, param1, param2,
        (uint8_t)((checksum >> 8) & 0xFF),  // High byte
        (uint8_t)(checksum & 0xFF),         // Low byte
        0xEF
    };

    uart_write_bytes(UART_NUM, (const char *)command, sizeof(command));
    ESP_LOGI(TAG, "Sent command: 0x%02X with feedback=%d", cmd, feedback);
    
    // If feedback was requested, wait for and process response
    if (request_feedback) {
        vTaskDelay(pdMS_TO_TICKS(50));  // Small delay to allow DFPlayer to respond
        process_response(cmd);
    }
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
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));

    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 0, NULL, 0);
}

// Function to play MP3 file based on number
void specifyFileNum(uint8_t number, bool get_feedback = false) {
    send_command(CMD_PLAY_TRACK, 0x00, number, get_feedback);
}

// Function to play MP3 file in a specific folder
void specifyPlay_mp3(uint8_t folder, uint8_t number, bool get_feedback = false) {
    send_command(CMD_PLAY_FOLDER, folder, number, get_feedback);
}

// Function to play next track
void next_mp3(bool get_feedback = false) {
    send_command(CMD_NEXT, 0x00, 0x00, get_feedback);
}

// Function to set volume (0-30)
void set_volume(uint8_t volume, bool get_feedback = false) {
    if(volume > 30) volume = 30;
    send_command(CMD_SET_VOLUME, 0x00, volume, get_feedback);
}

// Function to resume playback
void playback(bool get_feedback = false) {
    send_command(CMD_PLAY, 0x00, 0x00, get_feedback);
}

// Function to pause playback
void pause_mp3(bool get_feedback = false) {
    send_command(CMD_PAUSE, 0x00, 0x00, get_feedback);
}

// Function to play previous track
void prev_mp3(bool get_feedback = false) {
    send_command(CMD_PREV, 0x00, 0x00, get_feedback);
}

// Function to increase volume
void volumePlus(bool get_feedback = false) {
    send_command(CMD_VOLUME_UP, 0x00, 0x00, get_feedback);
}

// Function to decrease volume
void volumeMinus(bool get_feedback = false) {
    send_command(CMD_VOLUME_DOWN, 0x00, 0x00, get_feedback);
}

// Status query functions

// Get player status
void get_status() {
    send_command(CMD_GET_STATUS, 0x00, 0x00, true);
}

// Get current volume
void get_volume() {
    send_command(CMD_GET_VOLUME, 0x00, 0x00, true);
}

// Get total number of files
void get_total_files() {
    send_command(CMD_GET_TOTAL_FILES, 0x00, 0x00, true);
}

// Get current track number
void get_current_track() {
    send_command(CMD_GET_CURRENT_TRACK, 0x00, 0x00, true);
}

// Get number of folders
void get_folders_count() {
    send_command(CMD_GET_FOLDERS_COUNT, 0x00, 0x00, true);
}

// Get number of tracks in a specific folder
void get_tracks_in_folder(uint8_t folder) {
    send_command(CMD_GET_TRACKS_IN_FOLDER, 0x00, folder, true);
}

// Initialize GPIO
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

// Function to display all status information
void display_player_status() {
    ESP_LOGI(TAG, "--- DFPlayer Status Information ---");
    get_status();
    vTaskDelay(pdMS_TO_TICKS(100));
    get_volume();
    vTaskDelay(pdMS_TO_TICKS(100));
    get_current_track();
    vTaskDelay(pdMS_TO_TICKS(100));
    get_total_files();
    vTaskDelay(pdMS_TO_TICKS(100));
    get_folders_count();
    ESP_LOGI(TAG, "--------------------------------");
}




/*
//Mp3Player.hpp , The version has only Tx , no responses from DFmini

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
    uart_driver_install(UART_NUM, BUF_SIZE, 0, 0, NULL, 0);
}

// Function to play the first MP3 file
void specifyPlay_mp3(uint8_t folder, uint8_t number) {
    send_command(0x0F, folder, number);  // Play
}

void next_mp3()
{
    send_command(0x01, 0x00, 0x00);         // Play next track
}

// Function to set volume (0-30)
void set_volume(uint8_t volume) {

    if(volume > 30 ) volume = 30;
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
*/