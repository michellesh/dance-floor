// Ripple visualizations

uint8_t convert(int16_t value) {
  if (value <= 0) {
    return 0;
  } else if (value >= 255) {
    return 255;
  } else {
    return (uint8_t)value;
  }
}

void viz_ripple(CRGB color) {
  float DAMPENING = 0.99;
  for (int i = 1; i < NUM_STRIPS - 1; i++) {
    for (int j = 1; j < NUM_LEDS - 1; j++) {
      current[i][j] = (
        previous[i-1][j] +
        previous[i+1][j] +
        previous[i][j-1] +
        previous[i][j+1]) / 2 -
        current[i][j];
      current[i][j] = current[i][j] * DAMPENING;
      int16_t value = convert(current[i][j]);
      if (value > 0) {
        leds[i][j] = color;
        leds[i][j].fadeLightBy(255 - value);
      }
    }
  }

  int16_t temp;
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      temp = previous[i][j];
      previous[i][j] = current[i][j];
      current[i][j] = temp;
    }
  }
}
