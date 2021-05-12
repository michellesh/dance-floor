/* Ripple visualizations
 */
float RIPPLE_WIDTH = 50;
float DAMPENING = 0.99;
float MAX_RIPPLE_COLOR = 1;

void viz_ripple() {
  for (int i = 1; i < STRIPS_PER_SAIL - 1; i++) {
    for (int j = 1; j < NUM_LEDS - 1; j++) {
      current[i][j] = (
        previous[i-1][j] +
        previous[i+1][j] +
        previous[i][j-1] +
        previous[i][j+1]) / 2 -
        current[i][j];
      current[i][j] = current[i][j] * DAMPENING;
      if (current[i][j] > 0 && current[i][j] < 255) {
        leds[i][j] = CRGB(current[i][j], current[i][j], current[i][j]);
      }
    }
  }

  for (int i = 0; i < STRIPS_PER_SAIL; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      temp[i][j] = previous[i][j];
    }
  }

  for (int i = 0; i < STRIPS_PER_SAIL; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      previous[i][j] = current[i][j];
    }
  }

  for (int i = 0; i < STRIPS_PER_SAIL; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      current[i][j] = temp[i][j];
    }
  }
}

/* A ripple viz based on the xy coordinate plane. Too slow currently
 */
void viz_ripple_xy(float radius, XY center) {
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      float d = distance(ledxy(i, j), center);
      if (d < radius && d > radius - RIPPLE_WIDTH) {
        leds[i][j] = CRGB::White;
      }
    }
  }
}
