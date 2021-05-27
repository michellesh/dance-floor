#include <FastLED.h>

#define BRIGHTNESS      255
#define COLOR_ORDER     GRB
#define LED_TYPE        WS2813

//CRGB leds[NUM_STRIPS][NUM_LEDS];
CRGB leds_0[LEDS_0];
CRGB leds_1[LEDS_1];
CRGB leds_2[LEDS_2];
CRGB leds_3[LEDS_3];
CRGB leds_4[LEDS_4];
CRGB leds_5[LEDS_5];
CRGB leds_6[LEDS_6];
CRGB leds_7[LEDS_7];
CRGB leds_8[LEDS_8];
CRGB leds_9[LEDS_9];
CRGB leds_10[LEDS_10];
CRGB leds_11[LEDS_11];
CRGB leds_12[LEDS_12];
CRGB leds_13[LEDS_13];
CRGB leds_14[LEDS_14];
CRGB leds_15[LEDS_15];
CRGB leds_16[LEDS_16];
CRGB leds_17[LEDS_17];
CRGB leds_18[LEDS_18];
CRGB leds_19[LEDS_19];
CRGB leds_20[LEDS_20];
CRGB leds_21[LEDS_21];
CRGB leds_22[LEDS_22];
CRGB leds_23[LEDS_23];
CRGB leds_24[LEDS_24];
CRGB leds_25[LEDS_25];
CRGB leds_26[LEDS_26];
CRGB leds_27[LEDS_27];
CRGB *leds[] = {
  leds_0, leds_1, leds_2, leds_3, leds_4, leds_5, leds_6,
  leds_7, leds_8, leds_9, leds_10, leds_11, leds_12, leds_13,
  leds_14, leds_15, leds_16, leds_17, leds_18, leds_19, leds_20,
  leds_21, leds_22, leds_23, leds_24, leds_25, leds_26, leds_27
};

msg data;

uint8_t numEchos = 5;
uint8_t echos[] = {STRAND_MAX, STRAND_MAX, STRAND_MAX, STRAND_MAX, STRAND_MAX};
uint8_t wipeIndex = NUM_STRIPS;
uint8_t setBrightness = BRIGHTNESS;
uint8_t setBackgroundBrightness = BRIGHTNESS;
uint8_t activeViz = 0;
uint8_t speed = 1;
bool strobeOn = false;

//int16_t current[NUM_STRIPS][NUM_LEDS];
//int16_t previous[NUM_STRIPS][NUM_LEDS];
int16_t current_0[LEDS_0];
int16_t current_1[LEDS_1];
int16_t current_2[LEDS_2];
int16_t current_3[LEDS_3];
int16_t current_4[LEDS_4];
int16_t current_5[LEDS_5];
int16_t current_6[LEDS_6];
int16_t current_7[LEDS_7];
int16_t current_8[LEDS_8];
int16_t current_9[LEDS_9];
int16_t current_10[LEDS_10];
int16_t current_11[LEDS_11];
int16_t current_12[LEDS_12];
int16_t current_13[LEDS_13];
int16_t current_14[LEDS_14];
int16_t current_15[LEDS_15];
int16_t current_16[LEDS_16];
int16_t current_17[LEDS_17];
int16_t current_18[LEDS_18];
int16_t current_19[LEDS_19];
int16_t current_20[LEDS_20];
int16_t current_21[LEDS_21];
int16_t current_22[LEDS_22];
int16_t current_23[LEDS_23];
int16_t current_24[LEDS_24];
int16_t current_25[LEDS_25];
int16_t current_26[LEDS_26];
int16_t current_27[LEDS_27];
int16_t *current[] = {
  current_0, current_1, current_2, current_3, current_4, current_5, current_6,
  current_7, current_8, current_9, current_10, current_11, current_12, current_13,
  current_14, current_15, current_16, current_17, current_18, current_19, current_20,
  current_21, current_22, current_23, current_24, current_25, current_26, current_27
};
int16_t previous_0[LEDS_0];
int16_t previous_1[LEDS_1];
int16_t previous_2[LEDS_2];
int16_t previous_3[LEDS_3];
int16_t previous_4[LEDS_4];
int16_t previous_5[LEDS_5];
int16_t previous_6[LEDS_6];
int16_t previous_7[LEDS_7];
int16_t previous_8[LEDS_8];
int16_t previous_9[LEDS_9];
int16_t previous_10[LEDS_10];
int16_t previous_11[LEDS_11];
int16_t previous_12[LEDS_12];
int16_t previous_13[LEDS_13];
int16_t previous_14[LEDS_14];
int16_t previous_15[LEDS_15];
int16_t previous_16[LEDS_16];
int16_t previous_17[LEDS_17];
int16_t previous_18[LEDS_18];
int16_t previous_19[LEDS_19];
int16_t previous_20[LEDS_20];
int16_t previous_21[LEDS_21];
int16_t previous_22[LEDS_22];
int16_t previous_23[LEDS_23];
int16_t previous_24[LEDS_24];
int16_t previous_25[LEDS_25];
int16_t previous_26[LEDS_26];
int16_t previous_27[LEDS_27];
int16_t *previous[] = {
  previous_0, previous_1, previous_2, previous_3, previous_4, previous_5, previous_6,
  previous_7, previous_8, previous_9, previous_10, previous_11, previous_12, previous_13,
  previous_14, previous_15, previous_16, previous_17, previous_18, previous_19, previous_20,
  previous_21, previous_22, previous_23, previous_24, previous_25, previous_26, previous_27
};

uint8_t gHue = 0; // used for bpm pattern


/** COLORS **/

CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;
CRGB currentColor = CRGB::White;

#define Bride_Dress   0xDED1D0
#define Maid_Of_Honor 0x005C49
#define Light_Blue    0x809EB0
#define Bronze1       0xEAB597
#define Bronze2       0xF0AE62
#define Bronze3       0xCA745C
const TProgmemRGBPalette16 BridalPalette_p FL_PROGMEM =
{ Bride_Dress, Bride_Dress, Bride_Dress, Bride_Dress,
  Bronze1, Bronze1, Bronze1, Bronze2,
  Bronze2, Bronze2, Bronze3, Bronze3,
  Maid_Of_Honor, Maid_Of_Honor, Light_Blue, Light_Blue
};

#define Navy1    0x2B4873
#define Navy2    0x5D6071
#define DarkNavy 0x1B365D
#define Shoe     0xA4674F
const TProgmemRGBPalette16 Groomsmen_p FL_PROGMEM = {
  Navy1, Navy1, Navy1, Navy1,
  Navy2, Navy2, Navy2, Navy2,
  DarkNavy, DarkNavy, DarkNavy, DarkNavy,
  Shoe, Shoe, Shoe, 0x91514D
};

// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 Red_p FL_PROGMEM = {
  CRGB::Red, CRGB::Red, CRGB::Red, CRGB::MistyRose,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
  CRGB::Maroon, CRGB::FireBrick, CRGB::OrangeRed, CRGB::Gold,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray
};

const TProgmemRGBPalette16 OrangeYellow_p FL_PROGMEM = {
  CRGB::OrangeRed, CRGB::DarkOrange, CRGB::Orange, CRGB::Orange,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
  CRGB::Goldenrod, CRGB::LightYellow, CRGB::DarkGoldenrod, CRGB::Yellow,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray
};

const TProgmemRGBPalette16 Green_p FL_PROGMEM = {
  CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green,
  CRGB::ForestGreen, CRGB::DarkGreen, CRGB::DarkOliveGreen, CRGB::DarkSeaGreen,
  CRGB::ForestGreen, CRGB::ForestGreen, CRGB::LawnGreen, CRGB::PaleGreen,
  CRGB::SpringGreen, CRGB::Gray, CRGB::Gray, CRGB::Gray
};

const TProgmemRGBPalette16 Teal_p FL_PROGMEM = {
  CRGB::Teal, CRGB::Teal, CRGB::Teal, CRGB::Tan,
  CRGB::Turquoise, CRGB::PaleTurquoise, CRGB::MediumTurquoise, CRGB::DarkTurquoise,
  CRGB::MintCream, CRGB::Aquamarine, CRGB::MediumAquamarine, CRGB::Teal,
  CRGB::Teal, CRGB::Gray, CRGB::Gray, CRGB::Gray
};

const TProgmemRGBPalette16 Blue_p FL_PROGMEM = {
  CRGB::DarkSlateBlue, CRGB::DodgerBlue, CRGB::LightSkyBlue, CRGB::LightBlue,
  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue,
  CRGB::AliceBlue, CRGB::DarkBlue, CRGB::CadetBlue, CRGB::CornflowerBlue,
  CRGB::Blue, CRGB::Cyan, CRGB::Gray, CRGB::Gray
};

const TProgmemRGBPalette16 PurplePink_p FL_PROGMEM = {
  CRGB::Purple, CRGB::Purple, CRGB::Purple, CRGB::BlueViolet,
  CRGB::Pink, CRGB::Amethyst, CRGB::DeepPink, CRGB::HotPink,
  CRGB::Fuchsia, CRGB::MediumPurple, CRGB::Violet, CRGB::Orchid,
  CRGB::Lavender, CRGB::Gray, CRGB::Gray, CRGB::Gray
};

// A pure "fairy light" palette with some brightness variations
#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)
const TProgmemRGBPalette16 FairyLight_p FL_PROGMEM = {
  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight,
  HALFFAIRY, HALFFAIRY, CRGB::FairyLight, CRGB::FairyLight,
  QUARTERFAIRY, QUARTERFAIRY, CRGB::FairyLight, CRGB::FairyLight,
  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight
};

// Add or remove palette names from this list to control which color
// palettes are used, and in what order.
const TProgmemRGBPalette16* ActivePaletteList[] = {
  &BridalPalette_p,
  &Red_p,
  &OrangeYellow_p,
  &Green_p,
  &Teal_p,
  &Blue_p,
  &PurplePink_p,
  &Groomsmen_p,
  &FairyLight_p,
};
const CRGB colorList[] = {
  CRGB::White,
  CRGB::Red,
  CRGB::Orange,
  CRGB::Yellow,
  CRGB::Green,
  CRGB::Teal,
  CRGB::Navy,
  CRGB::Purple,
  CRGB::Pink,
};

// Advance to the next color palette in the list (above).
void chooseNextColorPalette( CRGBPalette16& pal) {
  const uint8_t numberOfPalettes = sizeof(ActivePaletteList) / sizeof(ActivePaletteList[0]);
  static uint8_t whichPalette = -1;
  whichPalette = addmod8( whichPalette, 1, numberOfPalettes);

  pal = *(ActivePaletteList[whichPalette]);
}
