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
