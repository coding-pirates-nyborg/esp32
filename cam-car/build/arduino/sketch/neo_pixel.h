#line 1 "/home/frey/coding-pirates/esp32/cam-car/src/arduino/neo_pixel.h"
#ifndef NEO_PIXEL_H
#define NEO_PIXEL_H

#include <FastLED.h>

struct NeoPixel {
    int led_index;
    CRGB rgb_color;
};

#endif // NEO_PIXEL_H