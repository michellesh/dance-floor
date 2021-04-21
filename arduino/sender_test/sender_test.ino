/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-one-to-many-esp8266-nodemcu/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to send data
// Must match the receiver structure
typedef struct test_struct {
    int x;
    int y;
} test_struct;

// Create a struct_message called test to store variables to be sent
test_struct test;

byte buttonPin = 0;
bool buttonDown = false;

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Register peers
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  pinMode(buttonPin, INPUT_PULLUP);
}

bool isButtonPressed() {
  return digitalRead(buttonPin) == 0;
}

void loop() {
  if (isButtonPressed()) {
    if (!buttonDown) {
      buttonDown = true;

      // Set values to send
      test.x = random(1, 50);
      test.y = random(1, 50);

      // Send message via ESP-NOW
      esp_now_send(0, (uint8_t *) &test, sizeof(test));
    }
  } else {
    buttonDown = false;
  }
}
