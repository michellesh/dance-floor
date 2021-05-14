#include "secrets.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#define RED_BUTTON   4   // D2
#define WHITE_BUTTON 5   // D1
#define SLIDER_1     13  // D7
#define SLIDER_2     15  // D8

#define NUM_LEDS        150     // TODO will depend on strand
#define NUM_STRIPS      28
#define STRIPS_PER_SAIL 7

#define VIZ_PRIDE       1
#define VIZ_WINDSHIELD  2
#define VIZ_RIPPLE      3
#define SET_BRIGHTNESS  4

typedef struct msg {
  uint8_t action;
  int value1;
  int value2;
} msg;

msg ripple = {VIZ_RIPPLE};
msg brightness = {SET_BRIGHTNESS};

byte buttonPin = 0;
bool buttonDown = false;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

int redButton;
int whiteButton;
int sliderValue1, prevSlider1;
int sliderValue2, prevSlider2;

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
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(WHITE_BUTTON, INPUT_PULLUP);
  pinMode(SLIDER_1, OUTPUT);
  //pinMode(SLIDER_2, OUTPUT);
  pinMode(A0, INPUT);
}

bool isButtonPressed() {
  return digitalRead(buttonPin) == 0;
}

int getSliderValue(int sliderPin) {
  digitalWrite(sliderPin, HIGH);
  delay(100);
  int sliderValue = analogRead(0);
  digitalWrite(sliderPin, LOW);
  return sliderValue;
}

auto scale(float domainStart, float domainEnd, float rangeStart, float rangeEnd, bool clamp = false) {
  return [=](float value) {
    float percentage = (value - domainStart) / (domainEnd - domainStart);
    float newValue = rangeStart + (rangeEnd - rangeStart) * percentage;
    return clamp && newValue < min(rangeStart, rangeEnd)   ? min(rangeStart, rangeEnd)
           : clamp && newValue > max(rangeStart, rangeEnd) ? max(rangeStart, rangeEnd)
                                                           : newValue;
  };
}

auto sliderToBrightness = scale(1024, 3, 0, 255, true);

bool sliderValueChanged(int value, int prev) {
  int BUFFER = 10;
  return value < (prev - BUFFER) || value > (prev + BUFFER);
}

void send(msg m) {
  esp_now_send(0, (uint8_t *) &m, sizeof(m));
}

void loop() {
  sliderValue1 = getSliderValue(SLIDER_1);
  if (sliderValueChanged(sliderValue1, prevSlider1)) {
    brightness.value1 = sliderToBrightness(sliderValue1);
    send(brightness);
    prevSlider1 = sliderValue1;
  }

  if (isButtonPressed()) {
    if (!buttonDown) {
      buttonDown = true;

      ripple.value1 = random(1, NUM_STRIPS - 1);
      ripple.value2 = random(1, NUM_LEDS - 1);

      send(ripple);
    }
  } else {
    buttonDown = false;
  }
}
