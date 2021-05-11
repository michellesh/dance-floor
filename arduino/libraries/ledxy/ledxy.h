/*
 * Everything needed to calculate the XY coordinates of each LED
 */
#define NUM_LEDS     150
#define NUM_PINS     7
#define NUM_STRIPS   28

#define HEIGHT       900
#define NUM_STRANDS  7
#define RADIUS_X     100
#define WIDTH        1100
#define STRIP_START  0.3

struct XY {
  float x;
  float y;
};

struct Sail {
  XY p1;
  XY p2;
  XY p3;
};

Sail sail1 = {{80, 500}, {400, 795}, {50, 850}};
Sail sail2 = {{410, 100}, {470, 650}, {90, 325}};
Sail sail3 = {{1000, 325}, {630, 650}, {690, 100}};
Sail sail4 = {{1050, 850}, {700, 795}, {1020, 500}};

// Get a point along the line that forms between p1 and p2
// howFar value between 0 and 1, how far along that line
XY pointOnLine(XY p1, XY p2, float howFar) {
  float x = p1.x + (p2.x - p1.x) * howFar;
  float y = p1.y + (p2.y - p1.y) * howFar;
  return {x, y};
}

// Get the xy coordinates for a given LED
// where i is the index of the strand
// and j is the index of the led on the strand
XY ledxy(int i, int j) {
  Sail sail = i >= 0 && i < NUM_STRANDS                     ? sail1
              : i >= NUM_STRANDS && i < NUM_STRANDS * 2     ? sail2
              : i >= NUM_STRANDS * 2 && i < NUM_STRANDS * 3 ? sail3
                                                            : sail4;
  int iSail = i - NUM_STRANDS * (i % NUM_STRANDS);
  XY pInc = pointOnLine(sail.p3, sail.p1, (float)iSail / (NUM_STRANDS - 1));
  return pointOnLine(sail.p2, pInc, (float)j / (NUM_LEDS - 1));
}

/*
// Alternatively, store the xy coordinates in a 2D array that matches the shape
// of the FastLED array. Will use this approach if calculating the coordinates
// on the fly ends up slowing down the visualizations
XY ledxy[NUM_STRIPS][NUM_LEDS];

void getSailLEDs(int startIndex, Sail sail) {
  for (int i = 0; i < NUM_STRANDS; i++) {
    XY pInc = pointOnLine(sail.p3, sail.p1, (float)i / (NUM_STRANDS - 1));
    for (int j = 0; j < NUM_LEDS; j++) {
      ledxy[i + startIndex][j] = pointOnLine(sail.p2, pInc, (float)j / (NUM_LEDS - 1));
    }
  }
}

void getLEDxy() {
  getSailLEDs(0, {{80, 500}, {400, 795}, {50, 850}}); // sail 1
  getSailLEDs(NUM_STRANDS, {{410, 100}, {470, 650}, {90, 325}}); // sail 2
  getSailLEDs(NUM_STRANDS * 2, {{1000, 325}, {630, 650}, {690, 100}}); // sail 3
  getSailLEDs(NUM_STRANDS * 3, {{1050, 850}, {700, 795}, {1020, 500}}); // sail 4
}
*/
