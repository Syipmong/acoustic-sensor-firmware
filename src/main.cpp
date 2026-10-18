#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <driver/i2s.h>

#include <telemetry.h>

// Tensor Arena  allocation
const int kTensorArenaSize = 40 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;

/* Hardware Pins*/
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define I2S_WS 15
#define I2S_SD 13
#define I2S_SCK 2
#define I2S_PORT I2S_NUM_0

// Sampling Configuration
#define SAMPLE_RATE 16000
#define DMA_BUFF_COUNT 8
#define DMA_BUFF_LEN 1024

uint16_t current_sequence = 0;

void setupI2S()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = DMA_BUFF_COUNT,
        .dma_buf_len = DMA_BUFF_LEN,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_PORT, &pin_config);
};

void setupLoRa()
{
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);


    if (!LoRa.begin(868E6))
    {
        Serial.println("Lora Initialization failed!");
        while (true)
        {
            delay(1000);
        }
    }
    LoRa.setSpreadingFactor(7);
    Lora.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
}

void transmitAlert(uint8_t threat, uint8_t conf)
{
    TelemetryPayload payload;
    payload.node_id = 101;
    payload.epoch_time = 1727626980;
    payload.sequence_num = current_sequence++;
    payload.latitude = 9.9312;
    payload.longitude = 8.8853;
    payload.threat_class = threat;
    payload.confidence = conf;
    payload.battery_volts = 3.7;

    memset(payload.signature, 0, 64);

    LoRa.beginPacket();
    LoRa.write((uint8_t*) &payload, sizeof(TelemetryPayload));
    LoRa.endPacket();
}

void setup()
{
    Serial.begin(115200);
    setupI2S();
    setupLoRa();
}

void loop()
{
    size_t bytesIn = 0;
    int32_t sampleBuffer[DMA_BUFF_LEN];
    

    esp_err_t result = i2s_read(I2S_PORT, &sampleBuffer, sizeof(sampleBuffer), &bytesIn, portMAX_DELAY);

    if (result == ESP_OK && bytesIn > 0)
    {
        //Tflite Micro invocation passes  the PCM buffer to the model arena here
        bool threatDetected = false;

        if (threatDetected)
        {
            transmitAlert(1, 95);
            delay(5000);
        }
    }
}