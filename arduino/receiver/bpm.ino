// colored stripes pulsing at a defined Beats-Per-Minute (BPM)

void bpm() {
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_STRIPS * NUM_LEDS; i++) { //9948
    leds[geti(i)][getj(i)] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}
