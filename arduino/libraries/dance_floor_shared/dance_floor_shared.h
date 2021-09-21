//#define NUM_LEDS        150     // TODO will depend on strand
#define NUM_STRIPS      28
#define STRAND_MAX      207
#define STRIPS_PER_SAIL 7

#define ACTION_SET_BACKGROUND     1
#define ACTION_RIPPLE             2
#define ACTION_SET_BRIGHTNESS     3
#define ACTION_WIPE               4
#define ACTION_SET_PALETTE        5
#define ACTION_SET_HUE            6
#define ACTION_SPEED              7
#define ACTION_STROBE_ON          8
#define ACTION_STROBE_OFF         9
#define ACTION_ECHO               10
#define ACTION_SET_BG_BRIGHTNESS  11

#define VIZ_DEFAULT    0
#define VIZ_PRIDE      1
#define VIZ_TWINKLE    2
#define VIZ_PACIFICA   3
#define VIZ_STARFIELD  4
#define VIZ_BPM        5
#define VIZ_JUGGLE     6

// Blue
#define LEDS_0   180  // 30
#define LEDS_1   140  // -10
#define LEDS_2   124  // -26
#define LEDS_3   122  // -28
#define LEDS_4   122  // -28
#define LEDS_5   127  // -23
#define LEDS_6   134  // -16

// Green
#define LEDS_7   207  // 57
#define LEDS_8   181  // 31
#define LEDS_9   170  // 20
#define LEDS_10  166  // 16
#define LEDS_11  166  // 16
#define LEDS_12  177  // 27
#define LEDS_13  196  // 46

// Yellow
#define LEDS_14  195  // 45
#define LEDS_15  175  // 25
#define LEDS_16  170  // 20
#define LEDS_17  168  // 18
#define LEDS_18  173  // 23
#define LEDS_19  178  // 28
#define LEDS_20  205  // 55

// Red
#define LEDS_21  144  // -6
#define LEDS_22  131  // -19
#define LEDS_23  128  // -22
#define LEDS_24  128  // -22
#define LEDS_25  134  // -16
#define LEDS_26  144  // -6
#define LEDS_27  190  // 40

int NUM_LEDS[] = {
  LEDS_0, LEDS_1, LEDS_2, LEDS_3, LEDS_4, LEDS_5, LEDS_6,
  LEDS_7, LEDS_8, LEDS_9, LEDS_10, LEDS_11, LEDS_12, LEDS_13,
  LEDS_14, LEDS_15, LEDS_16, LEDS_17, LEDS_18, LEDS_19, LEDS_20,
  LEDS_21, LEDS_22, LEDS_23, LEDS_24, LEDS_25, LEDS_26, LEDS_27,
};

typedef struct msg {
  uint8_t action;
  int value1;
  int value2;
} msg;

auto scale(float domainStart, float domainEnd, float rangeStart, float rangeEnd, bool clamp = false) {
  return [=](float value) {
    float percentage = (value - domainStart) / (domainEnd - domainStart);
    float newValue = rangeStart + (rangeEnd - rangeStart) * percentage;
    return clamp && newValue < min(rangeStart, rangeEnd)   ? min(rangeStart, rangeEnd)
           : clamp && newValue > max(rangeStart, rangeEnd) ? max(rangeStart, rangeEnd)
                                                           : newValue;
  };
}
