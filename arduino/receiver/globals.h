#include <FastLED.h>

#define BRIGHTNESS      255
#define COLOR_ORDER     GRB
#define LED_TYPE        WS2813

CRGB leds[NUM_STRIPS][NUM_LEDS];
msg data;

uint8_t wipeIndex = NUM_STRIPS;
uint8_t setBrightness = BRIGHTNESS;
uint8_t activeViz = 0;
uint8_t speed = 1;
uint8_t strobeColor = 0;

int16_t current[NUM_STRIPS][NUM_LEDS];
int16_t previous[NUM_STRIPS][NUM_LEDS];

uint8_t gHue = 0; // used for bpm pattern


/** COLORS **/

CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;

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

// Advance to the next color palette in the list (above).
void chooseNextColorPalette( CRGBPalette16& pal) {
  const uint8_t numberOfPalettes = sizeof(ActivePaletteList) / sizeof(ActivePaletteList[0]);
  static uint8_t whichPalette = -1;
  whichPalette = addmod8( whichPalette, 1, numberOfPalettes);

  pal = *(ActivePaletteList[whichPalette]);
}
