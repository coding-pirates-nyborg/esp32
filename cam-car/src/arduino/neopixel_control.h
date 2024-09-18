#include <FastLED.h>
#include <constants.h>

/*----------------------NEO_PIXEL---------------------*/

#define NUM_LEDS    14
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 100
#define BRIGHTNESS  64

CRGB leds[NUM_LEDS];

