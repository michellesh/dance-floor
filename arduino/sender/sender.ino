#include "secrets.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#define VIZ_DEFAULT     0
#define VIZ_WINDSHIELD  1

typedef struct msg {
  uint8_t viz;
} msg;

msg test;

byte buttonPin = 0;
bool buttonDown = false;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress4, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  pinMode(buttonPin, INPUT_PULLUP);
}

bool isButtonPressed() {
  return digitalRead(buttonPin) == 0;
}

void loop() {
  if (isButtonPressed()) {
    if (!buttonDown) {
      buttonDown = true;

      test.viz = VIZ_WINDSHIELD;

      esp_now_send(0, (uint8_t *) &test, sizeof(test));
    }
  } else {
    buttonDown = false;
  }
  /*
  if ((millis() - lastTime) > timerDelay) {

    test.x = random(14, 16); // strand number (for board 3 temporarily)
    test.y = random(1, 5);   // which LED on strand
    test.r = random(0, 255);
    test.g = random(0, 255);
    test.b = random(0, 255);
    test.a = random(0, 255);

    esp_now_send(0, (uint8_t *) &test, sizeof(test));

    lastTime = millis();
  }
  */
}
