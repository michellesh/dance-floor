#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <espnow.h>

#define DATA_PIN_2  2
#define DATA_PIN_3  3
#define DATA_PIN_4  4
#define DATA_PIN_5  5
#define DATA_PIN_6  6
#define DATA_PIN_7  7
#define DATA_PIN_8  8

#define BRIGHTNESS   255
#define COLOR_ORDER  GRB
#define LED_TYPE     WS2813
#define NUM_LEDS     150     // TODO will depend on strand
#define NUM_PINS     7
#define NUM_STRIPS   28

#define HEIGHT       900
#define NUM_STRANDS  7
#define RADIUS_X     100
#define STRIP_START  0.3
#define WIDTH        1100

#define VIZ_DEFAULT     0
#define VIZ_PRIDE       1
#define VIZ_WINDSHIELD  2

typedef struct XY {
  float x;
  float y;
} XY;

struct Sail {
  XY p1;
  XY p2;
  XY p3;
};

// Structure example to receive data
// Must match the sender structure
typedef struct msg {
  uint8_t viz;
} msg;

CRGB leds[NUM_STRIPS][NUM_LEDS];
msg myData;

uint8_t boardNumber;
uint8_t offset;
uint8_t stripIndex = NUM_STRIPS;
uint8_t viz = VIZ_DEFAULT;

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  boardNumber = WiFi.macAddress() == macAddress1   ? 1
                : WiFi.macAddress() == macAddress2 ? 2
                : WiFi.macAddress() == macAddress3 ? 3
                                                   : 4;
  offset = (boardNumber - 1) * NUM_PINS;

  FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[offset], NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds[offset + 1], NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.addLeds<LED_TYPE, DATA_PIN_4, COLOR_ORDER>(leds[offset + 2], NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[offset + 3], NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[offset + 4], NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.addLeds<LED_TYPE, DATA_PIN_7, COLOR_ORDER>(leds[offset + 5], NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.addLeds<LED_TYPE, DATA_PIN_8, COLOR_ORDER>(leds[offset + 6], NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  viz = myData.viz;
  if (viz == VIZ_WINDSHIELD) {
    stripIndex = 0;
  }
}

void set_all(CRGB color, uint8_t num_leds = NUM_LEDS) {
  for (uint16_t i = 0; i < NUM_STRIPS; i++) {
    for (uint16_t j = 0; j < num_leds; j++) {
      leds[i][j] = color;
    }
  }
}

void set_strip(uint8_t strip_number, CRGB color, uint8_t num_leds = NUM_LEDS) {
  for (uint16_t j = 0; j < num_leds; j++) {
    leds[strip_number][j] = color;
  }
}

void loop() {
  viz_pride();

  if (stripIndex < NUM_STRIPS) {
    set_strip(stripIndex, CRGB::White);
    stripIndex += 1;
  }

  FastLED.show();

  Serial.println(ledxy(24, 111).x);
  Serial.println(ledxy(24, 111).y);
  delay(2000);
}
