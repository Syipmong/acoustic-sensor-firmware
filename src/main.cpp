#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <driver/i2s.h>

#include <telemetry.h>

/* Hardware Pins*/
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define I2S_WS 15
#define I2s_SD 13
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0

// Sampling Configuration
#define SAMPLE_RATE 16000
#define DMA_BUFF_COUNT 8
#define DMA_BUFF_LEN 1024

uint16_t current_sequence = 0;

void setupI2S()
{
    i2s_config_t 12s_config = {
        .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_RX).
        .sample
    }
}