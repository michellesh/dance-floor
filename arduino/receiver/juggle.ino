// Eight colored dots, weaving in and out of sync with each other

void viz_juggle() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    fadeToBlackBy(leds[i], NUM_LEDS, 20);
  }
  uint8_t dothue = 0;
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i][beatsin16(j + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
  }
}
