int geti(int pixelnumber) {
  return pixelnumber / NUM_LEDS;
}

int getj(int pixelnumber) {
  return pixelnumber - (geti(pixelnumber) * NUM_LEDS);
}

void set_all(CRGB color) {
  for (uint16_t i = 0; i < NUM_STRIPS; i++) {
    for (uint16_t j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = color;
    }
  }
}

void set_strip(uint8_t strip_number, CRGB color, uint8_t num_leds = NUM_LEDS) {
  for (uint16_t j = 0; j < num_leds; j++) {
    leds[strip_number][j] = color;
  }
}

float mapf(float value, float inMin, float inMax, float outMin, float outMax) {
  float percentage = (value - inMin) / (inMax - inMin);
  return outMin + (outMax - outMin) * percentage;
}
