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

#define BRIGHTNESS      255
#define COLOR_ORDER     GRB
#define LED_TYPE        WS2813
#define NUM_LEDS        150     // TODO will depend on strand
#define NUM_STRIPS      28
#define STRIPS_PER_SAIL 7

#define HEIGHT       900
#define RADIUS_X     100
#define STRIP_START  0.3
#define WIDTH        1100

#define VIZ_PRIDE       1
#define VIZ_WINDSHIELD  2
#define VIZ_RIPPLE      3
#define SET_BRIGHTNESS  4

#define SECONDS_PER_PALETTE  30

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
  uint8_t action;
  int value1;
  int value2;
} msg;

CRGB leds[NUM_STRIPS][NUM_LEDS];
msg data;

uint8_t boardNumber;
uint8_t offset;
uint8_t stripIndex = NUM_STRIPS;
uint8_t action;
uint8_t setBrightness = BRIGHTNESS;

int16_t current[NUM_STRIPS][NUM_LEDS];
int16_t previous[NUM_STRIPS][NUM_LEDS];

CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;

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
  offset = (boardNumber - 1) * STRIPS_PER_SAIL;

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

  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j < NUM_LEDS; j++) {
      current[i][j] = 0;
      previous[i][j] = 0;
    }
  }

  chooseNextColorPalette(gTargetPalette);
}

void set_all(CRGB color) {
  for (uint16_t i = 0; i < NUM_STRIPS; i++) {
    for (uint16_t j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = color;
    }
  }
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("Bytes received: ");
  Serial.println(len);

  action = data.action;
  if (action == VIZ_WINDSHIELD) {
    stripIndex = 0;
  } else if (action == VIZ_RIPPLE) {
    set_all(CRGB(0, 0, 0));
    Serial.print("RIPPLE: ");
    Serial.print(data.value1);
    Serial.print(" ");
    Serial.println(data.value2);
    current[(int)data.value1][(int)data.value2] = 500;
  } else if (action == SET_BRIGHTNESS) {
    Serial.print("SET_BRIGHTNESS: ");
    Serial.println(data.value1);
    setBrightness = (uint8_t)data.value1;
  }
}

int geti(int pixelnumber) {
  return pixelnumber / NUM_LEDS;
}

int getj(int pixelnumber) {
  return pixelnumber - (geti(pixelnumber) * NUM_LEDS);
}

void set_strip(uint8_t strip_number, CRGB color, uint8_t num_leds = NUM_LEDS) {
  for (uint16_t j = 0; j < num_leds; j++) {
    leds[strip_number][j] = color;
  }
}

float distance(XY p1, XY p2) {
  return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

auto scale(float domainStart, float domainEnd, float rangeStart, float rangeEnd) {
  return [=](float value) {
    float percentage = (value - domainStart) / (domainEnd - domainStart);
    return rangeStart + (rangeEnd - rangeStart) * percentage;
  };
}

uint8_t gHue = 0;
void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_STRIPS * NUM_LEDS; i++) { //9948
    leds[geti(i)][getj(i)] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  for (int i = 0; i < NUM_STRIPS; i++) {
    fadeToBlackBy( leds[i], NUM_LEDS, 20);
  }
  uint8_t dothue = 0;
  for( int i = 0; i < NUM_STRIPS; i++) {
    for( int j = 0; j < 8; j++) {
      leds[i][beatsin16( j+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
  }
}

void loop() {
  viz_pride();
  //viz_pacifica();

  //EVERY_N_SECONDS( SECONDS_PER_PALETTE ) {
  //  chooseNextColorPalette( gTargetPalette );
  //}
  //EVERY_N_MILLISECONDS( 10 ) {
  //  nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 12);
  //}
  //viz_twinkle();

  //EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //bpm();

  //if (stripIndex < NUM_STRIPS) {
  //  set_strip(stripIndex, CRGB::White);
  //  stripIndex += 1;
  //}

  //viz_ripple();

  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[i][j].nscale8(setBrightness);
    }
  }

  FastLED.show();
}
