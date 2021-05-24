#define NUM_LEDS        150     // TODO will depend on strand
#define NUM_STRIPS      28
#define STRIPS_PER_SAIL 7

#define ACTION_CYCLE_BACKGROUND  1
#define ACTION_RIPPLE            2
#define ACTION_SET_BRIGHTNESS    3
#define ACTION_WINDSHIELD        4
#define ACTION_SET_PALETTE       5
#define ACTION_CYCLE_PALETTE     6
#define ACTION_SPEED             7

#define VIZ_PRIDE     1
#define VIZ_TWINKLE   2
#define VIZ_PACIFICA  3
#define VIZ_STARFIELD 4
#define VIZ_BPM       5
#define VIZ_JUGGLE    6

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
