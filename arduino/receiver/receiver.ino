#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <espnow.h>

#define DATA_PIN    2
#define LED_TYPE    WS2813
#define COLOR_ORDER GRB
#define NUM_LEDS    5
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];

// Structure example to receive data
// Must match the sender structure
typedef struct led {
  uint8_t x;
  uint8_t y;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} led;

// Create a struct_message called myData
led myData;

int boardNumber;
int x = -1;
int y = -1;
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(500);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(LED_BUILTIN, OUTPUT);

  boardNumber = WiFi.macAddress() == macAddress1   ? 1
                : WiFi.macAddress() == macAddress2 ? 2
                : WiFi.macAddress() == macAddress3 ? 3
                                                   : 4;

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.setBrightness(BRIGHTNESS);
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  Serial.print("x: ");
  Serial.print(myData.x);
  Serial.print(" y: ");
  Serial.print(myData.y);
  Serial.print(" R:");
  Serial.print(myData.r);
  Serial.print(" G:");
  Serial.print(myData.g);
  Serial.print(" B:");
  Serial.print(myData.b);
  Serial.print(" a:");
  Serial.print(myData.a);
  Serial.println();

  x = myData.x;
  y = myData.y;
  r = myData.r;
  g = myData.g;
  b = myData.b;
}

void set_all_black() {
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0,0,0);
  }
}

void loop() {
  set_all_black();
  if (x == boardNumber) {
    leds[y] = CRGB(r, g, b);
  }
  FastLED.show();
}
