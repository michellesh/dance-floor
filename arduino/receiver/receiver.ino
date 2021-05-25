#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "dance_floor_shared.h"
#include "globals.h"
#include "secrets.h"
#include "utils.h"
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

  uint8_t boardNumber = WiFi.macAddress() == macAddress1   ? 1
                        : WiFi.macAddress() == macAddress2 ? 2
                        : WiFi.macAddress() == macAddress3 ? 3
                                                           : 4;
  uint8_t offset = (boardNumber - 1) * STRIPS_PER_SAIL;

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
  init_starfield();
}

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("Bytes received: ");
  Serial.println(len);

  if (data.action == ACTION_WIPE) {
    wipeIndex = 0;
  } else if (data.action == ACTION_RIPPLE) {
    Serial.print("RIPPLE: ");
    Serial.print(data.value1);
    Serial.print(" ");
    Serial.println(data.value2);
    current[(int)data.value1][(int)data.value2] = 500;
  } else if (data.action == ACTION_SET_BRIGHTNESS) {
    Serial.print("ACTION_SET_BRIGHTNESS: ");
    Serial.println(data.value1);
    setBrightness = (uint8_t)data.value1;
  } else if (data.action == ACTION_SET_BG_BRIGHTNESS) {
    Serial.print("ACTION_SET_BG_BRIGHTNESS: ");
    Serial.println(data.value1);
    setBackgroundBrightness = (uint8_t)data.value1;
  } else if (data.action == ACTION_SET_BACKGROUND) {
    Serial.print("ACTION_SET_BACKGROUND");
    Serial.println(data.value1);
    activeViz = data.value1;
  } else if (data.action == ACTION_SET_PALETTE) {
    Serial.print("ACTION_SET_PALETTE ");
    Serial.println(data.value1);
    gCurrentPalette = *(ActivePaletteList[data.value1]);
    currentColor = colorList[data.value1];
  } else if (data.action == ACTION_CYCLE_PALETTE) {
    Serial.print("ACTION_CYCLE_PALETTE ");
    Serial.println(data.value1);
    gTargetPalette = *(ActivePaletteList[data.value1]);
  } else if (data.action == ACTION_SPEED) {
    Serial.print("ACTION_SPEED ");
    Serial.println(data.value1);
    speed = data.value1;
  } else if (data.action == ACTION_STROBE_ON) {
    Serial.print("ACTION_STROBE_ON");
    strobeOn = true;
  } else if (data.action == ACTION_STROBE_OFF) {
    Serial.print("ACTION_STROBE_OFF");
    strobeOn = false;
  } else if (data.action == ACTION_ECHO) {
    for (int i = 0; i < numEchos; i++) {
      if (echos[i] >= NUM_LEDS) {
        echos[i] = 0;
        break;
      }
    }
  }
}

void loop() {
  if (activeViz == VIZ_DEFAULT) {
    set_all(CRGB::Black, 0);
  } else if (activeViz == VIZ_PRIDE) {
    viz_pride();
  } else if (activeViz == VIZ_TWINKLE) {
    EVERY_N_MILLISECONDS(10) {
      nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, 12);
    }
    viz_twinkle(mapf(speed, 1, 10, 4, 9));
  } else if (activeViz == VIZ_PACIFICA) {
    viz_pacifica();
  } else if (activeViz == VIZ_STARFIELD) {
    viz_starfield(currentColor, mapf(speed, 1, 10, 0.5, 9));
  } else if (activeViz == VIZ_BPM) {
    EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
    bpm();
  } else if (activeViz == VIZ_JUGGLE) {
    viz_juggle();
  }

  // Background Brightness
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[i][j].nscale8(setBackgroundBrightness);
    }
  }

  // Wipe
  if (wipeIndex < NUM_STRIPS) {
    set_strip(wipeIndex, currentColor, setBrightness);
    wipeIndex += 1;
  }

  // Ripple
  viz_ripple(currentColor, setBrightness);

  // Strobe
  if (strobeOn) {
    set_all(currentColor, setBrightness);
  }

  // Echo
  for (int i = 0; i < numEchos; i++) {
    if (echos[i] < NUM_LEDS) {
      set_index(echos[i], currentColor, setBrightness);
      echos[i] += 5;
    }
  }

  FastLED.show();
}
