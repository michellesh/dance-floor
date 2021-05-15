#include "secrets.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#define SLIDER_1     16 // D0
#define SLIDER_2     5  // D1
#define SLIDER_3     4  // D2
#define SLIDER_4     0  // D3

#define RED_BUTTON     2  // D4
#define BLUE_BUTTON    14 // D5
#define YELLOW_BUTTON  12 // D6
#define GREEN_BUTTON   13 // D7

#define NUM_LEDS        150     // TODO will depend on strand
#define NUM_STRIPS      28
#define STRIPS_PER_SAIL 7

#define ACTION_CYCLE_BACKGROUND  1
#define ACTION_RIPPLE            2
#define ACTION_SET_BRIGHTNESS    3
#define ACTION_WINDSHIELD        4

typedef struct msg {
  uint8_t action;
  int value1;
  int value2;
} msg;

struct Button {
  int pin;
  bool pressed;
};

struct Slider {
  int pin;
  int value;
  int prev;
};

msg ripple = {ACTION_RIPPLE};
msg brightness = {ACTION_SET_BRIGHTNESS};
msg background = {ACTION_CYCLE_BACKGROUND};

Button redButton = {RED_BUTTON, false};
Button blueButton = {BLUE_BUTTON, false};
Button yellowButton = {YELLOW_BUTTON, false};
Button greenButton = {GREEN_BUTTON, false};

Slider slider1 = {SLIDER_1};
Slider slider2 = {SLIDER_2};
Slider slider3 = {SLIDER_3};
Slider slider4 = {SLIDER_4};

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

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(SLIDER_1, OUTPUT);
  pinMode(SLIDER_2, OUTPUT);
  pinMode(SLIDER_3, OUTPUT);
  pinMode(SLIDER_4, OUTPUT);
  pinMode(A0, INPUT);
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

bool isButtonPressed(Button button) {
  return digitalRead(button.pin) == 0;
}

int getSliderValue(Slider slider) {
  digitalWrite(slider.pin, HIGH);
  delay(100);
  int sliderValue = analogRead(0);
  digitalWrite(slider.pin, LOW);
  return sliderValue;
}

bool sliderValueChanged(Slider slider) {
  int BUFFER = 20;
  return slider.value < (slider.prev - BUFFER) || slider.value > (slider.prev + BUFFER);
}

void send(msg m) {
  esp_now_send(0, (uint8_t *) &m, sizeof(m));
}

void loop() {
  slider1.value = getSliderValue(slider1);
  if (sliderValueChanged(slider1)) {
    Serial.print("Slider 1 changed ");
    Serial.println(slider1.value);
    brightness.value1 = sliderToBrightness(slider1.value);
    Serial.print("Brightness ");
    Serial.println(brightness.value1);
    send(brightness);
    slider1.prev = slider1.value;
  }

  if (isButtonPressed(yellowButton)) {
    if (!yellowButton.pressed) {
      Serial.println("Yellow button pressed");
      yellowButton.pressed = true;
      send(background);
    }
  } else {
    yellowButton.pressed = false;
  }

  if (isButtonPressed(blueButton)) {
    if (!blueButton.pressed) {
      Serial.println("Blue button pressed");
      blueButton.pressed = true;

      ripple.value1 = random(1, NUM_STRIPS - 1);
      ripple.value2 = random(1, NUM_LEDS - 1);

      send(ripple);
    }
  } else {
    blueButton.pressed = false;
  }
}
