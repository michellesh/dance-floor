#include "dance_floor_shared.h"
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <espnow.h>

#define SLIDER_1       15  // D8
#define SLIDER_2       5   // D1
#define SLIDER_3       16  // D0
#define SLIDER_4       0   // D3

#define RED_BUTTON     2   // D4
#define BLUE_BUTTON    14  // D5
#define YELLOW_BUTTON  12  // D6
#define GREEN_BUTTON   13  // D7
#define WHITE_BUTTON   4   // D2

#define BACKGROUND_MODE  0
#define OVERLAY_MODE     1

int backgrounds[] = {VIZ_PRIDE, VIZ_TWINKLE, VIZ_STARFIELD, VIZ_JUGGLE};
int activeVizIndex = 0;
int activePalette = 0;
int numPalettes = 9;
int mode = BACKGROUND_MODE;

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
msg background = {ACTION_SET_BACKGROUND, VIZ_DEFAULT};
msg blendPalette = {ACTION_CYCLE_PALETTE};
msg palette = {ACTION_SET_PALETTE};
msg wipe = {ACTION_WIPE};
msg speed = {ACTION_SPEED};
msg strobe = {ACTION_STROBE};

Button redButton = {RED_BUTTON, false};
Button blueButton = {BLUE_BUTTON, false};
Button yellowButton = {YELLOW_BUTTON, false};
Button greenButton = {GREEN_BUTTON, false};
Button whiteButton = {WHITE_BUTTON, false};

Slider slider1 = {SLIDER_1};
Slider slider2 = {SLIDER_2};
Slider slider3 = {SLIDER_3};
Slider slider4 = {SLIDER_4};

auto sliderToBrightness = scale(1024, 3, 0, 255, true);
auto sliderToColorPalette = scale(972, 5, 8, 0, true);
auto sliderToSpeed = scale(1000, 0, 1, 10, true);

void send(msg m) {
  esp_now_send(0, (uint8_t *) &m, sizeof(m));
}

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
  pinMode(WHITE_BUTTON, INPUT_PULLUP);
  pinMode(SLIDER_1, OUTPUT);
  pinMode(SLIDER_2, OUTPUT);
  pinMode(SLIDER_3, OUTPUT);
  pinMode(SLIDER_4, OUTPUT);
  pinMode(A0, INPUT);
}

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

void setBackground(int viz) {
  background.value1 = background.value1 == viz ? VIZ_DEFAULT : viz;
}

void cycleBackground() {
  int numBackgrounds = sizeof(backgrounds) / sizeof(backgrounds[0]);
  int currentIndex = 0;
  for (int i = 0; i < numBackgrounds; i++) {
    if (backgrounds[i] == background.value1) {
      currentIndex = i;
    }
  }
  int newIndex = currentIndex < numBackgrounds - 1 ? currentIndex + 1 : 0;
  background.value1 = backgrounds[newIndex];
}

void loop() {
  // Every N seconds, cycle through the active background viz
  EVERY_N_SECONDS(120) {
    cycleBackground();
    send(background);
  }

  EVERY_N_SECONDS(10) {
    activePalette = activePalette == numPalettes - 1 ? 0 : activePalette + 1;
    blendPalette.value1 = activePalette;
    send(blendPalette);
  }

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

  slider2.value = getSliderValue(slider2);
  if (sliderValueChanged(slider2)) {
    Serial.print("Slider 2 changed ");
    Serial.println(slider2.value);
    activePalette = sliderToColorPalette(slider2.value);
    palette.value1 = activePalette;
    Serial.print("Palette ");
    Serial.println(palette.value1);
    send(palette);
    slider2.prev = slider2.value;
  }

  slider3.value = getSliderValue(slider3);
  if (sliderValueChanged(slider3)) {
    Serial.print("Slider 3 changed ");
    Serial.println(slider3.value);
    speed.value1 = sliderToSpeed(slider3.value);
    Serial.print("Speed ");
    Serial.println(speed.value1);
    send(speed);
    slider3.prev = slider3.value;
  }

  slider4.value = getSliderValue(slider4);
  if (sliderValueChanged(slider4)) {
    Serial.print("Slider 4 changed ");
    Serial.println(slider4.value);
    slider4.prev = slider4.value;
  }

  if (isButtonPressed(redButton) && !redButton.pressed) {
    Serial.println("Red button pressed (Pride/Wipe)");
    redButton.pressed = true;

    if (mode == BACKGROUND_MODE) {
      setBackground(VIZ_PRIDE);
      send(background);
    } else {
      send(wipe);
    }
  } else {
    redButton.pressed = false;
  }

  if (isButtonPressed(yellowButton) && !yellowButton.pressed) {
    Serial.println("Yellow button pressed (Twinkle/Strobe)");
    yellowButton.pressed = true;

    if (mode == BACKGROUND_MODE) {
      setBackground(VIZ_TWINKLE);
      send(background);
    } else {
      send(strobe);
    }
  } else {
    yellowButton.pressed = false;
  }

  if (isButtonPressed(blueButton) && !blueButton.pressed) {
    Serial.println("Blue button pressed (Starfield/Ripple)");
    blueButton.pressed = true;

    if (mode == BACKGROUND_MODE) {
      setBackground(VIZ_STARFIELD);
      send(background);
    } else {
      ripple.value1 = random(1, NUM_STRIPS - 1);
      ripple.value2 = random(1, NUM_LEDS - 1);
      send(ripple);
    }
  } else {
    blueButton.pressed = false;
  }

  if (isButtonPressed(greenButton) && !greenButton.pressed) {
    Serial.println("Green button pressed (Juggle/Clap)");
    greenButton.pressed = true;

    if (mode == BACKGROUND_MODE) {
      setBackground(VIZ_JUGGLE);
      send(background);
    } else {
      // TODO clap
    }
  } else {
    greenButton.pressed = false;
  }

  if (isButtonPressed(whiteButton)) {
    if (!whiteButton.pressed) {
      Serial.println("White button pressed (Overlay mode)");
      whiteButton.pressed = true;
      mode = OVERLAY_MODE;
    }
  } else if (whiteButton.pressed) {
    Serial.println("White button UN-pressed (Background mode)");
    whiteButton.pressed = false;
    mode = BACKGROUND_MODE;
  }
}
