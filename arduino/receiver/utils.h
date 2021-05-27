void set_all(CRGB color, uint8_t brightness) {
  for (uint16_t i = 0; i < NUM_STRIPS; i++) {
    for (uint16_t j = 0; j < NUM_LEDS[i]; j++) {
      leds[i][j] = color;
      leds[i][j].nscale8(brightness);
    }
  }
}

void set_strip(uint8_t strip_number, CRGB color, uint8_t brightness) {
  for (uint16_t j = 0; j < NUM_LEDS[strip_number]; j++) {
    leds[strip_number][j] = color;
    leds[strip_number][j].nscale8(brightness);
  }
}

void set_index(int index, CRGB color, uint8_t brightness) {
  for (uint16_t i = 0; i < NUM_STRIPS; i++) {
    if (index < NUM_LEDS[i]) {
      leds[i][index] = color;
      leds[i][index].nscale8(brightness);
    }
  }
}

float mapf(float value, float inMin, float inMax, float outMin, float outMax) {
  float percentage = (value - inMin) / (inMax - inMin);
  return outMin + (outMax - outMin) * percentage;
}
