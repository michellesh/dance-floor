#define NUM_STARS  400
#define MAX_DEPTH  255
#define MIN_SPEED  0.01
#define MAX_SPEED  5

uint8_t maxDepth = 255;
typedef struct Star {
  uint8_t strand;
  float startY;
  float currentY;
  float endY;
} Star;
Star stars[NUM_STARS];

void init_starfield() {
  for (int x = 0; x < NUM_STARS; x++) {
    uint8_t strand = (uint8_t)random(0, NUM_STRIPS);
    float startY = random(-50, NUM_LEDS);
    float endY = random(NUM_LEDS, NUM_LEDS * 2);
    float currentY = random(startY, endY);
    stars[x] = {strand, startY, currentY, endY};
  }
}

void viz_starfield(CRGB color, float speed) {
  FastLED.clear();
  for (int x = 0; x < NUM_STARS; x++) {
    stars[x].currentY += speed * mapf(stars[x].currentY, stars[x].startY, stars[x].endY, MIN_SPEED, MAX_SPEED);
    if (stars[x].currentY >= NUM_LEDS - 1) {
      uint8_t strand = (uint8_t)random(0, NUM_STRIPS);
      float startY = random(-50, NUM_LEDS);
      float endY = random(NUM_LEDS, NUM_LEDS * 2);
      stars[x] = {strand, startY, startY, endY};
    }
    uint8_t value = mapf(stars[x].currentY, stars[x].startY, stars[x].endY, 0, 255);
    int currentY = (int)round(stars[x].currentY);
    if (currentY >= 0) {
      leds[stars[x].strand][currentY] = color;
      leds[stars[x].strand][currentY].nscale8(value);
      addTail(stars[x].strand, currentY, value, color);
    }
  }
}

void addTail(uint8_t strand, int currentY, uint8_t value, CRGB color) {
  int tailLength = map(value, 0, 255, 2, 10);
  float step = value / tailLength;
  for (int i = 1; i <= tailLength; i++) {
    uint8_t stepValue = value - (i * step);
    if (current - i > 0) {
      leds[strand][currentY - i] = color;
      leds[strand][currentY - i].nscale8(stepValue);
    }
  }
}
